#include "wallabag_api.h"


void WallabagApi::setConfig(WallabagConfig conf)
{
	this->config = conf;
}


size_t WallabagApi::_createAOauthTokenWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	WallabagApi *that = (WallabagApi *)userdata;

	size_t data_size = size * nmemb;

	that->json_string_get_token = (char *)realloc(that->json_string_get_token, that->json_string_get_token_len + data_size + 1);
	if (that->json_string_get_token == NULL) {
		// TODO error-handling
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

	//char buffer[2048];
	//log_message("Requesting OAuth token...");

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

		if (!this->config.http_login.empty() && !this->config.http_password.empty()) {
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
			curl_easy_setopt(curl, CURLOPT_USERNAME, this->config.http_login.c_str());
			curl_easy_setopt(curl, CURLOPT_PASSWORD, this->config.http_password.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WallabagApi::_createAOauthTokenWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			//snprintf(buffer, sizeof(buffer), "Error %d : %s", res, curl_easy_strerror(res));
			//log_message(buffer);

			// TODO error-handling

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


void WallabagApi::refreshOAuthToken()
{
	CURL *curl;
	CURLcode res;


	if (this->oauthToken.access_token.empty()) {
		// We do not have an oauth token yet => request one!
		createOAuthToken();
		return;
	}

	if (this->oauthToken.expires_at > time(NULL) - 60) {
		// The OAuth token expires in more than 1 minute => no need to refresh it now
		return;
	}

	//char buffer[2048];
	//log_message("Refreshing OAuth token...");

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
		char *encoded_refresh_token = curl_easy_escape(curl, this->oauthToken.refresh_token.c_str(), 0);

		snprintf(postdata, sizeof(postdata), "grant_type=refresh_token&client_id=%s&client_secret=%s&refresh_token=%s",
				encoded_client_id, encoded_secret_key, encoded_refresh_token);

		curl_free(encoded_client_id);
		curl_free(encoded_secret_key);
		curl_free(encoded_refresh_token);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata));

		if (!this->config.http_login.empty() && !this->config.http_password.empty()) {
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
			curl_easy_setopt(curl, CURLOPT_USERNAME, this->config.http_login.c_str());
			curl_easy_setopt(curl, CURLOPT_PASSWORD, this->config.http_password.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WallabagApi::_createAOauthTokenWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			//snprintf(buffer, sizeof(buffer), "Error %d : %s", res, curl_easy_strerror(res));
			//log_message(buffer);

			// TODO error-handling

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


// TODO rename this method: it's used as a callback for other requests too
size_t WallabagApi::_loadRecentArticlesWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	WallabagApi *that = (WallabagApi *)userdata;

	size_t data_size = size * nmemb;

	that->json_string = (char *)realloc(that->json_string, that->json_string_len + data_size + 1);
	if (that->json_string == NULL) {
		// TODO error-handling
	}

	memcpy(that->json_string + that->json_string_len, ptr, data_size);
	that->json_string_len += data_size;
	that->json_string[that->json_string_len] = '\0';

	return data_size;
}


void WallabagApi::loadRecentArticles(EntryRepository repository, gui_update_progressbar progressbarUpdater)
{
	progressbarUpdater("Rafraichissement token OAuth", Gui::SYNC_PROGRESS_PERCENTAGE_OAUTH_START, NULL);
	this->refreshOAuthToken();
	progressbarUpdater("Rafraichissement token OAuth", Gui::SYNC_PROGRESS_PERCENTAGE_OAUTH_END, NULL);

	//char buffer[2048];
	//log_message("Chargement des articles...");

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
				config.url.c_str(), encoded_access_token, encoded_sort, encoded_order, 1, 200);

		curl_free(encoded_access_token);
		curl_free(encoded_sort);
		curl_free(encoded_order);

		curl_easy_setopt(curl, CURLOPT_URL, enries_url);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		if (!this->config.http_login.empty() && !this->config.http_password.empty()) {
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
			curl_easy_setopt(curl, CURLOPT_USERNAME, this->config.http_login.c_str());
			curl_easy_setopt(curl, CURLOPT_PASSWORD, this->config.http_password.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WallabagApi::_loadRecentArticlesWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		progressbarUpdater("Envoi requête HTTP chargement entries", Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_HTTP_START, NULL);
		res = curl_easy_perform(curl);
		progressbarUpdater("Envoi requête HTTP chargement entries", Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_HTTP_END, NULL);
		if (res != CURLE_OK) {
			//snprintf(buffer, sizeof(buffer), "Error %d : %s", res, curl_easy_strerror(res));
			//log_message(buffer);

			// TODO error-handling

			goto end;
		}
		else {

			// As we do not have a **sync** yet, only a **fetch**, we remove all entries
			// from local database before inserting those received from the API.
			// This way, we will not have duplicated in the application
			//repository.deleteAll();


			json_object *obj = json_tokener_parse(json_string);

			//snprintf(buffer, sizeof(buffer), "Page %d / %d - posts %d / %d",
			//	json_object_get_int(json_object_object_get(obj, "page")),
			//	json_object_get_int(json_object_object_get(obj, "pages")),
			//	json_object_get_int(json_object_object_get(obj, "limit")),
			//	json_object_get_int(json_object_object_get(obj, "total"))
			//);
			//log_message(buffer);

			progressbarUpdater("Enregistrement des entries en local...", Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_START, NULL);

			array_list *items = json_object_get_array(json_object_object_get(json_object_object_get(obj, "_embedded"), "items"));
			int numberOfEntries = items->length;
			float percentage = (float)Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_START;
			float incrementPercentageEvery = (float)numberOfEntries / (float)(Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_END - Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_START);
			float nextIncrement = incrementPercentageEvery;
			for (int i=0 ; i<numberOfEntries ; i++) {
				json_object *item = (json_object *)array_list_get_idx(items, i);

				int remoteId = json_object_get_int(json_object_object_get(item, "id"));
				Entry localEntry = repository.findByRemoteId(remoteId);
				if (localEntry.id > 0) {
					// Entry already exists in local DB => we must merge the remote data with the local data
					// and save an updated version of the entry in local DB
					Entry remoteEntry = this->entitiesFactory.createEntryFromJson(item);
					Entry entry = this->entitiesFactory.mergeLocalAndRemoteEntries(localEntry, remoteEntry);
					repository.persist(entry);
				}
				else {
					// Entry does not already exist in local DB => just create it
					Entry entry = this->entitiesFactory.createEntryFromJson(item);
					repository.persist(entry);
				}

				if (i >= nextIncrement) {
					nextIncrement += incrementPercentageEvery;
					percentage += 1;
					progressbarUpdater("Enregistrement des entries en local...", percentage, NULL);
				}
			}

			progressbarUpdater("Enregistrement des entries en local...", Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_END, NULL);
		}

		end:
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	free(this->json_string);
}



void WallabagApi::syncEntriesToServer(EntryRepository repository, gui_update_progressbar progressbarUpdater)
{
	this->refreshOAuthToken();

	// Basic idea :
	// For each entry that's been updated more recently on the device than on the server,
	// send updates (archived / starred statuses) to the server

	progressbarUpdater("Sending updated statuses to server", Gui::SYNC_PROGRESS_PERCENTAGE_UP_START, NULL);


	std::vector<Entry> changedEntries;
	repository.findUpdatedLocallyMoreRecentlyThanRemotely(changedEntries);

	int numberOfEntries = changedEntries.size();
	float percentage = (float)Gui::SYNC_PROGRESS_PERCENTAGE_UP_START;
	float incrementPercentageEvery = (float)numberOfEntries / (float)(Gui::SYNC_PROGRESS_PERCENTAGE_UP_END - Gui::SYNC_PROGRESS_PERCENTAGE_UP_START);
	float nextIncrement = incrementPercentageEvery;

	for (unsigned int i=0 ; i<changedEntries.size() ; i++) {
		Entry entry = changedEntries.at(i);

		DEBUG("Must sync entry l#%d r#%s ; local_updated_at=%d remote_updated_at=%d ; local_is_archived=%d remote_is_archived=%d ; local_is_starred=%d remote_is_starred=%d",
			entry.id, entry.remote_id.c_str(),
			entry.local_updated_at, entry.remote_updated_at,
			entry.local_is_archived, entry.remote_is_archived,
			entry.local_is_starred, entry.remote_is_starred
		);

		syncOneEntryToServer(repository, entry);

		if (i >= nextIncrement) {
			nextIncrement += incrementPercentageEvery;
			percentage += 1;
			progressbarUpdater("Sending updated statuses to server", percentage, NULL);
		}
	}

	progressbarUpdater("Sending updated statuses to server", Gui::SYNC_PROGRESS_PERCENTAGE_UP_END, NULL);
}


void WallabagApi::syncOneEntryToServer(EntryRepository repository, Entry &entry)
{
	char enries_url[2048];

	CURL *curl;
	CURLcode res;

	this->json_string_len = 0;
	this->json_string = (char *)calloc(1, 1);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		char *encoded_access_token = curl_easy_escape(curl, this->oauthToken.access_token.c_str(), 0);

		snprintf(enries_url, sizeof(enries_url), "%sapi/entries/%s.json?access_token=%s",
				config.url.c_str(), entry.remote_id.c_str(), encoded_access_token);

		DEBUG("URL: %s", enries_url);

		curl_free(encoded_access_token);


		char postdata[2048];
		snprintf(postdata, sizeof(postdata), "archive=%d&starred=%d", entry.local_is_archived, entry.local_is_starred);
		DEBUG("  -> archive=%d starred=%d", entry.local_is_archived, entry.local_is_starred);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata));


		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");

		curl_easy_setopt(curl, CURLOPT_URL, enries_url);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		if (!this->config.http_login.empty() && !this->config.http_password.empty()) {
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
			curl_easy_setopt(curl, CURLOPT_USERNAME, this->config.http_login.c_str());
			curl_easy_setopt(curl, CURLOPT_PASSWORD, this->config.http_password.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WallabagApi::_loadRecentArticlesWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {

			ERROR("Error %d : %s", res, curl_easy_strerror(res));

			// TODO error-handling

			goto end;
		}
		else {

			json_object *item = json_tokener_parse(json_string);
			Entry remoteEntry = this->entitiesFactory.createEntryFromJson(item);

			DEBUG("Entry l#%d r#%s -> %s", entry.id, entry.remote_id.c_str(), entry.title.c_str());
			DEBUG(" * la=%d->%d ; ra=%d->%d", entry.local_is_archived, remoteEntry.local_is_archived, entry.remote_is_archived, remoteEntry.remote_is_archived);
			DEBUG(" * l*=%d->%d ; r*=%d->%d", entry.local_is_starred, remoteEntry.local_is_starred, entry.remote_is_starred, remoteEntry.remote_is_starred);

			entry = this->entitiesFactory.mergeLocalAndRemoteEntries(entry, remoteEntry);
			repository.persist(entry);
		}

		end:
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	free(this->json_string);
}


