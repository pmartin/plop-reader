#include "database.h"


void Database::drop(void)
{
	sqlite3_close(this->db);

	iv_unlink(Database::DB_FILE);

	this->open();
}


void Database::open(void)
{
	//char buffer[2048];
	if (sqlite3_open(Database::DB_FILE, &this->db)) {
		//snprintf(buffer, sizeof(buffer), "Fail opening DB : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);

		// TODO error-handling
	}
}


void Database::runMigrations(void)
{
	const std::string currentVersion = "3";

	Internal migrationVersion = this->selectInternal("db.migration_version");
	if (migrationVersion.isNull) {
		migrationVersion.value = "0";
	}

	if (migrationVersion.value < "1") {
		this->migration_001_createInternalsTable();
	}
	if (migrationVersion.value < "2") {
		this->migration_002_createEntriesTable();
	}
	if (migrationVersion.value < "3") {
		this->migration_003_createIndexesOnEntries();
	}

	this->saveInternal("db.migration_version", currentVersion);
}


void Database::migration_001_createInternalsTable()
{
	char *err_msg;
	//char buffer[2048];
	const char *sql = R"sql(
create table internals (
	key text primary key,
    value text,
    created_at integer not null,
    updated_at integer not null
)
)sql";
	if (sqlite3_exec(this->db, sql, NULL, 0, &err_msg) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail creating table : %s", err_msg);
		//log_message(buffer);

		// TODO error-handling
	}
}


void Database::migration_002_createEntriesTable()
{
	char *err_msg;
	//char buffer[2048];
	const char *sql = R"sql(
create table entries (
	local_id integer primary key,
	remote_id text not null,
	
	local_is_archived integer not null default 0,
	remote_is_archived integer not null default 0,

	local_is_starred integer not null default 0,
	remote_is_starred integer not null default 0,

	title text not null,
	url text not null,
	content text not null,

	local_created_at integer not null,
	remote_created_at integer null,

	local_updated_at integer not null,
	remote_updated_at integer null,

	reading_time integer null,

	preview_picture_url text null,
	preview_picture_type integer null,
	preview_picture_path text null,

    local_content_file_html text null,
    local_content_file_epub text null
)
)sql";
	if (sqlite3_exec(this->db, sql, NULL, 0, &err_msg) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail creating table : %s", err_msg);
		//log_message(buffer);

		// TODO error-handling
	}
}

void Database::migration_003_createIndexesOnEntries()
{
	char *err_msg;
	//char buffer[2048];
	const char *sql = R"sql(
create unique index idx_entries_remote_id on entries (remote_id);
create index idx_entries_local_updated_at on entries (local_updated_at desc);
)sql";
	if (sqlite3_exec(this->db, sql, NULL, 0, &err_msg) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail migration 003 : %s", err_msg);
		//log_message(buffer);

		// TODO error-handling
	}
}


void Database::saveInternal(std::string key, std::string value)
{
	Internal internal = this->selectInternal(key);
	if (internal.isNull) {
		this->insertInternal(key, value);
		return;
	}
	if (internal.value != value) {
		this->updateInternal(key, value);
		return;
	}
}


Internal Database::selectInternal(std::string key)
{
	const char *sqlSelect = "select value from internals where key = :key";

	sqlite3_stmt *stmt;
	const char *tail;
	int result;

	//char buffer[2048];

	Internal internal;
	internal.key = key;
	internal.isNull = true;

	if (sqlite3_prepare(this->db, sqlSelect, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":key"), key.c_str(), key.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}

	if ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
		internal.isNull = false;
		internal.value = (const char *)sqlite3_column_text(stmt, 0);
	}

	sqlite3_finalize(stmt);

	return internal;
}


void Database::insertInternal(std::string key, std::string value)
{
	const char *sqlInsert = "insert into internals (`key`, `value`, `created_at`, `updated_at`) values (:key, :value, strftime('%s','now'), strftime('%s','now'))";

	sqlite3_stmt *stmt;
	const char *tail;

	//char buffer[2048];

	if (sqlite3_prepare(this->db, sqlInsert, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":key"), key.c_str(), key.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":value"), value.c_str(), value.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		//snprintf(buffer, sizeof(buffer), "Fail inserting : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	sqlite3_finalize(stmt);
}


void Database::updateInternal(std::string key, std::string value)
{
	const char *sqlUpdate = "update internals set value = :value, updated_at = strftime('%s','now') where key = :key";

	sqlite3_stmt *stmt;
	const char *tail;

	//char buffer[2048];

	if (sqlite3_prepare(this->db, sqlUpdate, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":key"), key.c_str(), key.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":value"), value.c_str(), value.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		//snprintf(buffer, sizeof(buffer), "Fail updating : %s", sqlite3_errmsg(this->db));
		//log_message(buffer);
	}
	sqlite3_finalize(stmt);
}


Database::~Database()
{
	sqlite3_close(this->db);
}


