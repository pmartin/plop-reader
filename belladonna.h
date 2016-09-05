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


// api
#include "api/wallabag_api.h"
#include "api/wallabag_oauth_token.h"

// entities
#include "entities/entry.h"


// config
extern b_config_s *config;

extern void load_config();
extern void unload_config();


// database
extern void database_drop();
extern void database_open();
extern void database_close();

extern void database_write_entry(Entry entry);

void database_display_entries();


// log
extern void log_reset();
extern void log_message(const char *msg);


#endif /* BELLADONNA_H_ */
