#include "belladonna.h"

b_config_s *config;


void load_config()
{
	//char buffer[2048];
	const char *filepath = "/mnt/ext1/system/tmp/belladonna.json";

	config = (b_config_s *)malloc(sizeof(b_config_s));

	json_object *obj = json_object_from_file(filepath);

	const char *url = json_object_get_string(json_object_object_get(obj, "url"));
	config->url = (char *)malloc(strlen(url) + 1);
	strcpy(config->url, url);

	const char *http_login = json_object_get_string(json_object_object_get(obj, "http_login"));
	if (http_login) {
		config->http_login = (char *)malloc(strlen(http_login) + 1);
		strcpy(config->http_login, http_login);
	}
	else {
		config->http_login = NULL;
	}

	const char *http_password = json_object_get_string(json_object_object_get(obj, "http_password"));
	if (http_login) {
		config->http_password = (char *)malloc(strlen(http_password) + 1);
		strcpy(config->http_password, http_password);
	}
	else {
		config->http_password = NULL;
	}

	const char *client_id = json_object_get_string(json_object_object_get(obj, "client_id"));
	config->client_id = (char *)malloc(strlen(client_id) + 1);
	strcpy(config->client_id, client_id);

	const char *secret_key = json_object_get_string(json_object_object_get(obj, "secret_key"));
	config->secret_key = (char *)malloc(strlen(secret_key) + 1);
	strcpy(config->secret_key, secret_key);

	const char *login = json_object_get_string(json_object_object_get(obj, "login"));
	config->login = (char *)malloc(strlen(login) + 1);
	strcpy(config->login, login);

	const char *password = json_object_get_string(json_object_object_get(obj, "password"));
	config->password = (char *)malloc(strlen(password) + 1);
	strcpy(config->password, password);


	//log_message("JSON config:");
	//snprintf(buffer, sizeof(buffer), "URL = %s", config->url);
	//log_message(buffer);
}


void unload_config()
{
	free(config->url);
	if (config->http_login) {
		free(config->http_login);
	}
	if (config->http_password) {
		free(config->http_password);
	}
	free(config->client_id);
	free(config->secret_key);
	free(config->login);
	free(config->password);
	free(config);
}


