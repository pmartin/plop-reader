#ifndef API_WALLABAG_ENTITIES_FACTORY_H_
#define API_WALLABAG_ENTITIES_FACTORY_H_


#include "json-c/json.h"
#include <stdio.h>
#include <string.h>

#include "../entities/entry.h"


class WallabagEntitiesFactory
{
public:

	Entry createEntryFromJson(json_object *item);
	Entry mergeLocalAndRemoteEntries(Entry &local, Entry &remote);

private:
	time_t datetimeStringToTimeT(const char *str);

};


#endif /* API_WALLABAG_ENTITIES_FACTORY_H_ */
