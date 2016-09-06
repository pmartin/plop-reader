#ifndef APPLICATION_H_
#define APPLICATION_H_


#include "api/wallabag_api.h"
#include "api/wallabag_config_loader.h"
#include "api/wallabag_config.h"

#include "database/database.h"

#include "gui/gui.h"

#include "repositories/entry_repository.h"


class Application
{
public:
	Application() : entryRepository(db) {
		db.open();
	}

	void init();

	void loadRecentArticles();

	Database &getDb() {
		return db;
	}


	void show();


	// TODO remove this method
	EntryRepository getEntryRepository() {
		return entryRepository;
	}

private:
	Database db;
	WallabagApi wallabag_api;
	EntryRepository entryRepository;
	Gui gui;

};


#endif /* APPLICATION_H_ */
