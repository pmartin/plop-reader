#include "application.h"


void Application::init()
{
	// TODO instead of dropping + creating the DB, we should detect if its structure is up-to-date
	// and run migrations only if it is not ;-)
	//db.drop();

	db.runMigrations();


	WallabagConfigLoader configLoader;
	WallabagConfig config = configLoader.load();
	wallabag_api.setConfig(config);

	gui.init();
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
		// Failed to connect
		return false;
	}

	// Just to be sure: check if we are, now, connected
	netinfo = NetInfo();
	if (netinfo->connected) {
		return true;
	}

	// Connection failed, I don't know why
	return false;
}


void Application::loadRecentArticles()
{
	if (!connectToNetwork()) {
		// No network connection... not going to be able to synchronize!
		Message(ICON_ERROR, "Synchronization failed!", "No active network connection, no way to synchronize!", 3*1000);
	}
	else {
		// So the debug log is easier to read...
		//ClearScreen();

		wallabag_api.loadRecentArticles(entryRepository);

		// Just so have a bit of time to read the debug log...
		//sleep(5);
	}

	show();
}


void Application::setMode(int m)
{
	mode = (entries_mode)m;
	pageNum = 0;

	gui.setMode(mode);

	show();
}


void Application::show()
{
	// TODO la pagination ne doit pas se faire sur le nombre total d'entrées,
	// mais sur le nombre d'entrées du type en cours d'affichage

	int countEntries;

	//ClearScreen();

	std::vector<Entry> entries;
	if (mode == MODE_UNREAD) {
		countEntries = entryRepository.countUnread();
		entryRepository.listUnread(entries, numPerPage, pageNum * numPerPage);
	}
	else if (mode == MODE_ARCHIVED) {
		countEntries = entryRepository.countArchived();
		entryRepository.listArchived(entries, numPerPage, pageNum * numPerPage);
	}
	else if (mode == MODE_STARRED) {
		countEntries = entryRepository.countStarred();
		entryRepository.listStarred(entries, numPerPage, pageNum * numPerPage);
	}

	//sleep(5);

	int numberOfPages = countEntries / numPerPage;
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
	int countEntries;

	if (mode == MODE_UNREAD) {
		countEntries = entryRepository.countUnread();
	}
	else if (mode == MODE_ARCHIVED) {
		countEntries = entryRepository.countArchived();
	}
	else if (mode == MODE_STARRED) {
		countEntries = entryRepository.countStarred();
	}

	int numberOfPages = countEntries / numPerPage;

	if (key == KEY_PREV && pageNum > 0) {
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
		snprintf(filepath, sizeof(filepath), "/mnt/ext1/system/tmp/belladonna/entry-%d.html", entry.id);
		entry.local_content_file_html = filepath;

		//Message(ICON_INFORMATION, "Opening entry... One day!", filepath, 1*1000);

		// TODO no hard-coded path, and create this directory somewhere else and all...
		iv_mkdir("/mnt/ext1/system/tmp/belladonna/", 0755);

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
		const char *parameters = "r";
		int flags = 0;

		//Message(ICON_INFORMATION, "Here we go!", entry.local_content_file_html.c_str(), 1*1000);

		OpenBook(entry.local_content_file_html.c_str(), parameters, flags);

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
	int result = DialogSynchro(ICON_QUESTION, "What now with this entry?", buffer, "Archive", "Archive + Star", "Do nothing");

	if (result == 3) {
		// do nothing
		return;
	}

	if (result == 2) {
		// archive + star
		entry.local_is_archived = true;
		entry.local_is_starred = true;
	}
	else if (result == 1) {
		// archive
		entry.local_is_archived = true;
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

