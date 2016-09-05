#include "wallabag_config_loader.h"


WallabagConfig WallabagConfigLoader::load(void)
{
	WallabagConfig config;

	//char buffer[2048];
	const char *filepath = "/mnt/ext1/system/tmp/belladonna.json";

	json_object *obj = json_object_from_file(filepath);

	const char *url = json_object_get_string(json_object_object_get(obj, "url"));
	config.url = url;

	const char *http_login = json_object_get_string(json_object_object_get(obj, "http_login"));
	if (http_login) {
		config.http_login = http_login;
	}
	else {
		config.http_login = "";
	}

	const char *http_password = json_object_get_string(json_object_object_get(obj, "http_password"));
	if (http_password) {
		config.http_password = http_password;
	}
	else {
		config.http_password = "";
	}

	const char *client_id = json_object_get_string(json_object_object_get(obj, "client_id"));
	config.client_id = client_id;

	const char *secret_key = json_object_get_string(json_object_object_get(obj, "secret_key"));
	config.secret_key = secret_key;

	const char *login = json_object_get_string(json_object_object_get(obj, "login"));
	config.login = login;

	const char *password = json_object_get_string(json_object_object_get(obj, "password"));
	config.password = password;

	return config;
}
