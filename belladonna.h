#ifndef BELLADONNA_H_
#define BELLADONNA_H_

#include "inkview.h"

#include "curl/curl.h"
#include "json-c/json.h"
#include "sqlite3.h"


#include "application.h"


extern Application app;


// api
#include "api/wallabag_api.h"
#include "api/wallabag_config_loader.h"
#include "api/wallabag_config.h"
#include "api/wallabag_entities_factory.h"
#include "api/wallabag_oauth_token.h"

// database
#include "database/database.h"

// entities
#include "entities/entry.h"

// repositories
#include "repositories/entry_repository.h"

// gui
#include "gui/gui.h"


// log
// TODO remove all this old-school logging stuff
extern void log_init();
extern void log_reset();
extern void log_message(const char *msg);
extern void log_close();


#endif /* BELLADONNA_H_ */
