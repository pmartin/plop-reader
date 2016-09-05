#include "belladonna.h"

// debugging function, to display what's in the DB,
// until we have some better to do with it...
void database_display_entries(Database db)
{
	char *err_msg;
	char buffer[2048];

	log_message("Entries from database:");

	if (sqlite3_exec(db.getDb(), "select * from entries", Database::callback_debug_log, 0, &err_msg) != SQLITE_OK) {
		snprintf(buffer, sizeof(buffer), "Fail selecting : %s", err_msg);
		log_message(buffer);
	}
}
