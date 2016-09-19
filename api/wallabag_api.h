#ifndef API_WALLABAG_API_H_
#define API_WALLABAG_API_H_

#include "curl/curl.h"

#include "wallabag_config.h"
#include "wallabag_entities_factory.h"
#include "wallabag_oauth_token.h"

#include "../repositories/entry_repository.h"

#include "../log.h"
#include "../gui/gui.h"


class WallabagApi
{

public:
	void setConfig(WallabagConfig conf);

	void createOAuthToken();
	void refreshOAuthToken();

	void loadRecentArticles(EntryRepository repository, time_t lastSyncTimestamp, gui_update_progressbar progressbarUpdater);

	void syncEntriesToServer(EntryRepository repository, gui_update_progressbar progressbarUpdater);

private:
	void syncOneEntryToServer(EntryRepository repository, Entry &entry);
	static size_t _curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

	WallabagConfig config;

	WallabagOAuthToken oauthToken;

	WallabagEntitiesFactory entitiesFactory;

	// For loadRecentArticles
	int json_string_len;
	char *json_string;
};


#endif /* API_WALLABAG_API_H_ */
