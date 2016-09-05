#ifndef API_WALLABAG_API_H_
#define API_WALLABAG_API_H_

#include "../belladonna.h"

#include "wallabag_config.h"
#include "wallabag_oauth_token.h"


class WallabagApi
{

public:
	void setConfig(WallabagConfig conf);

	void createOAuthToken();

	void loadRecentArticles();


	//~WallabagApi();


	static size_t _createAOauthTokenHeaderCallback(char *ptr, size_t size, size_t nitems, void *userdata);
	static size_t _createAOauthTokenWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

	static size_t _loadRecentArticlesWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

private:
	WallabagConfig config;

	WallabagOAuthToken oauthToken;

	// For createOAuthToken
	int json_string_get_token_len;
	char *json_string_get_token;

	// For loadRecentArticles
	int json_string_len;
	char *json_string;
};


#endif /* API_WALLABAG_API_H_ */
