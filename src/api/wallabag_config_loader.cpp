#include "wallabag_config_loader.h"



char *WallabagConfigLoader::readConfigFile(const char *path)
{
	FILE *f = iv_fopen(path, "rb");
	if (f == NULL) {
		ERROR("Could not open file %s", path);
		throw ConfigFileNotFoundException(std::string("The ") + std::string(CONFIG_FILE) + std::string(" configuration file  has not been found."));
	}

	// TODO error handling if malloc or realloc fails

	char *json_string = (char *)malloc(0);
	int json_string_len = 0;

	char buffer[2048];
	int count_read;
	while ((count_read = iv_fread(buffer, 1, sizeof(buffer)-1, f)) > 0 ) {
		json_string = (char *)realloc(json_string, json_string_len + count_read);
		strncpy(json_string + json_string_len, buffer, count_read);
		json_string_len += count_read;
	}

	json_string = (char *)realloc(json_string, json_string_len + 1);
	json_string[json_string_len] = '\0';

	iv_fclose(f);

	return json_string;
}


WallabagConfig WallabagConfigLoader::load(void)
{
	WallabagConfig config;

	char *json_string = readConfigFile(CONFIG_FILE);

	json_tokener_error error;
	json_object *obj = json_tokener_parse_verbose(json_string, &error);

	free(json_string);

	if (obj == NULL) {
		ERROR("Failed reading JSON from config file: %s", json_tokener_error_desc(error));
		throw ConfigFileInvalidException(std::string("Error while reading JSON config file: " + std::string(json_tokener_error_desc(error))));
	}

	const char *url = json_object_get_string(json_object_object_get(obj, "url"));
	if (url && strlen(url) > 0) {
		config.url = url;
		if (config.url.at(config.url.length() - 1) != '/') {
			DEBUG("URL in config doesn't end with a '/' => adding one");
			config.url.append("/");
		}
	}
	else {
		ERROR("Entry 'url' not found in %s", CONFIG_FILE);
	}

	const char *http_login = json_object_get_string(json_object_object_get(obj, "http_login"));
	if (http_login && strlen(http_login) > 0) {
		config.http_login = http_login;
	}
	else {
		config.http_login = "";
	}

	const char *http_password = json_object_get_string(json_object_object_get(obj, "http_password"));
	if (http_password && strlen(http_password) > 0) {
		config.http_password = http_password;
	}
	else {
		config.http_password = "";
	}

	const char *client_id = json_object_get_string(json_object_object_get(obj, "client_id"));
	if (client_id && strlen(client_id) > 0) {
		config.client_id = client_id;
	}
	else {
		ERROR("Entry 'client_id' not found in %s", CONFIG_FILE);
	}

	const char *secret_key = json_object_get_string(json_object_object_get(obj, "secret_key"));
	if (secret_key && strlen(secret_key) > 0) {
		config.secret_key = secret_key;
	}
	else {
		ERROR("Entry 'secret_key' not found in %s", CONFIG_FILE);
	}

	const char *login = json_object_get_string(json_object_object_get(obj, "login"));
	if (login && strlen(login) > 0) {
		config.login = login;
	}
	else {
		ERROR("Entry 'login' not found in %s", CONFIG_FILE);
	}

	const char *password = json_object_get_string(json_object_object_get(obj, "password"));
	if (password && strlen(password) > 0) {
		config.password = password;
	}
	else {
		ERROR("Entry 'password' not found in %s", CONFIG_FILE);
	}

	config.force_download_epub = json_object_get_boolean(json_object_object_get(obj, "force_download_epub"));

	if (
		(url == NULL || strlen(url) == 0)
		|| (client_id == NULL || strlen(client_id) == 0)
		|| (secret_key == NULL || strlen(secret_key) == 0)
		|| (login == NULL || strlen(login) == 0 )
		|| (password == NULL || strlen(password) == 0)
		) {
		throw ConfigFileMissingItemException("Some items are missing in your JSON config file. Check for 'url', 'client_id', 'secret_key', 'login' and 'password': they must be present and not empty.");
	}

	return config;
}
