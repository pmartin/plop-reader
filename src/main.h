#ifndef MAIN_H_
#define MAIN_H_

#include "inkview.h"

#include "curl/curl.h"
#include "json-c/json.h"
#include "sqlite3.h"


#include "application.h"
#include "exceptions.h"


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

#endif /* MAIN_H_ */
