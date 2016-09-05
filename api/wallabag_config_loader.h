#ifndef API_WALLABAG_CONFIG_LOADER_H_
#define API_WALLABAG_CONFIG_LOADER_H_

#include "wallabag_config.h"

#include "json-c/json.h"


class WallabagConfigLoader
{
public:
	WallabagConfig load(void);
};


#endif /* API_WALLABAG_CONFIG_LOADER_H_ */
