#include "belladonna.h"


static int json_string_len = 0;
static char *json_string = NULL;

static size_t callback_entries(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t data_size = size * nmemb;

	json_string = (char *)realloc(json_string, json_string_len + data_size + 1);
	if (json_string == NULL) {
		// TODO not good ^^
	}

	memcpy(json_string + json_string_len, ptr, data_size);
	json_string_len += data_size;
	json_string[json_string_len] = '\0';

	return data_size;
}


void load_recent_articles(b_config_s *conf, b_oauth_token_s *token)
{
	char buffer[2048];
	log_message("Chargement des articles...");

	char url[2048];

	CURL *curl;
	CURLcode res;

	json_string_len = 0;
	json_string = (char *)calloc(1, 1);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		char *encoded_access_token = curl_easy_escape(curl, token->access_token, 0);
		char *encoded_sort = curl_easy_escape(curl, "updated", 0);
		char *encoded_order = curl_easy_escape(curl, "desc", 0);

		snprintf(url, sizeof(url), "%sapi/entries.json?access_token=%s&archive=%d&sort=%s&order=%s&page=%d&perPage=%d",
				config->url, encoded_access_token, 0, encoded_sort, encoded_order, 1, 3);

		curl_free(encoded_access_token);
		curl_free(encoded_sort);
		curl_free(encoded_order);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
		curl_easy_setopt(curl, CURLOPT_USERNAME, config->http_login);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, config->http_password);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_entries);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			snprintf(buffer, sizeof(buffer), "Error %d : %s", res, curl_easy_strerror(res));
			log_message(buffer);

			goto end;
		}
		else {

			log_message(json_string);

			// TODO parcourir les articles renvoyés et les stocker en DB


		}


		free(json_string);

		end:
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
}

