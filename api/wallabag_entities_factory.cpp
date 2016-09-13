#include "wallabag_entities_factory.h"


Entry WallabagEntitiesFactory::createEntryFromJson(json_object *item)
{
	int id = json_object_get_int(json_object_object_get(item, "id"));
	int is_archived = json_object_get_int(json_object_object_get(item, "is_archived"));
	int is_starred = json_object_get_int(json_object_object_get(item, "is_starred"));
	const char *title = json_object_get_string(json_object_object_get(item, "title"));
	const char *url = json_object_get_string(json_object_object_get(item, "url"));
	const char *content = json_object_get_string(json_object_object_get(item, "content"));
	const char *created_at_str = json_object_get_string(json_object_object_get(item, "created_at"));
	const char *updated_at_str = json_object_get_string(json_object_object_get(item, "updated_at"));
	int reading_time = json_object_get_int(json_object_object_get(item, "reading_time")) * 60;
	const char *preview_picture = json_object_get_string(json_object_object_get(item, "preview_picture"));

	//snprintf(buffer, sizeof(buffer), "%d - (%c%c) %s (%s)", id, (is_archived ? 'a' : '.'), (is_starred ? '*' : '.'), title, url);
	//log_message(buffer);

	struct tm tm;
	memset(&tm, 0, sizeof(struct tm));
	strptime(created_at_str, "%Y-%m-%dT%H:%M:%S%z", &tm);
	time_t created_at_ts = timegm(&tm);

	memset(&tm, 0, sizeof(struct tm));
	strptime(updated_at_str, "%Y-%m-%dT%H:%M:%S%z", &tm);
	time_t updated_at_ts = timegm(&tm);

	Entry entry;
	entry.id = 0;

	char tmp[128];
	snprintf(tmp, sizeof(tmp), "%d", id);
	entry.remote_id = tmp;

	// When creating an entry locally, its local values are the same as remote ones
	entry.local_is_archived = is_archived;
	entry.remote_is_archived = is_archived;

	entry.local_is_starred = is_starred;
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

	entry.local_created_at = created_at_ts;
	entry.remote_created_at = created_at_ts;

	entry.local_updated_at = updated_at_ts;
	entry.remote_updated_at = updated_at_ts;

	entry.reading_time = reading_time;
	if (preview_picture != NULL) {
		entry.preview_picture_url = preview_picture;
	}

	entry.local_content_file_html = std::string();
	entry.local_content_file_epub = std::string();

	return entry;
}


Entry WallabagEntitiesFactory::mergeLocalAndRemoteEntries(Entry &local, Entry &remote)
{
	Entry entry;

	entry.id = local.id;
	entry.remote_id = remote.remote_id;

	entry.remote_updated_at = remote.remote_updated_at;

	if (remote.remote_updated_at > local.local_updated_at && local.local_is_archived != remote.remote_is_archived) {
		entry.local_is_archived = remote.remote_is_archived;
	}
	else {
		entry.local_is_archived = local.local_is_archived;
	}
	entry.remote_is_archived = remote.remote_is_archived;

	if (remote.remote_updated_at > local.local_updated_at && local.local_is_starred != remote.remote_is_starred) {
		entry.local_is_starred = remote.remote_is_starred;
	}
	else {
		entry.local_is_starred = local.local_is_starred;
	}
	entry.remote_is_starred = remote.remote_is_starred;

	// For some fields, we always use what comes from the remote
	entry.remote_created_at = remote.remote_created_at;
	entry.title = remote.title;
	entry.url = remote.url;
	entry.content = remote.content;
	entry.reading_time = remote.reading_time;
	entry.preview_picture_url = remote.preview_picture_url;

	// For some other fields that are more specific to the application, we always use what we had locally
	entry.local_created_at = local.local_created_at;
	entry.local_content_file_html = local.local_content_file_html;
	entry.local_content_file_epub = local.local_content_file_epub;

	// TODO mettre à jour date locale si on a récupèré des infos du serveur
	entry.local_updated_at = local.local_updated_at;

	return entry;
}
