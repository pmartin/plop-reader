#include "application.h"
#include "log.h"


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
}


void Application::deinit()
{
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

	gui.openProgressBar(ICON_WIFI, "Synchronizing with server", "Starting synchronization", 0, [](int button) {});
	gui.updateProgressBar("Fetching recent entries from server", Gui::SYNC_PROGRESS_PERCENTAGE_ALL_START);

	Internal lastSyncTimestampObj = db.selectInternal("sync.last-sync.timestamp");
	time_t lastSyncTimestamp = lastSyncTimestampObj.isNull ? 0 : atoi(lastSyncTimestampObj.value.c_str());
	DEBUG("Last sync TS = %ld", lastSyncTimestamp);

	wallabag_api.loadRecentArticles(entryRepository, lastSyncTimestamp, [](const char *text, int percent, void *context) {
		app.gui.updateProgressBar(text, percent);
	});

	// Send changes to server, for entries marked as archived/starred recently on the device
	wallabag_api.syncEntriesToServer(entryRepository, [](const char *text, int percent, void *context) {
		app.gui.updateProgressBar(text, percent);
	});

	db.saveInternal("sync.last-sync.timestamp", SSTR(time(NULL)));
	DEBUG("Saving last sync TS = %ld", time(NULL));

	gui.updateProgressBar("All done \\o/", Gui::SYNC_PROGRESS_PERCENTAGE_ALL_DONE);
	gui.closeProgressBar();

	INFO("End of synchronization process");

	show();
}


void Application::setMode(int m)
{
	mode = (entries_mode)m;
	pageNum = 1;

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

void Application::read(Entry &entry)
{
	//char buffer[2048];
	//snprintf(buffer, sizeof(buffer), "This should open entry li=%d / ri=%s\n-> %s", entry.id, entry.remote_id.c_str(), entry.title.c_str());
	//Message(ICON_INFORMATION, "Opening entry... One day!", buffer, 1*1000);

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


void Application::debug(const char *format...)
{
	va_list args;
	va_start(args, format);
	gui.statusBarText(format, args);
	va_end(args);
}


void Application::handleActionOnReadEntry(int entryId)
{
	Entry entry = entryRepository.get(entryId);

	char buffer[2048];
	snprintf(buffer, sizeof(buffer), "What do you want to do with entry #%d?\n%.128s?", entry.id, entry.title.c_str());

	bool isArchived = entry.local_is_archived;
	bool isStarred = entry.local_is_starred;

	const char *strButton1, *strButton2;

	if (isArchived == false) {
		strButton1 = "Archive";
		if (isStarred == false) {
			strButton2 = "Archive + Star";
		}
		else {
			strButton2 = "Archive + Un-star";
		}
	}
	else {
		strButton1 = "Un-archive";
		if (isStarred == false) {
			strButton2 = "Un-archive + Star";
		}
		else {
			strButton2 = "Un-archive + Un-star";
		}
	}

	int result = DialogSynchro(ICON_QUESTION, "What now with this entry?", buffer, strButton1, strButton2, "Do nothing");

	if (result == 3) {
		// do nothing
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
}


void Application::foreground()
{
	if (isLastActionRead && lastReadEntryId != 0) {
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

