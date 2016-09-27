#ifndef API_WALLABAG_CONFIG_LOADER_H_
#define API_WALLABAG_CONFIG_LOADER_H_

#include "inkview.h"
#include "json-c/json.h"

#include "wallabag_config.h"
#include "../defines.h"
#include "../log.h"
#include "../exceptions.h"

class WallabagConfigLoader
{
public:
	WallabagConfig load(void);

private:
	const char *CONFIG_FILE = PLOP_BASE_DIRECTORY "/config.json";

	char *readConfigFile(const char *path);
};


#endif /* API_WALLABAG_CONFIG_LOADER_H_ */
