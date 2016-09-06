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
	iv_unlink(Database::DB_FILE);
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
	this->createEntriesTable();
}


void Database::createEntriesTable()
{
	char *err_msg;
	char buffer[2048];
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
	preview_picture_path text null
)
)sql";
	if (sqlite3_exec(this->db, sql, NULL, 0, &err_msg) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail creating table : %s", err_msg);
		log_message(buffer);
	}
}


Database::~Database()
{
	sqlite3_close(this->db);
}


