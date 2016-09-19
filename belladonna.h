#ifndef BELLADONNA_H_
#define BELLADONNA_H_

#include "inkview.h"

#include "curl/curl.h"
#include "json-c/json.h"
#include "sqlite3.h"


#include "application.h"


#define BELLADONNA_VERSION_STR "v0.1.0"
#define BELLADONNA_VERSION_NUM 1
#define BELLADONNA_WEBSITE_URL "http://belladonna-reader.org"
#define BELLADONNE_OPENSOURCE_URL "https://github.com/pmartin/belladonna"


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
#include "log.h"

#endif /* BELLADONNA_H_ */
