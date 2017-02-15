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

	initAssets();

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

	setMode(MODE_UNREAD, 1);
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


void Application::initAssets()
{
	const static unsigned char sync_png[] = {
		0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
		0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30,
		0x08, 0x04, 0x00, 0x00, 0x00, 0xfd, 0x0b, 0x31, 0x0c, 0x00, 0x00, 0x01,
		0x9a, 0x49, 0x44, 0x41, 0x54, 0x78, 0x01, 0xe5, 0x94, 0x41, 0xab, 0x12,
		0x51, 0x14, 0xc7, 0x7f, 0x4d, 0x91, 0x86, 0x23, 0x2d, 0x1f, 0x98, 0xd0,
		0x62, 0xa4, 0xad, 0x9b, 0x08, 0x6a, 0x19, 0xe2, 0x47, 0x10, 0xe9, 0xd3,
		0x08, 0x81, 0x3c, 0xad, 0x75, 0x08, 0x63, 0xb4, 0xd1, 0xcf, 0x22, 0xb8,
		0x08, 0x6c, 0xd9, 0x46, 0x5d, 0x8a, 0xd0, 0xa6, 0xf7, 0xa8, 0x67, 0xc5,
		0x69, 0x73, 0xb8, 0x08, 0xd3, 0x70, 0xef, 0x8d, 0x39, 0x2b, 0x7f, 0x77,
		0xfb, 0x3f, 0xff, 0x1f, 0xc3, 0x9c, 0x7b, 0xb9, 0x38, 0x46, 0xd6, 0xf5,
		0x62, 0x5d, 0x2f, 0xd6, 0xf5, 0x62, 0x5d, 0x2f, 0xd6, 0xf5, 0x62, 0x5d,
		0x2f, 0x9c, 0x38, 0xb2, 0x26, 0x67, 0x48, 0xd3, 0xa2, 0xfe, 0xfc, 0xdc,
		0x32, 0xa7, 0x63, 0x28, 0xd0, 0xef, 0x99, 0x52, 0xb7, 0x55, 0x08, 0x1b,
		0xb2, 0x2a, 0x15, 0x0f, 0x69, 0xd1, 0x63, 0xcc, 0x16, 0x71, 0xe7, 0x40,
		0xb7, 0xfa, 0x35, 0x4d, 0x18, 0xb0, 0x3b, 0x53, 0x64, 0x16, 0x17, 0xad,
		0xc1, 0xc2, 0x29, 0x36, 0xd4, 0x09, 0xe6, 0x55, 0xc4, 0x53, 0xf1, 0xde,
		0x29, 0x26, 0x04, 0xf2, 0x8c, 0x5f, 0x8c, 0x22, 0x1e, 0xbb, 0xa5, 0xdb,
		0xa8, 0x8c, 0x20, 0x3e, 0x21, 0x48, 0xa9, 0xa2, 0x48, 0xca, 0x5e, 0x15,
		0x39, 0x01, 0x3c, 0xe6, 0x87, 0xc6, 0x9f, 0x13, 0xca, 0x50, 0x27, 0x6e,
		0x48, 0xf1, 0xf2, 0x46, 0xc3, 0x5f, 0xb9, 0x47, 0x28, 0x89, 0xdb, 0xa7,
		0x01, 0x5e, 0x3e, 0x6a, 0xf4, 0x2d, 0x31, 0x5c, 0xeb, 0xd4, 0x0c, 0x2f,
		0x9f, 0x35, 0xfa, 0x9a, 0x18, 0xfa, 0x3a, 0xb5, 0xc2, 0xcb, 0x37, 0x8d,
		0x5e, 0x11, 0x43, 0xdb, 0x5d, 0x38, 0x2f, 0x7f, 0x34, 0x7a, 0x9f, 0x18,
		0x6a, 0x3a, 0x75, 0x87, 0x97, 0x93, 0x7b, 0x73, 0x62, 0x78, 0xa0, 0x53,
		0xbf, 0xf1, 0x72, 0xd4, 0x68, 0x8b, 0x18, 0x9e, 0xe8, 0xd4, 0x11, 0x2f,
		0x6b, 0x8d, 0xf6, 0xfe, 0xeb, 0x27, 0xaf, 0xf1, 0x92, 0x6b, 0x74, 0x4c,
		0x0c, 0x13, 0xb7, 0xa6, 0xc1, 0xb7, 0x72, 0x4b, 0x42, 0x28, 0x09, 0xfb,
		0xf0, 0x8b, 0xd6, 0xe4, 0x36, 0x3c, 0xac, 0xbc, 0xd4, 0x89, 0xef, 0x34,
		0x08, 0x60, 0xae, 0xf1, 0xdd, 0x3f, 0xe3, 0xe5, 0xef, 0xac, 0xf0, 0x81,
		0x20, 0x3a, 0x6e, 0x55, 0x17, 0x14, 0x91, 0x52, 0xc5, 0x4f, 0x9e, 0x12,
		0xc8, 0x14, 0xd1, 0xf3, 0xae, 0x28, 0x28, 0x55, 0xbc, 0x20, 0x98, 0x47,
		0x7c, 0x71, 0x8a, 0x25, 0x69, 0x41, 0x50, 0x54, 0x44, 0x93, 0x71, 0x70,
		0x8a, 0x3d, 0x43, 0x12, 0x14, 0xa4, 0x2a, 0x45, 0xf7, 0x4c, 0x21, 0xec,
		0xb8, 0xa6, 0x4f, 0x9b, 0x1a, 0x52, 0x9d, 0x22, 0x63, 0x83, 0x14, 0x4f,
		0x95, 0x8a, 0x3a, 0x13, 0x4e, 0xb6, 0x0a, 0xc8, 0xc8, 0xb9, 0x31, 0x13,
		0x28, 0x29, 0x03, 0x66, 0xac, 0x38, 0x70, 0x17, 0x5f, 0x1f, 0x8b, 0x71,
		0x3d, 0xd6, 0xf5, 0x58, 0xd7, 0x63, 0x5d, 0x8f, 0x75, 0x3d, 0x8c, 0xb8,
		0x30, 0xfe, 0x02, 0xa0, 0xaa, 0x69, 0x89, 0x87, 0xe0, 0xb7, 0x19, 0x00,
		0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
	};
	const static unsigned int sync_png_len = 467;

	iv_mkdir(PLOP_ENTRIES_ASSETS_DIRECTORY, 0777);
	FILE *fp = iv_fopen(PLOP_ICON_SYNC_PATH, "w");
	for (unsigned int i=0 ; i<sync_png_len ; i++) {
		iv_fwrite(sync_png + i, 1, 1, fp);
	}
	iv_fclose(fp);
}

