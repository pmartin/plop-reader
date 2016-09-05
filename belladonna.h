#ifndef BELLADONNA_H_
#define BELLADONNA_H_

#include "inkview.h"

#include "curl/curl.h"
#include "json-c/json.h"
#include "sqlite3.h"


extern ifont *font;
extern const int kFontSize;


// api
#include "api/wallabag_api.h"
#include "api/wallabag_config_loader.h"
#include "api/wallabag_config.h"
#include "api/wallabag_oauth_token.h"

// entities
#include "entities/entry.h"

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
