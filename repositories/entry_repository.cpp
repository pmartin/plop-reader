#include "entry_repository.h"


void EntryRepository::persist(Entry entry)
{
	char buffer[2048];

	sqlite3_stmt *stmt;
	const char *tail;
	const char *sql = R"sql(
insert into entries (
	remote_id, remote_is_archived, remote_is_starred, 
	title, url, content, 
	remote_created_at, remote_updated_at,
	local_created_at, local_updated_at,
	reading_time, preview_picture_url,
    local_content_file_html, local_content_file_epub
)
values (
	:remote_id, :is_archived, :is_starred, 
	:title, :url, :content,
	:remote_created_at, :remote_updated_at,
	datetime(), datetime(),
	:reading_time, :preview_picture_url,
    :local_content_file_html, :local_content_file_epub
)
)sql";
	if (sqlite3_prepare(this->db.getDb(), sql, -1, &stmt, &tail) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}

	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":remote_id"), entry.remote_id.c_str(), entry.remote_id.length(), SQLITE_STATIC) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":is_archived"), entry.remote_is_archived) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":is_starred"), entry.remote_is_starred) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":title"), entry.title.c_str(), entry.title.length(), SQLITE_STATIC) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":url"), entry.url.c_str(), entry.url.length(), SQLITE_STATIC) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":content"), entry.content.c_str(), entry.content.length(), SQLITE_STATIC) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":remote_created_at"), entry.remote_created_at.c_str(), entry.remote_created_at.length(), SQLITE_STATIC) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":remote_updated_at"), entry.remote_updated_at.c_str(), entry.remote_updated_at.length(), SQLITE_STATIC) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":reading_time"), entry.reading_time) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}

	if (entry.preview_picture_url.length() > 0) {
		if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":preview_picture_url"), entry.preview_picture_url.c_str(), entry.preview_picture_url.length(), SQLITE_STATIC) != SQLITE_OK) {
			snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
			log_message(buffer);
		}
	}
	else {
		if (sqlite3_bind_null(stmt, sqlite3_bind_parameter_index(stmt, ":preview_picture_url")) != SQLITE_OK) {
			snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
			log_message(buffer);
		}
	}

	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":local_content_file_html"), entry.local_content_file_html.c_str(), entry.local_content_file_html.length(), SQLITE_STATIC) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":local_content_file_epub"), entry.local_content_file_epub.c_str(), entry.local_content_file_epub.length(), SQLITE_STATIC) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		snprintf(buffer, sizeof(buffer), "Fail inserting : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	sqlite3_finalize(stmt);
}


void EntryRepository::deleteAll()
{
	char *err_msg;
	char buffer[2048];
	const char *sql = "delete from entries";
	if (sqlite3_exec(this->db.getDb(), sql, NULL, 0, &err_msg) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail deleting all from 'entries' table : %s", err_msg);
		log_message(buffer);
	}
}


void EntryRepository::list(std::vector<Entry> &entries, int limit, int offset)
{
	//std::vector<Entry> entries;

	entries.clear();
	entries.reserve(limit + 10);

	//char buffer[2048];

	const char *sql = R"sql(
select 
    local_id, 
    remote_id, 
    local_is_archived, 
    remote_is_archived,
    local_is_starred,
    remote_is_starred,
    title,
    url,
    content,
    local_created_at,
    remote_created_at,
    local_updated_at,
    remote_updated_at,
    reading_time,
    preview_picture_url,
    preview_picture_type,
    preview_picture_path,
    local_content_file_html,
    local_content_file_epub
from entries 
order by remote_created_at desc 
limit :limit 
offset :offset
)sql";

	sqlite3_stmt *stmt;
	const char *tail;
	int result;

	if (sqlite3_prepare(this->db.getDb(), sql, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":limit"), limit) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":offset"), offset) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
		Entry entry;
		const char *tmp;

		entry.id = sqlite3_column_int(stmt, 0);
		entry.remote_id = (const char *)sqlite3_column_text(stmt, 1);
		entry.local_is_archived = sqlite3_column_int(stmt, 2);
		entry.remote_is_archived = sqlite3_column_int(stmt, 3);
		entry.local_is_starred = sqlite3_column_int(stmt, 4);
		entry.remote_is_starred = sqlite3_column_int(stmt, 5);
		entry.title = (const char *)sqlite3_column_text(stmt, 6);
		entry.url = (const char *)sqlite3_column_text(stmt, 7);
		entry.content = (const char *)sqlite3_column_text(stmt, 8);
		entry.local_created_at = (const char *)sqlite3_column_text(stmt, 9);
		entry.remote_created_at = ((tmp = (const char *)sqlite3_column_text(stmt, 10))) ? tmp : std::string();
		entry.local_updated_at = (const char *)sqlite3_column_text(stmt, 11);
		entry.remote_updated_at = ((tmp = (const char *)sqlite3_column_text(stmt, 12))) ? tmp : std::string();
		entry.reading_time = sqlite3_column_int(stmt, 13);
		entry.preview_picture_url = ((tmp = (const char *)sqlite3_column_text(stmt, 14))) ? tmp : std::string();
		entry.preview_picture_type = sqlite3_column_int(stmt, 15);
		entry.preview_picture_path = ((tmp = (const char *)sqlite3_column_text(stmt, 16))) ? tmp : std::string();
		entry.local_content_file_html = ((tmp = (const char *)sqlite3_column_text(stmt, 17))) ? tmp : std::string();
		entry.local_content_file_epub = ((tmp = (const char *)sqlite3_column_text(stmt, 18))) ? tmp : std::string();

		entries.push_back(entry);
	}
	sqlite3_finalize(stmt);

	//return entries;
}


int EntryRepository::countAllEntries()
{
	const char *sql = "select count(*) from entries";
	sqlite3_stmt *stmt;
	const char *tail;

	if (sqlite3_prepare(this->db.getDb(), sql, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	int count = -1;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}

	sqlite3_finalize(stmt);

	return count;
}


