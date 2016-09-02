#include "belladonna.h"

static int json_string_get_token_len = 0;
static char *json_string_get_token = NULL;


static size_t header_callback(char *ptr, size_t size, size_t nitems, void *userdata)
{
	//char buffer[1024];

	int data_size = size * nitems;
	//snprintf(buffer, sizeof(buffer), "  Header : %d bytes : %.128s", data_size, ptr);
	//log_message(buffer);

	return data_size;
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t data_size = size * nmemb;

	json_string_get_token = (char *)realloc(json_string_get_token, json_string_get_token_len + data_size + 1);
	if (json_string_get_token == NULL) {
		// TODO not good ^^
	}

	memcpy(json_string_get_token + json_string_get_token_len, ptr, data_size);
	json_string_get_token_len += data_size;
	json_string_get_token[json_string_get_token_len] = '\0';

	return data_size;
}


void create_token()
{
	CURL *curl;
	CURLcode res;

	char buffer[2048];

	json_string_get_token_len = 0;
	json_string_get_token = (char *)calloc(1, 1);

	char url[2048];
	snprintf(url, sizeof(url), "%soauth/v2/token", config->url);
	//log_message(url);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		char postdata[2048];
		char *encoded_client_id = curl_easy_escape(curl, config->client_id, 0);
		char *encoded_secret_key = curl_easy_escape(curl, config->secret_key, 0);
		char *encoded_login = curl_easy_escape(curl, config->login, 0);
		char *encoded_password = curl_easy_escape(curl, config->password, 0);

		//snprintf(buffer, sizeof(buffer), "username=%s&password=%s", encoded_login, encoded_password);
		//log_message(buffer);

		//snprintf(buffer, sizeof(buffer), "client_id=%s&client_secret=%s", encoded_client_id, encoded_secret_key);
		//log_message(buffer);

		snprintf(postdata, sizeof(postdata), "grant_type=password&client_id=%s&client_secret=%s&username=%s&password=%s",
				encoded_client_id, encoded_secret_key, encoded_login, encoded_password);

		curl_free(encoded_client_id);
		curl_free(encoded_secret_key);
		curl_free(encoded_login);
		curl_free(encoded_password);

		//log_message(postdata);

		//snprintf(buffer, sizeof(buffer), "length post = %d", strlen(postdata));
		//log_message(buffer);


		//char auth[512];
		//snprintf(auth, sizeof(auth), "%s:%s", config->http_login, config->http_password);
		//log_message(auth);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata));

		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
		curl_easy_setopt(curl, CURLOPT_USERNAME, config->http_login);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, config->http_password);

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			snprintf(buffer, sizeof(buffer), "Error %d : %s", res, curl_easy_strerror(res));
			log_message(buffer);

			goto end;
		}
		else {
			log_message("OAuth token data:");

			json_object *token = json_tokener_parse(json_string_get_token);

			const char *access_token = json_object_get_string(json_object_object_get(token, "access_token"));
			int expires_in = json_object_get_int(json_object_object_get(token, "expires_in"));
			const char *refresh_token = json_object_get_string(json_object_object_get(token, "refresh_token"));

			snprintf(buffer, sizeof(buffer), "  > access = %s", access_token);
			log_message(buffer);

			snprintf(buffer, sizeof(buffer), "  > expires in = %d", expires_in);
			log_message(buffer);

			snprintf(buffer, sizeof(buffer), "  > refresh = %s", refresh_token);
			log_message(buffer);

			free(json_string_get_token);

			// TODO we have an oauth token => store it somewhere, and use the API \o/
		}

		end:
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
}

