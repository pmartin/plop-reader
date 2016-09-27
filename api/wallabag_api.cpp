#include "wallabag_api.h"


void WallabagApi::setConfig(WallabagConfig conf)
{
	this->config = conf;
}


void WallabagApi::createOAuthToken(gui_update_progressbar progressbarUpdater)
{
	auto getUrl = [this] (CURL *curl) -> char * {
		char *url = (char *)calloc(2048, sizeof(char));
		snprintf(url, 2048, "%soauth/v2/token", config.url.c_str());
		return url;
	};

	auto getMethod = [this] (CURL *curl) -> char * {
		return (char *)strdup("POST");
	};

	auto getData = [this] (CURL *curl) -> char * {
		char *postdata = (char *)malloc(2048);

		char *encoded_client_id = curl_easy_escape(curl, this->config.client_id.c_str(), 0);
		char *encoded_secret_key = curl_easy_escape(curl, this->config.secret_key.c_str(), 0);
		char *encoded_login = curl_easy_escape(curl, this->config.login.c_str(), 0);
		char *encoded_password = curl_easy_escape(curl, this->config.password.c_str(), 0);

		snprintf(postdata, 2048, "grant_type=password&client_id=%s&client_secret=%s&username=%s&password=%s",
				encoded_client_id, encoded_secret_key, encoded_login, encoded_password);

		curl_free(encoded_client_id);
		curl_free(encoded_secret_key);
		curl_free(encoded_login);
		curl_free(encoded_password);

		return postdata;
	};

	auto beforeRequest = [progressbarUpdater] (void) -> void {
		progressbarUpdater("Création token OAuth", Gui::SYNC_PROGRESS_PERCENTAGE_OAUTH_START, NULL);
	};

	auto afterRequest = [progressbarUpdater] (void) -> void {
		progressbarUpdater("Création token OAuth", Gui::SYNC_PROGRESS_PERCENTAGE_OAUTH_END, NULL);
	};

	auto onSuccess = [this] (CURLcode res, char *json_string) -> void {
		json_tokener_error error;
		json_object *json_token = json_tokener_parse_verbose(json_string, &error);
		if (json_token == NULL) {
			ERROR("Could not create OAuth token: server returned an invalid JSON string: %s", json_tokener_error_desc(error));
			throw SyncOAuthException(std::string("Could not create OAuth token: server returned an invalid JSON string: ") + std::string(json_tokener_error_desc(error)));
		}

		const char *access_token = json_object_get_string(json_object_object_get(json_token, "access_token"));
		int expires_in = json_object_get_int(json_object_object_get(json_token, "expires_in"));
		const char *refresh_token = json_object_get_string(json_object_object_get(json_token, "refresh_token"));

		this->oauthToken.access_token = access_token;
		this->oauthToken.refresh_token = refresh_token;
		this->oauthToken.expires_at = time(NULL) + expires_in;
	};

	auto onFailure = [this] (CURLcode res, long response_code, CURL *curl) -> void {
		ERROR("API: createOAuthToken(): failure. HTTP response code = %ld", response_code);

		std::ostringstream ss;
		ss << "Could not create OAuth token: server returned a ";
		ss << response_code;
		ss << " status code.";
		if (response_code == 401) {
			ss << "\n\nYou should set 'http_login' and 'http_password', or check their value, in the JSON configuration file.";
		}
		throw SyncOAuthException(ss.str());
	};

	DEBUG("API: createOAuthToken()");

	doHttpRequest(getUrl, getMethod, getData, beforeRequest, afterRequest, onSuccess, onFailure);

	DEBUG("API: createOAuthToken(): done");
}


void WallabagApi::refreshOAuthToken(gui_update_progressbar progressbarUpdater)
{
	if (this->oauthToken.access_token.empty()) {
		// We do not have an oauth token yet => request one!
		createOAuthToken(progressbarUpdater);
		return;
	}

	if (this->oauthToken.expires_at > time(NULL) - 60) {
		// The OAuth token expires in more than 1 minute => no need to refresh it now
		return;
	}

	auto getUrl = [this] (CURL *curl) -> char * {
		char *url = (char *)calloc(2048, sizeof(char));
		snprintf(url, 2048, "%soauth/v2/token", config.url.c_str());
		return url;
	};

	auto getMethod = [this] (CURL *curl) -> char * {
		return (char *)strdup("POST");
	};

	auto getData = [this] (CURL *curl) -> char * {
		char *postdata = (char *)malloc(2048);

		char *encoded_client_id = curl_easy_escape(curl, this->config.client_id.c_str(), 0);
		char *encoded_secret_key = curl_easy_escape(curl, this->config.secret_key.c_str(), 0);
		char *encoded_refresh_token = curl_easy_escape(curl, this->oauthToken.refresh_token.c_str(), 0);

		snprintf(postdata, 2048, "grant_type=refresh_token&client_id=%s&client_secret=%s&refresh_token=%s",
				encoded_client_id, encoded_secret_key, encoded_refresh_token);

		curl_free(encoded_client_id);
		curl_free(encoded_secret_key);
		curl_free(encoded_refresh_token);

		return postdata;
	};

	auto beforeRequest = [progressbarUpdater] (void) -> void {
		progressbarUpdater("Rafraichissement token OAuth", Gui::SYNC_PROGRESS_PERCENTAGE_OAUTH_START, NULL);
	};

	auto afterRequest = [progressbarUpdater] (void) -> void {
		progressbarUpdater("Rafraichissement token OAuth", Gui::SYNC_PROGRESS_PERCENTAGE_OAUTH_END, NULL);
	};

	auto onSuccess = [this] (CURLcode res, char *json_string) -> void {
		json_tokener_error error;
		json_object *json_token = json_tokener_parse_verbose(json_string, &error);
		if (json_token == NULL) {
			ERROR("Could not refresh OAuth token: server returned an invalid JSON string: %s", json_tokener_error_desc(error));
			throw SyncOAuthException(std::string("Could not refresh OAuth token: server returned an invalid JSON string: ") + std::string(json_tokener_error_desc(error)));
		}

		const char *access_token = json_object_get_string(json_object_object_get(json_token, "access_token"));
		int expires_in = json_object_get_int(json_object_object_get(json_token, "expires_in"));
		const char *refresh_token = json_object_get_string(json_object_object_get(json_token, "refresh_token"));

		this->oauthToken.access_token = access_token;
		this->oauthToken.refresh_token = refresh_token;
		this->oauthToken.expires_at = time(NULL) + expires_in;
	};

	auto onFailure = [this] (CURLcode res, long response_code, CURL *curl) -> void {
		ERROR("API: refreshOAuthToken(): failure. HTTP response code = %ld", response_code);

		std::ostringstream ss;
		ss << "Could not refresh OAuth token: server returned a ";
		ss << response_code;
		ss << " status code.";
		if (response_code == 401) {
			ss << "\n\nYou should set 'http_login' and 'http_password', or check their value, in the JSON configuration file.";
		}
		throw SyncOAuthException(ss.str());
	};

	DEBUG("API: refreshOAuthToken()");

	doHttpRequest(getUrl, getMethod, getData, beforeRequest, afterRequest, onSuccess, onFailure);

	DEBUG("API: refreshOAuthToken(): done");
}


void WallabagApi::loadRecentArticles(EntryRepository repository, time_t lastSyncTimestamp, gui_update_progressbar progressbarUpdater)
{
	this->refreshOAuthToken(progressbarUpdater);

	auto getUrl = [this] (CURL *curl) -> char * {
		char *entries_url = (char *)calloc(2048, sizeof(char));

		char *encoded_access_token = curl_easy_escape(curl, this->oauthToken.access_token.c_str(), 0);
		char *encoded_sort = curl_easy_escape(curl, "updated", 0);
		char *encoded_order = curl_easy_escape(curl, "desc", 0);

		snprintf(entries_url, 2048, "%sapi/entries.json?access_token=%s&sort=%s&order=%s&page=%d&perPage=%d",
				config.url.c_str(), encoded_access_token, encoded_sort, encoded_order, 1, 200);

		curl_free(encoded_access_token);
		curl_free(encoded_sort);
		curl_free(encoded_order);

		return entries_url;
	};

	auto getMethod = [this] (CURL *curl) -> char * {
		return (char *)strdup("GET");
	};

	auto getData = [this] (CURL *curl) -> char * {
		return NULL;
	};

	auto beforeRequest = [progressbarUpdater] (void) -> void {
		progressbarUpdater("Envoi requête HTTP chargement entries", Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_HTTP_START, NULL);
	};

	auto afterRequest = [progressbarUpdater] (void) -> void {
		progressbarUpdater("Envoi requête HTTP chargement entries", Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_HTTP_END, NULL);
	};

	auto onSuccess = [&] (CURLcode res, char *json_string) -> void {
		DEBUG("API: loadRecentArticles(): response fetched from server");
		progressbarUpdater("Enregistrement des entries en local...", Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_START, NULL);

		json_tokener_error error;
		json_object *obj = json_tokener_parse_verbose(json_string, &error);
		if (obj == NULL) {
			ERROR("Could not decode entries: server returned an invalid JSON string: %s", json_tokener_error_desc(error));
			throw SyncOAuthException(std::string("Could not decode entries: server returned an invalid JSON string: ") + std::string(json_tokener_error_desc(error)));
		}

		array_list *items = json_object_get_array(json_object_object_get(json_object_object_get(obj, "_embedded"), "items"));
		int numberOfEntries = items->length;
		DEBUG("API: loadRecentArticles(): number of entries fetched from server: %d", numberOfEntries);

		float percentage = (float)Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_START;
		float incrementPercentageEvery = (float)numberOfEntries / (float)(Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_END - Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_START);
		float nextIncrement = incrementPercentageEvery;

		DEBUG("API: loadRecentArticles(): saving entries to local DB");
		for (int i=0 ; i<numberOfEntries ; i++) {
			json_object *item = (json_object *)array_list_get_idx(items, i);

			Entry remoteEntry = this->entitiesFactory.createEntryFromJson(item);

			if (lastSyncTimestamp != 0) {
				time_t updated_at_ts = remoteEntry.remote_updated_at;
				if (lastSyncTimestamp > updated_at_ts) {
					// Remote updated_at if older than last sync => the entry has not been modified on the server
					// since we last fetched it => no need to re-save it locally
					continue;
				}
			}

			int remoteId = json_object_get_int(json_object_object_get(item, "id"));
			Entry localEntry = repository.findByRemoteId(remoteId);
			if (localEntry.id > 0) {
				// Entry already exists in local DB => we must merge the remote data with the local data
				// and save an updated version of the entry in local DB
				Entry entry = this->entitiesFactory.mergeLocalAndRemoteEntries(localEntry, remoteEntry);
				if (entry._isChanged) {
					DEBUG("API: loadRecentArticles(): updating entry local_id=%d remote_id=%s", entry.id, entry.remote_id.c_str());
					repository.persist(entry);
				}
			}
			else {
				// Entry does not already exist in local DB => just create it
				Entry entry = remoteEntry;
				DEBUG("API: loadRecentArticles(): creating entry for remote_id=%s", entry.id, entry.remote_id.c_str());
				repository.persist(entry);
			}

			if (i >= nextIncrement) {
				nextIncrement += incrementPercentageEvery;
				percentage += 1;
				progressbarUpdater("Enregistrement des entries en local...", percentage, NULL);
			}
		}

		progressbarUpdater("Enregistrement des entries en local...", Gui::SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_END, NULL);
	};

	auto onFailure = [this] (CURLcode res, long response_code, CURL *curl) -> void {
		ERROR("API: loadRecentArticles(): failure. HTTP response code = %ld", response_code);

		std::ostringstream ss;
		ss << "Could not load entries from server: server returned a ";
		ss << response_code;
		ss << " status code.";
		if (response_code == 401) {
			ss << "\n\nYou should set 'http_login' and 'http_password', or check their value, in the JSON configuration file.";
		}
		throw SyncOAuthException(ss.str());
	};

	DEBUG("API: loadRecentArticles()");

	doHttpRequest(getUrl, getMethod, getData, beforeRequest, afterRequest, onSuccess, onFailure);

	DEBUG("API: loadRecentArticles(): done");
}



void WallabagApi::syncEntriesToServer(EntryRepository repository, gui_update_progressbar progressbarUpdater)
{
	this->refreshOAuthToken(progressbarUpdater);

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

	DEBUG("API: syncEntriesToServer()");

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

	DEBUG("API: syncEntriesToServer(): done");
}


void WallabagApi::syncOneEntryToServer(EntryRepository repository, Entry &entry)
{
	auto getUrl = [this, &entry] (CURL *curl) -> char * {
		char *url = (char *)calloc(2048, sizeof(char));
		char *encoded_access_token = curl_easy_escape(curl, this->oauthToken.access_token.c_str(), 0);
		snprintf(url, 2048, "%sapi/entries/%s.json?access_token=%s",
				config.url.c_str(), entry.remote_id.c_str(), encoded_access_token);
		return url;
	};

	auto getMethod = [this] (CURL *curl) -> char * {
		return (char *)strdup("PATCH");
	};

	auto getData = [this, &entry] (CURL *curl) -> char * {
		char *postdata = (char *)malloc(2048);
		snprintf(postdata, 2048, "archive=%d&starred=%d", entry.local_is_archived, entry.local_is_starred);
		return postdata;
	};

	auto beforeRequest = [] (void) -> void {};

	auto afterRequest = [] (void) -> void {};

	auto onSuccess = [this, &repository, &entry] (CURLcode res, char *json_string) -> void {
		json_object *item = json_tokener_parse(json_string);
		Entry remoteEntry = this->entitiesFactory.createEntryFromJson(item);

		DEBUG("Entry l#%d r#%s -> %s", entry.id, entry.remote_id.c_str(), entry.title.c_str());
		DEBUG(" * la=%d->%d ; ra=%d->%d", entry.local_is_archived, remoteEntry.local_is_archived, entry.remote_is_archived, remoteEntry.remote_is_archived);
		DEBUG(" * l*=%d->%d ; r*=%d->%d", entry.local_is_starred, remoteEntry.local_is_starred, entry.remote_is_starred, remoteEntry.remote_is_starred);

		entry = this->entitiesFactory.mergeLocalAndRemoteEntries(entry, remoteEntry);
		repository.persist(entry);
	};

	auto onFailure = [this] (CURLcode res, long response_code, CURL *curl) -> void {
		ERROR("API: syncOneEntryToServer(): failure. HTTP response code = %ld", response_code);

		std::ostringstream ss;
		ss << "Could not sync entry to server: server returned a ";
		ss << response_code;
		ss << " status code.";
		if (response_code == 401) {
			ss << "\n\nYou should set 'http_login' and 'http_password', or check their value, in the JSON configuration file.";
		}
		throw SyncOAuthException(ss.str());
	};

	DEBUG("API: syncOneEntryToServer(%d)", entry.id);

	doHttpRequest(getUrl, getMethod, getData, beforeRequest, afterRequest, onSuccess, onFailure);

	DEBUG("API: syncOneEntryToServer(%d): done", entry.id);
}


CURLcode WallabagApi::doHttpRequest(
	std::function<char * (CURL *curl)> getUrl,
	std::function<char * (CURL *curl)> getMethod,
	std::function<char * (CURL *curl)> getData,
	std::function<void (void)> beforeRequest,
	std::function<void (void)> afterRequest,
	std::function<void (CURLcode res, char *json_string)> onSuccess,
	std::function<void (CURLcode res, long response_code, CURL *curl)> onFailure
)
{
	CURL *curl;
	CURLcode res = CURLE_OK;

	curl = curl_easy_init();
	if (curl) {
		this->json_string_len = 0;
		this->json_string = (char *)calloc(1, 1);

		char *url = getUrl(curl);
		char *method = getMethod(curl);
		char *data = getData(curl);

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		if (data != NULL) {
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
		}

		if (!this->config.http_login.empty() && !this->config.http_password.empty()) {
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
			curl_easy_setopt(curl, CURLOPT_USERNAME, this->config.http_login.c_str());
			curl_easy_setopt(curl, CURLOPT_PASSWORD, this->config.http_password.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WallabagApi::_curlWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		beforeRequest();

		res = curl_easy_perform(curl);

		afterRequest();

		free(url);
		free(method);
		if (data != NULL) {
			free(data);
		}

		long response_code;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		try {
			if (res != CURLE_OK || response_code != 200) {
				onFailure(res, response_code, curl);
			}
			else {
				onSuccess(res, json_string);
			}

			free(this->json_string);
			curl_easy_cleanup(curl);
		}
		catch (std::exception &e) {
			free(this->json_string);
			curl_easy_cleanup(curl);

			throw;
		}
	}

	return res;
}


size_t WallabagApi::WallabagApi::_curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
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
