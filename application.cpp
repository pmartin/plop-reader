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
	wallabag_api.createOAuthToken();
	wallabag_api.loadRecentArticles(entryRepository);
}


void Application::show()
{
	int countAllEntries = entryRepository.countAllEntries();
	std::vector<Entry> entries = entryRepository.list();

	gui.show(countAllEntries, entries);
}


void Application::debug(const char *format...)
{
	va_list args;
	va_start(args, format);
	gui.statusBarText(format, args);
	va_end(args);
}
