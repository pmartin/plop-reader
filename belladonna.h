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

extern b_config_s *config;

extern void load_config();
extern void unload_config();


// log
extern void log_reset();
extern void log_message(const char *msg);


// oauth
extern void create_token();



#endif /* BELLADONNA_H_ */
