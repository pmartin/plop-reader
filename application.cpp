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
	gui.show();
}
