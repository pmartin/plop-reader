#include "epub_download_queue_repository.h"


/*
void EpubDownloadQueueRepository::persist(EpubDownload item)
{

}
*/


void EpubDownloadQueueRepository::enqueueDownloadForEntry(Entry &entry)
{
	// TODO il ne faut enqueuer une entrée pour téléchargement que si elle n'est pas déjà en attente de téléchargement

	//const char *sqlSelect = "select * from epub_download_queue where entry_id = :entry_id";
	const char *sqlInsert = "insert into epub_download_queue (entry_id, downloading_at, downloaded_at, created_at) values (:entry_id, NULL, NULL, strftime('%s','now'))";
	//const char *sqlUpdate = "update epub_download_queue set downloading_at = :downloading_at, downloaded_at= :downloaded_at where entry_id = :entry_id";

	sqlite3_stmt *stmt;
	const char *tail;

	if (sqlite3_prepare(this->db.getDb(), sqlInsert, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing (insert): %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":entry_id"), entry.id) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding entry_id : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		//snprintf(buffer, sizeof(buffer), "Fail saving : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	sqlite3_finalize(stmt);
}


void EpubDownloadQueueRepository::listEntryIdsToDownload(std::vector<int> &ids, int limit, int offset)
{
	const char *sql = "select entry_id from epub_download_queue where downloading_at is NULL and downloaded_at is NULL order by created_at limit :limit offset :offset";

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
		int entry_id = sqlite3_column_int(stmt, 0);
		ids.push_back(entry_id);
	}

	sqlite3_finalize(stmt);
}
