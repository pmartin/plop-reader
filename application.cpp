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


void Application::loadRecentArticles()
{
	Message(ICON_INFORMATION, "Working...", "Downloading most recent entries from API...", 3*1000);

	wallabag_api.loadRecentArticles(entryRepository);
	show();
}


void Application::show()
{
	int countAllEntries = entryRepository.countAllEntries();
	int numberOfPages = countAllEntries / numPerPage;

	std::vector<Entry> entries;
	entryRepository.list(entries, numPerPage, pageNum * numPerPage);

	gui.show(pageNum, numberOfPages, countAllEntries, entries);
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
	//gui.keypressEvent(key);

	int countAllEntries = entryRepository.countAllEntries();
	int numberOfPages = countAllEntries / numPerPage;

	if (key == KEY_PREV && pageNum > 0) {
		pageNum -= 1;
	}

	if (key == KEY_NEXT && pageNum < numberOfPages) {
		pageNum += 1;
	}

	std::vector<Entry> entries;
	entryRepository.list(entries, numPerPage, pageNum * numPerPage);

	gui.show(pageNum, numberOfPages, countAllEntries, entries);
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

