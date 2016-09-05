#include "wallabag_api.h"


void WallabagApi::setConfig(WallabagConfig conf)
{
	this->config = conf;
}


size_t WallabagApi::_createAOauthTokenHeaderCallback(char *ptr, size_t size, size_t nitems, void *userdata)
{
	//WallabagApi *that = (WallabagApi *)userdata;

	//char buffer[1024];

	int data_size = size * nitems;
	//snprintf(buffer, sizeof(buffer), "  Header : %d bytes : %.128s", data_size, ptr);
	//log_message(buffer);

	return data_size;
}


size_t WallabagApi::_createAOauthTokenWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	WallabagApi *that = (WallabagApi *)userdata;

	size_t data_size = size * nmemb;

	that->json_string_get_token = (char *)realloc(that->json_string_get_token, that->json_string_get_token_len + data_size + 1);
	if (that->json_string_get_token == NULL) {
		// TODO not good ^^
	}

	memcpy(that->json_string_get_token + that->json_string_get_token_len, ptr, data_size);
	that->json_string_get_token_len += data_size;
	that->json_string_get_token[that->json_string_get_token_len] = '\0';

	return data_size;
}


void WallabagApi::createOAuthToken()
{

	CURL *curl;
	CURLcode res;

	char buffer[2048];
	log_message("Requesting OAuth token...");

	this->json_string_get_token_len = 0;
	this->json_string_get_token = (char *)calloc(1, 1);

	char url[2048];
	snprintf(url, sizeof(url), "%soauth/v2/token", config.url.c_str());

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		char postdata[2048];
		char *encoded_client_id = curl_easy_escape(curl, this->config.client_id.c_str(), 0);
		char *encoded_secret_key = curl_easy_escape(curl, this->config.secret_key.c_str(), 0);
		char *encoded_login = curl_easy_escape(curl, this->config.login.c_str(), 0);
		char *encoded_password = curl_easy_escape(curl, this->config.password.c_str(), 0);

		snprintf(postdata, sizeof(postdata), "grant_type=password&client_id=%s&client_secret=%s&username=%s&password=%s",
				encoded_client_id, encoded_secret_key, encoded_login, encoded_password);

		curl_free(encoded_client_id);
		curl_free(encoded_secret_key);
		curl_free(encoded_login);
		curl_free(encoded_password);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata));

		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
		curl_easy_setopt(curl, CURLOPT_USERNAME, this->config.http_login.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, this->config.http_password.c_str());

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, WallabagApi::_createAOauthTokenHeaderCallback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WallabagApi::_createAOauthTokenWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			snprintf(buffer, sizeof(buffer), "Error %d : %s", res, curl_easy_strerror(res));
			log_message(buffer);

			goto end;
		}
		else {
			json_object *json_token = json_tokener_parse(json_string_get_token);

			const char *access_token = json_object_get_string(json_object_object_get(json_token, "access_token"));
			int expires_in = json_object_get_int(json_object_object_get(json_token, "expires_in"));
			const char *refresh_token = json_object_get_string(json_object_object_get(json_token, "refresh_token"));

			this->oauthToken.access_token = access_token;
			this->oauthToken.refresh_token = refresh_token;
			this->oauthToken.expires_at = time(NULL) + expires_in;
		}

		end:
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	free(this->json_string_get_token);
}


size_t WallabagApi::_loadRecentArticlesWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	WallabagApi *that = (WallabagApi *)userdata;

	size_t data_size = size * nmemb;

	that->json_string = (char *)realloc(that->json_string, that->json_string_len + data_size + 1);
	if (that->json_string == NULL) {
		// TODO not good ^^
	}

	memcpy(that->json_string + that->json_string_len, ptr, data_size);
	that->json_string_len += data_size;
	that->json_string[that->json_string_len] = '\0';

	return data_size;
}


void WallabagApi::loadRecentArticles(EntryRepository repository)
{
	char buffer[2048];
	log_message("Chargement des articles...");

	char enries_url[2048];

	CURL *curl;
	CURLcode res;

	this->json_string_len = 0;
	this->json_string = (char *)calloc(1, 1);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		char *encoded_access_token = curl_easy_escape(curl, this->oauthToken.access_token.c_str(), 0);
		char *encoded_sort = curl_easy_escape(curl, "updated", 0);
		char *encoded_order = curl_easy_escape(curl, "desc", 0);

		snprintf(enries_url, sizeof(enries_url), "%sapi/entries.json?access_token=%s&sort=%s&order=%s&page=%d&perPage=%d",
				config.url.c_str(), encoded_access_token, encoded_sort, encoded_order, 1, 15);

		curl_free(encoded_access_token);
		curl_free(encoded_sort);
		curl_free(encoded_order);

		curl_easy_setopt(curl, CURLOPT_URL, enries_url);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
		curl_easy_setopt(curl, CURLOPT_USERNAME, this->config.http_login.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, this->config.http_password.c_str());

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WallabagApi::_loadRecentArticlesWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			snprintf(buffer, sizeof(buffer), "Error %d : %s", res, curl_easy_strerror(res));
			log_message(buffer);

			goto end;
		}
		else {
			json_object *obj = json_tokener_parse(json_string);

			snprintf(buffer, sizeof(buffer), "Page %d / %d - posts %d / %d",
				json_object_get_int(json_object_object_get(obj, "page")),
				json_object_get_int(json_object_object_get(obj, "pages")),
				json_object_get_int(json_object_object_get(obj, "limit")),
				json_object_get_int(json_object_object_get(obj, "total"))
			);
			log_message(buffer);

			array_list *items = json_object_get_array(json_object_object_get(json_object_object_get(obj, "_embedded"), "items"));
			for (int i=0 ; i<items->length ; i++) {
				json_object *item = (json_object *)array_list_get_idx(items, i);

				int id = json_object_get_int(json_object_object_get(item, "id"));
				int is_archived = json_object_get_int(json_object_object_get(item, "is_archived"));
				int is_starred = json_object_get_int(json_object_object_get(item, "is_starred"));
				const char *title = json_object_get_string(json_object_object_get(item, "title"));
				const char *url = json_object_get_string(json_object_object_get(item, "url"));
				const char *content = json_object_get_string(json_object_object_get(item, "content"));
				const char *created_at = json_object_get_string(json_object_object_get(item, "created_at"));
				const char *updated_at = json_object_get_string(json_object_object_get(item, "updated_at"));
				int reading_time = json_object_get_int(json_object_object_get(item, "reading_time")) * 60;
				const char *preview_picture = json_object_get_string(json_object_object_get(item, "preview_picture"));

				//snprintf(buffer, sizeof(buffer), "%d - (%c%c) %s (%s)", id, (is_archived ? 'a' : '.'), (is_starred ? '*' : '.'), title, url);
				//log_message(buffer);

				Entry entry;
				char tmp[128];
				snprintf(tmp, sizeof(tmp), "%d", id);
				entry.remote_id = tmp;
				entry.remote_is_archived = is_archived;
				entry.remote_is_starred = is_starred;
				if (title != NULL) {
					entry.title = title;
				}
				if (url != NULL) {
					entry.url = url;
				}
				if (content != NULL) {
					entry.content = content;
				}
				entry.remote_created_at = created_at;
				entry.remote_updated_at = updated_at;
				entry.reading_time = reading_time;
				if (preview_picture != NULL) {
					entry.preview_picture_url = preview_picture;
				}

				repository.persist(entry);
			}
		}

		end:
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	free(this->json_string);
}
