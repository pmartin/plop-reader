#include "database.h"


int Database::callback_debug_log(void *not_used, int argc, char **argv, char **col_name)
{
	char buffer[128*5];
	char row_buffer[256];

	strcpy(buffer, " > ");
	for(int i=0; i<argc && i<14 ; i++) {
		const char *short_name = NULL;
		const char *aliases[][2] = {
			 {"local_id", "l#"},
			 {"remote_id", "r#"},
			 {"local_is_archived", "la"},
			 {"remote_is_archived", "ra"},
			 {"local_is_starred", "l*"},
			 {"remote_is_starred", "r*"},
			 {"local_created_at", "lcT"},
			 {"remote_created_at", "rcT"},
			 {"local_updated_at", "luT"},
			 {"remote_updated_at", "ruT"},
			 {"reading_time", "t"},
			 {"preview_picture_url", "pu"},
			 {"preview_picture_type", "pt"},
			 {"preview_picture_path", "pp"}
		};
		const int nb_aliases = 14;
		for (int a=0 ; a<nb_aliases ; a++) {
			if (iv_strcmp(aliases[a][0], col_name[i]) == 0) {
				short_name = aliases[a][1];
				break;
			}
		}

		snprintf(row_buffer, sizeof(row_buffer), "%s=%s, ", (short_name ? short_name : col_name[i]), argv[i] ? argv[i] : "NULL");
		strcat(buffer, row_buffer);
	}

	if (strlen(buffer) > 3) {
		log_message(buffer);
	}
	return 0;
}


void Database::drop(void)
{
	sqlite3_close(this->db);

	iv_unlink(Database::DB_FILE);

	this->open();
}


void Database::open(void)
{
	char buffer[2048];
	if (sqlite3_open(Database::DB_FILE, &this->db)) {
		snprintf(buffer, sizeof(buffer), "Fail opening DB : %s", sqlite3_errmsg(this->db));
		log_message(buffer);
	}
}


void Database::runMigrations(void)
{
	Internal migrationVersion = this->selectInternal("db.migration_version");
	if (migrationVersion.isNull) {
		migrationVersion.value = "0";
	}

	if (migrationVersion.value < "1") {
		//log_message("Create table internals");
		this->migration_001_createInternalsTable();
	}
	if (migrationVersion.value < "2") {
		//log_message("Create table entries");
		this->migration_002_createEntriesTable();
	}

	this->saveInternal("db.migration_version", "2");
}


void Database::migration_001_createInternalsTable()
{
	char *err_msg;
	char buffer[2048];
	const char *sql = R"sql(
create table internals (
	key text primary key,
    value text,
    created_at text not null,
    updated_at text not null
)
)sql";
	if (sqlite3_exec(this->db, sql, NULL, 0, &err_msg) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail creating table : %s", err_msg);
		log_message(buffer);
	}
}


void Database::migration_002_createEntriesTable()
{
	char *err_msg;
	char buffer[2048];

	// TODO add index on remote_id (used to find an entry during sync)
	// TODO add index on remote_created_at (use for sort in entries list)

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

	local_created_at text not null,
	remote_created_at text null,

	local_updated_at text not null,
	remote_updated_at text null,

	reading_time integer null,

	preview_picture_url text null,
	preview_picture_type integer null,
	preview_picture_path text null,

    local_content_file_html text null,
    local_content_file_epub text null
)
)sql";
	if (sqlite3_exec(this->db, sql, NULL, 0, &err_msg) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail creating table : %s", err_msg);
		log_message(buffer);
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
	const char *sqlInsert = "insert into internals (`key`, `value`, `created_at`, `updated_at`) values (:key, :value, datetime(), datetime())";

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
	const char *sqlUpdate = "update internals set value = :value, updated_at = datetime() where key = :key";

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


