#include "inkview.h"

#include "curl/curl.h"
#include "json-c/json.h"
#include "sqlite3.h"


static ifont *font;
static const int kFontSize = 16;
static int y_log;

static void log_message(const char *msg)
{
	if (strlen(msg) == 0) {
		return;
	}
	DrawTextRect(0, y_log, ScreenWidth(), kFontSize, msg, ALIGN_LEFT);
	PartialUpdate(0, y_log, ScreenWidth(), y_log + kFontSize + 2);
	y_log += kFontSize + 2;
}



typedef struct b_config_s
{
	char *url;
	char *http_login, *http_password;
	char *client_id;
	char *secret_key;
	char *login, *password;
} b_config_s;

b_config_s *config;




static void load_config()
{
	char buffer[2048];
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


	log_message("JSON config:");
	snprintf(buffer, sizeof(buffer), "URL = %s", config->url);
	log_message(buffer);
}


static void unload_config()
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



static size_t header_callback(char *ptr, size_t size, size_t nitems, void *userdata)
{
	char buffer[1024];

	int data_size = size * nitems;
	snprintf(buffer, sizeof(buffer), "  Header : %d bytes : %.128s", data_size, ptr);
	log_message(buffer);

	return data_size;
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	// Warning: the received data it not NUL-terminated
	char buffer[1024];

	int data_size = size * nmemb;
	snprintf(buffer, sizeof(buffer), "  Data %d bytes : %.128s", data_size, ptr);

	for (int i=0 ; i<strlen(buffer) ; i++) {
		if (buffer[i] == '\n') {
			buffer[i] = '`';
		}
	}

	log_message(buffer);



	// Even if we didn't display everything, we signal the system we used all received data
	return data_size;
}


static void create_token()
{
	CURL *curl;
	CURLcode res;

	char buffer[2048];

	char url[2048];
	snprintf(url, sizeof(url), "%soauth/v2/token", config->url);
	log_message(url);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		char postdata[2048];
		char *encoded_client_id = curl_easy_escape(curl, config->client_id, 0);
		char *encoded_secret_key = curl_easy_escape(curl, config->secret_key, 0);
		char *encoded_login = curl_easy_escape(curl, config->login, 0);
		char *encoded_password = curl_easy_escape(curl, config->password, 0);

		snprintf(buffer, sizeof(buffer), "username=%s&password=%s", encoded_login, encoded_password);
		log_message(buffer);

		snprintf(buffer, sizeof(buffer), "client_id=%s&client_secret=%s", encoded_client_id, encoded_secret_key);
		log_message(buffer);

		snprintf(postdata, sizeof(postdata), "grant_type=password&client_id=%s&client_secret=%s&username=%s&password=%s",
				encoded_client_id, encoded_secret_key, encoded_login, encoded_password);

		curl_free(encoded_client_id);
		curl_free(encoded_secret_key);
		curl_free(encoded_login);
		curl_free(encoded_password);

		log_message(postdata);

		snprintf(buffer, sizeof(buffer), "length post = %d", strlen(postdata));
		log_message(buffer);


		char auth[512];
		snprintf(auth, sizeof(auth), "%s:%s", config->http_login, config->http_password);
		log_message(auth);

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

		end:
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
}


static int main_handler(int event_type, int param_one, int param_two)
{
	int result = 0;

	static int step = 0;

	switch (event_type) {
	case EVT_INIT:
		font = OpenFont("LiberationSans", kFontSize, 1);
		SetFont(font, BLACK);
		y_log = 0;
		ClearScreen();
		FullUpdate();
		break;
	case EVT_SHOW:

		break;
	case EVT_KEYPRESS:
		if (param_one == KEY_PREV) {
			CloseApp();
			return 1;
		}
		else if (param_one == KEY_NEXT) {
			load_config();

			create_token();

			/*
			if (step == 0) {

			}
			else {
				CloseApp();
			}
			//*/

			step++;
			return 1;
		}

		break;
	case EVT_EXIT:
		unload_config();
		CloseFont(font);
		break;
	default:
		break;
	}

    return result;
}


int main (int argc, char* argv[])
{
    InkViewMain(main_handler);

    return 0;
}
