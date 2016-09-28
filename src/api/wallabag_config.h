#ifndef API_WALLABAG_CONFIG_H_
#define API_WALLABAG_CONFIG_H_

#include <string>


class WallabagConfig
{
public:
	std::string url;
	std::string http_login, http_password;
	std::string client_id;
	std::string secret_key;
	std::string login, password;
};


#endif /* API_WALLABAG_CONFIG_H_ */
