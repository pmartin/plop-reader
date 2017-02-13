#include "application.h"
#include "log.h"


#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}



void Application::init()
{
	DEBUG("Initializing the application")

	// Note: this is only left for debugging purposes, if one needs to
	// empty the database easily
	//db.drop();

	db.runMigrations();


	WallabagConfigLoader configLoader;
	WallabagConfig config = configLoader.load();
	wallabag_api.setConfig(config);

	gui.init();

	curl_global_init(CURL_GLOBAL_ALL);

	restoreModeAndPage();
}


void Application::deinit()
{
	DEBUG("De-initializing the application")

	curl_global_cleanup();
}


// To fetch data from server (or sync data to server), a network connection is required ;-)
bool Application::connectToNetwork()
{
	iv_netinfo *netinfo = NetInfo();
	if (netinfo->connected) {
		// Already connected => nothing more to do
		return true;
	}

	const char *network_name = NULL;
	int result = NetConnect2(network_name, 1);
	if (result != 0) {
		WARN("Failed to connect to network");
		return false;
	}

	// Just to be sure: check if we are, now, connected
	netinfo = NetInfo();
	if (netinfo->connected) {
		return true;
	}

	WARN("Failed to connect to network - not sure why");
	return false;
}


void Application::loadRecentArticles()
{
	INFO("Starting synchronization process");

	if (!connectToNetwork()) {
		WARN("No network connection => cannot synchronize");
		Message(ICON_ERROR, "Synchronization failed!", "No active network connection, no way to synchronize!", 3*1000);

		return;
	}

	gui.openProgressBar(ICON_WIFI, LBL_SYNC_DIALOG_TITLE, LBL_SYNC_START_SYNC, 0, [](int button) {});
	gui.updateProgressBar(LBL_SYNC_FETCHING_RECENT_ENTRIES, Gui::SYNC_PROGRESS_PERCENTAGE_ALL_START);

	Internal lastSyncTimestampObj = db.selectInternal("sync.last-sync.timestamp");
	time_t lastSyncTimestamp = lastSyncTimestampObj.isNull ? 0 : atoi(lastSyncTimestampObj.value.c_str());
	DEBUG("Last sync TS = %ld", lastSyncTimestamp);

	db.saveInternal("sync.last-sync.timestamp", SSTR(time(NULL)));
	DEBUG("Saving last sync TS = %ld", time(NULL));

	try {
		wallabag_api.loadRecentArticles(entryRepository, epubDownloadQueueRepository, lastSyncTimestamp, [](const char *text, int percent, void *context) {
			app.gui.updateProgressBar(text, percent);
		});

		// Send changes to server, for entries marked as archived/starred recently on the device
		wallabag_api.syncEntriesToServer(entryRepository, [](const char *text, int percent, void *context) {
			app.gui.updateProgressBar(text, percent);
		});

		gui.updateProgressBar(LBL_SYNC_SUCCESS_DONE, Gui::SYNC_PROGRESS_PERCENTAGE_ALL_DONE);
	}
	catch (SyncAbortAllOperations &e) {
		ERROR("Synchronization Exception: %s => aborting sync!", e.what());
		DialogSynchro(ICON_ERROR, PLOP_APPLICATION_FULLNAME, e.what(), LBL_SYNC_FAILED_TOO_BAD, NULL, NULL);
	}
	gui.closeProgressBar();

	INFO("End of synchronization process");

	// Syncing changes the number of entries (it can add or remove entries in each mode) => go back to page one
	pageNum = 1;

	show();
}


void Application::setMode(int m, int forcePageNum)
{
	mode = (entries_mode)m;
	pageNum = forcePageNum > 0 ? forcePageNum : 1;

	DEBUG("Switching to mode %d", mode);
	gui.setMode(mode);

	show();
}


void Application::show()
{
	std::vector<Entry> entries;
	listEntriesForCurrentMode(entries);

	int countEntries = countEntriesForCurrentMode();
	int numberOfPages = ceil((float)countEntries / (float)numPerPage);

	DEBUG("Showing entries: page:%d/%d (total:%d)", pageNum, numberOfPages, countEntries);
	gui.show(pageNum, numberOfPages, countEntries, entries);

	saveModeAndPage();
}


void Application::touchStartEvent(int x, int y)
{
	gui.touchStartEvent(x, y);
}


void Application::touchEndEvent(int x, int y)
{
	isLastActionRead = false;
	lastReadEntryId = 0;

	gui.touchEndEvent(x, y);
}

void Application::touchLong(int x, int y)
{
	gui.touchLong(x, y);
}


void Application::keypressEvent(int key)
{
	int countEntries = countEntriesForCurrentMode();
	int numberOfPages = ceil((float)countEntries / (float)numPerPage);

	if (key == KEY_PREV && pageNum > 1) {
		pageNum -= 1;
	}

	if (key == KEY_NEXT && pageNum < numberOfPages) {
		pageNum += 1;
	}

	show();
}



static std::string replaceAll(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void Application::read(Entry &entry, reading_format format)
{
	DEBUG("Trying to read entry %d / %s", entry.id, entry.remote_id.c_str());

	if (format == FORMAT_EPUB && !entry.local_content_file_epub.empty()) {
		// We have a path to a local EPUB file
		int statResult;
		struct stat st;
		if ((statResult = iv_stat(entry.local_content_file_epub.c_str(), &st)) == 0 && st.st_size > 0) {
			// And the file exists and is not empty => it could be OK ;-)
			DEBUG("EPUB file %s : stat=%d and size=%ld", entry.local_content_file_epub.c_str(), statResult, st.st_size);

			// We have a local EPUB file for the entry, it's the best format to read on an ereader => use it \o/
			DEBUG("Opening reading application for EPUB: %s", entry.local_content_file_epub.c_str());
			OpenBook(entry.local_content_file_epub.c_str(), "r", 0);

			isLastActionRead = true;
			lastReadEntryId = entry.id;

			return;
		}
		else {
			// The local file doesn't exist or is empty => we cannot use it => we'll fallback on the HTML content
			DEBUG("EPUB file %s : stat=%d and size=%ld => try to fallback on HTML content", entry.local_content_file_epub.c_str(), statResult, st.st_size);
		}
	}

	if (entry.local_content_file_html.empty()) {
		// We don't have the HTML file for the entry yet
		// => create it from the content stored in DB
		// TODO create (or delete) the local file when syncing with server ;-)

		char filepath[2048];
		snprintf(filepath, sizeof(filepath), PLOP_ENTRIES_CONTENT_DIRECTORY "/entry-%d.html", entry.id);
		entry.local_content_file_html = filepath;

		//Message(ICON_INFORMATION, "Opening entry... One day!", filepath, 1*1000);

		// TODO no hard-coded path, and create this directory somewhere else and all...
		iv_mkdir(PLOP_ENTRIES_CONTENT_DIRECTORY, 0755);

		DEBUG("Creating local file %s for entry %d", entry.local_content_file_html.c_str(), entry.id);
		FILE *fp = iv_fopen(filepath, "wb");

		const char *str = R"html(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8">
    </head>
    <body>

)html";
		iv_fwrite(str, sizeof(char), strlen(str), fp);

		// Yeah, I known ;-(
		// It seems the ereader's HTML reader doesn't recognize some characters => replace those ;-(
		// TODO find a better way... hopefully, there is a way to force the ereader to accept all those characters?
		std::string tmp = replaceAll(entry.content, "’", "'");;
		tmp = replaceAll(tmp, "“", "\"");
		tmp = replaceAll(tmp, "”", "\"");
		tmp = replaceAll(tmp, "\xC2\xA0", " ");
		tmp = replaceAll(tmp, "\xE2\x80\x8A", " ");
		tmp = replaceAll(tmp, "—", "--");
		tmp = replaceAll(tmp, "…", "...");
		tmp = replaceAll(tmp, "€", "&euro;");
		tmp = replaceAll(tmp, "\x20\x18", "'");
		tmp = replaceAll(tmp, "\x20\x19", "'");

		iv_fwrite(tmp.c_str(), sizeof(char), tmp.size(), fp);
		iv_fwrite("</body></html>", sizeof(char), strlen("</body></html>"), fp);

		iv_fclose(fp);

		// TODO we need a 'persist' method that updates the entry, and not insert a new one...
		//entryRepository.persist(entry);
	}

	if (!entry.local_content_file_html.empty()) {
		// We have an HTML file => open the reader and profit!
		DEBUG("Opening reading application for %s", entry.local_content_file_html.c_str());
		OpenBook(entry.local_content_file_html.c_str(), "r", 0);

		isLastActionRead = true;
		lastReadEntryId = entry.id;
	}
}


void Application::handleActionOnReadEntry(int entryId)
{
	Entry entry = entryRepository.get(entryId);

	char buffer[2048];
	snprintf(buffer, sizeof(buffer), LBL_AFTERREAD_ACTIONONENTRY_WHAT_TO_DO_WITH_ENTRY_CONTENT, entry.id, entry.title.c_str());

	bool isArchived = entry.local_is_archived;
	bool isStarred = entry.local_is_starred;

	const char *strButton1, *strButton2;

	if (isArchived == false) {
		strButton1 = LBL_AFTERREAD_ACTIONONENTRY_BUTTON_ARCHIVE;
		if (isStarred == false) {
			strButton2 = LBL_AFTERREAD_ACTIONONENTRY_BUTTON_ARCHIVE_STAR;
		}
		else {
			strButton2 = LBL_AFTERREAD_ACTIONONENTRY_BUTTON_ARCHIVE_UNSTAR;
		}
	}
	else {
		strButton1 = LBL_AFTERREAD_ACTIONONENTRY_BUTTON_UNARCHIVE;
		if (isStarred == false) {
			strButton2 = LBL_AFTERREAD_ACTIONONENTRY_BUTTON_UNARCHIVE_STAR;
		}
		else {
			strButton2 = LBL_AFTERREAD_ACTIONONENTRY_BUTTON_UNARCHIVE_UNSTAR;
		}
	}

	int result = DialogSynchro(ICON_QUESTION, LBL_AFTERREAD_ACTIONONENTRY_WHAT_TO_DO_WITH_ENTRY_TITLE, buffer, strButton1, strButton2, LBL_AFTERREAD_ACTIONONENTRY_BUTTON_DONOTHING);

	if (result == 3) {
		// do nothing
		show();
		gui.statusBarText(LBL_STATUSBAR_AFTERREAD_THANKS_READ_ANOTHER);
		return;
	}

	if (result == 1) {
		if (isArchived == false) {
			entry.local_is_archived = true;
		}
		else {
			entry.local_is_archived = false;
		}
	}
	if (result == 2) {
		if (isArchived == false) {
			if (isStarred == false) {
				entry.local_is_archived = true;
				entry.local_is_starred = true;
			}
			else {
				entry.local_is_archived = true;
				entry.local_is_starred = false;
			}
		}
		else {
			if (isStarred == false) {
				entry.local_is_archived = false;
				entry.local_is_starred = true;
			}
			else {
				entry.local_is_archived = false;
				entry.local_is_starred = false;
			}
		}
	}

	// The entry has been updated locally => keep track of it, to known it might have to be synced to server later on
	entry.local_updated_at = time(NULL);

	entryRepository.persist(entry);

	// One entry has changed => we must redraw the list
	show();

	gui.statusBarText(LBL_STATUSBAR_AFTERREAD_CHANGE_SENT_ON_NEXT_SYNC);
}


void Application::foreground()
{
	if (isLastActionRead && lastReadEntryId != 0) {
		gui.statusBarText(LBL_STATUSBAR_AFTERREAD_WHAT_TO_DO_WITH_ENTRY);
		handleActionOnReadEntry(lastReadEntryId);
	}
	isLastActionRead = false;
	lastReadEntryId = 0;
}


void Application::background()
{

}


int Application::countEntriesForCurrentMode()
{
	if (mode == MODE_UNREAD) {
		return entryRepository.countUnread();
	}
	if (mode == MODE_ARCHIVED) {
		return entryRepository.countArchived();
	}
	if (mode == MODE_STARRED) {
		return entryRepository.countStarred();
	}
	return 0;
}


void Application::listEntriesForCurrentMode(std::vector<Entry> &entries)
{
	if (mode == MODE_UNREAD) {
		entryRepository.listUnread(entries, numPerPage, (pageNum-1) * numPerPage);
	}
	else if (mode == MODE_ARCHIVED) {
		entryRepository.listArchived(entries, numPerPage, (pageNum-1) * numPerPage);
	}
	else if (mode == MODE_STARRED) {
		entryRepository.listStarred(entries, numPerPage, (pageNum-1) * numPerPage);
	}
}


// Delete all local data, which means next sync will be "from scratch"
//   -> local database
//   -> local HTML entries
//   -> local EPUB entries
//   -> log file ?
// Note: we do not delete the config file ;-)
void Application::deleteAllLocalData()
{
	DIR *dir;
	struct dirent *entry;

	DEBUG("Dropping database");
	db.drop();
	DEBUG("Running migrations to rebuild the database");
	db.runMigrations();

	// Delete all EPUB entries
	dir = iv_opendir(PLOP_ENTRIES_EPUB_DIRECTORY);
	if (dir) {
		while ((entry = iv_readdir(dir))) {
			if (iv_strcmp(entry->d_name, ".") == 0 || iv_strcmp(entry->d_name, "..") == 0) {
				continue;
			}
			DEBUG("Delete: %s", entry->d_name);
			iv_unlink(entry->d_name);
		}
		iv_closedir(dir);

		iv_rmdir(PLOP_ENTRIES_EPUB_DIRECTORY);
	}

	// Delete all HTML entries
	dir = iv_opendir(PLOP_ENTRIES_CONTENT_DIRECTORY);
	if (dir) {
		while ((entry = iv_readdir(dir))) {
			if (iv_strcmp(entry->d_name, ".") == 0 || iv_strcmp(entry->d_name, "..") == 0) {
				continue;
			}
			DEBUG("Delete: %s", entry->d_name);
			iv_unlink(entry->d_name);
		}
		iv_closedir(dir);

		iv_rmdir(PLOP_ENTRIES_CONTENT_DIRECTORY);
	}

	// Delete all images (entries thumbnails)
	dir = iv_opendir(PLOP_ENTRIES_IMAGES_DIRECTORY);
	if (dir) {
		while ((entry = iv_readdir(dir))) {
			if (iv_strcmp(entry->d_name, ".") == 0 || iv_strcmp(entry->d_name, "..") == 0) {
				continue;
			}
			DEBUG("Delete: %s", entry->d_name);
			iv_unlink(entry->d_name);
		}
		iv_closedir(dir);

		iv_rmdir(PLOP_ENTRIES_IMAGES_DIRECTORY);
	}

	app.show();
}


void Application::restoreModeAndPage()
{
	Internal valPageNum = db.selectInternal("gui.pageNum");
	Internal valMode = db.selectInternal("gui.mode");
	if (!valPageNum.isNull && !valMode.isNull) {
		int _pageNum = strtol(valPageNum.value.c_str(), 0, 10);
		int _mode = strtol(valMode.value.c_str(), 0, 10);
		if (_pageNum != 0 && _mode != 0) {
			setMode(_mode, _pageNum);
		}
	}
}


void Application::saveModeAndPage()
{
	getDb().saveInternal("gui.pageNum", patch::to_string(pageNum));
	getDb().saveInternal("gui.mode", patch::to_string(mode));
}

