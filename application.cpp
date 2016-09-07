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
	int page = 0;
	int perPage = 8;

	int countAllEntries = entryRepository.countAllEntries();

	std::vector<Entry> entries;
	entryRepository.list(entries, perPage, page * perPage);

	gui.show(countAllEntries, entries);
}


void Application::touchStartEvent(int x, int y)
{
	gui.touchStartEvent(x, y);
}


void Application::touchEndEvent(int x, int y)
{
	gui.touchEndEvent(x, y);
}


void Application::keypressEvent(int key)
{
	//gui.keypressEvent(key);

	int page;
	int perPage = 8;

	if (key == KEY_PREV) {
		page = 0;
	}
	else if (key == KEY_NEXT) {
		page = 1;
	}

	int countAllEntries = entryRepository.countAllEntries();

	std::vector<Entry> entries;
	entryRepository.list(entries, perPage, page * perPage);

	gui.show(countAllEntries, entries);
}


void Application::debug(const char *format...)
{
	va_list args;
	va_start(args, format);
	gui.statusBarText(format, args);
	va_end(args);
}
