#ifndef BELLADONNA_H_
#define BELLADONNA_H_

#include "inkview.h"

#include "curl/curl.h"
#include "json-c/json.h"
#include "sqlite3.h"


extern ifont *font;
extern const int kFontSize;



// config
typedef struct b_config_s
{
	char *url;
	char *http_login, *http_password;
	char *client_id;
	char *secret_key;
	char *login, *password;
} b_config_s;

// oauth
typedef struct b_oauth_token_s
{
	char *access_token;
	char *refresh_token;
	int expires_at;
} b_oauth_token_s;



// api
extern void load_recent_articles(b_config_s *conf, b_oauth_token_s *token);


// config
extern b_config_s *config;

extern void load_config();
extern void unload_config();


// database
extern void database_drop();
extern void database_open();
extern void database_close();

extern void database_write_entry(
		int remote_id, int is_archived, int is_starred,
		const char *title, const char *url, const char *content,
		const char *created_at, const char *updated_at,
		int reading_time
	);

void database_display_entries();


// log
extern void log_reset();
extern void log_message(const char *msg);


// oauth
extern b_oauth_token_s *oauth_token;

extern void create_token();
extern void destroy_token(b_oauth_token_s *token);



#endif /* BELLADONNA_H_ */
