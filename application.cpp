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
}


void Application::loadRecentArticles()
{
	// TODO this will not quite work if no oauth token has been created before
	// => we must create an oauth token on-the-fly
	// => or store one in DB or somewhere
	wallabag_api.loadRecentArticles(entryRepository);
}
