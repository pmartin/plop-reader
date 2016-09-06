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
	reading_time, preview_picture_url
)
values (
	:remote_id, :is_archived, :is_starred, 
	:title, :url, :content,
	:remote_created_at, :remote_updated_at,
	datetime(), datetime(),
	:reading_time, :preview_picture_url
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

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		snprintf(buffer, sizeof(buffer), "Fail inserting : %s", sqlite3_errmsg(this->db.getDb()));
		log_message(buffer);
	}
	sqlite3_finalize(stmt);
}


Entry *EntryRepository::list()
{
	Entry *entries = NULL;

	char *err_msg;

	char buffer[2048];

	const char *sql = "select * from entries order by remote_created_at desc limit 10 offset 0";
	if (sqlite3_exec(this->db.getDb(), sql, Database::callback_debug_log, 0, &err_msg) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail selecting : %s", err_msg);
		log_message(buffer);
	}

	return entries;
}


