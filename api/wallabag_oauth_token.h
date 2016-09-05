#ifndef API_WALLABAG_OAUTH_TOKEN_H_
#define API_WALLABAG_OAUTH_TOKEN_H_

#include <string>

class WallabagOAuthToken
{

public:
	std::string access_token;
	std::string refresh_token;
	int expires_at;

};


#endif /* API_WALLABAG_OAUTH_TOKEN_H_ */
