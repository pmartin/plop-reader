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
	Application() : entryRepository(db), gui(*this) {
		db.open();
		pageNum = 0;
		numPerPage = 8;
		isLastActionRead = false;
		lastReadEntryId = 0;
	}

	void init();

	void loadRecentArticles();

	Database &getDb() {
		return db;
	}


	void show();

	void touchStartEvent(int x, int y);
	void touchEndEvent(int x, int y);

	void keypressEvent(int key);

	void read(Entry &entry);

	void debug(const char *format...);

	void foreground();
	void background();

	void handleActionOnReadEntry(int entryId);


	// TODO remove this method
	EntryRepository getEntryRepository() {
		return entryRepository;
	}

private:
	Database db;
	WallabagApi wallabag_api;
	EntryRepository entryRepository;
	Gui gui;

	int pageNum;
	int numPerPage;

	bool isLastActionRead;
	int lastReadEntryId;

};


#endif /* APPLICATION_H_ */
