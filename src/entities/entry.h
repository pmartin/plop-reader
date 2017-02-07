#ifndef ENTITIES_ENTRY_H_
#define ENTITIES_ENTRY_H_

#include <string>

class Entry
{
public:

	int id;
	std::string remote_id;

	int local_is_archived;
	int remote_is_archived;

	int local_is_starred;
	int remote_is_starred;

	std::string title;
	std::string url;
	std::string content;

	int local_created_at;
	int remote_created_at;

	int local_updated_at;
	int remote_updated_at;

	int reading_time;

	std::string preview_picture_url;
	int preview_picture_type;
	std::string preview_picture_path;

	std::string local_content_file_html;
	std::string local_content_file_epub;

	bool is_empty;

	bool _isChanged;
};


#endif /* ENTITIES_ENTRY_H_ */
