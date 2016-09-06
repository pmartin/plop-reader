#include "wallabag_entities_factory.h"


Entry WallabagEntitiesFactory::createEntryFromJson(json_object *item)
{
	int id = json_object_get_int(json_object_object_get(item, "id"));
	int is_archived = json_object_get_int(json_object_object_get(item, "is_archived"));
	int is_starred = json_object_get_int(json_object_object_get(item, "is_starred"));
	const char *title = json_object_get_string(json_object_object_get(item, "title"));
	const char *url = json_object_get_string(json_object_object_get(item, "url"));
	const char *content = json_object_get_string(json_object_object_get(item, "content"));
	const char *created_at = json_object_get_string(json_object_object_get(item, "created_at"));
	const char *updated_at = json_object_get_string(json_object_object_get(item, "updated_at"));
	int reading_time = json_object_get_int(json_object_object_get(item, "reading_time")) * 60;
	const char *preview_picture = json_object_get_string(json_object_object_get(item, "preview_picture"));

	//snprintf(buffer, sizeof(buffer), "%d - (%c%c) %s (%s)", id, (is_archived ? 'a' : '.'), (is_starred ? '*' : '.'), title, url);
	//log_message(buffer);

	Entry entry;
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "%d", id);
	entry.remote_id = tmp;
	entry.remote_is_archived = is_archived;
	entry.remote_is_starred = is_starred;
	if (title != NULL) {
		entry.title = title;
	}
	if (url != NULL) {
		entry.url = url;
	}
	if (content != NULL) {
		entry.content = content;
	}
	entry.remote_created_at = created_at;
	entry.remote_updated_at = updated_at;
	entry.reading_time = reading_time;
	if (preview_picture != NULL) {
		entry.preview_picture_url = preview_picture;
	}

	return entry;
}
