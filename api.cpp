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

	char enries_url[2048];

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

		snprintf(enries_url, sizeof(enries_url), "%sapi/entries.json?access_token=%s&sort=%s&order=%s&page=%d&perPage=%d",
				config->url, encoded_access_token, encoded_sort, encoded_order, 1, 15);

		curl_free(encoded_access_token);
		curl_free(encoded_sort);
		curl_free(encoded_order);

		curl_easy_setopt(curl, CURLOPT_URL, enries_url);

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

				database_write_entry(entry);
			}
		}

		free(json_string);

		end:
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
}

