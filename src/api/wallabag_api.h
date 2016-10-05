#ifndef API_WALLABAG_API_H_
#define API_WALLABAG_API_H_

#include <sstream>
#include <string>
#include <functional>
#include "curl/curl.h"

#include "wallabag_config.h"
#include "wallabag_entities_factory.h"
#include "wallabag_oauth_token.h"

#include "../repositories/entry_repository.h"

#include "../log.h"
#include "../exceptions.h"
#include "../gui/gui.h"


class WallabagApi
{

public:
	void setConfig(WallabagConfig conf);

	void createOAuthToken(gui_update_progressbar progressbarUpdater);
	void refreshOAuthToken(gui_update_progressbar progressbarUpdater);

	void loadRecentArticles(EntryRepository repository, time_t lastSyncTimestamp, gui_update_progressbar progressbarUpdater);

	void syncEntriesToServer(EntryRepository repository, gui_update_progressbar progressbarUpdater);

	void downloadEpub(EntryRepository &repository, Entry &entry, gui_update_progressbar progressbarUpdater, int percent);

private:
	void syncOneEntryToServer(EntryRepository repository, Entry &entry);
	static size_t _curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

	CURLcode doHttpRequest(
		std::function<char * (CURL *curl)> getUrl,
		std::function<char * (CURL *curl)> getMethod,
		std::function<char * (CURL *curl)> getData,
		std::function<void (void)> beforeRequest,
		std::function<void (void)> afterRequest,
		std::function<void (CURLcode res, char *json_string)> onSuccess,
		std::function<void (CURLcode res, long response_code, CURL *curl)> onFailure,
		FILE *destinationFile = NULL
	);

	WallabagConfig config;

	WallabagOAuthToken oauthToken;

	WallabagEntitiesFactory entitiesFactory;

	// For loadRecentArticles
	int json_string_len;
	char *json_string;
};


#endif /* API_WALLABAG_API_H_ */
