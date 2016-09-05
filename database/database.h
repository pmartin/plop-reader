#ifndef DATABASE_DATABASE_H_
#define DATABASE_DATABASE_H_

#include "inkview.h"

#include <string>
#include "sqlite3.h"

extern void log_message(const char *msg);


class Database
{
public:

	~Database();

	void drop(void);
	void open(void);

	sqlite3 *getDb() { return this->db; }

	static int callback_debug_log(void *not_used, int argc, char **argv, char **col_name);

private:
	const char *DB_FILE = USERDATA TEMPDIR "/belladonna.sqlite3";
	sqlite3 *db;


};


#endif /* DATABASE_DATABASE_H_ */
