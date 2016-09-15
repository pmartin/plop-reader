#include "entry_repository.h"


// TODO a lot of error-handling around database manipulations


void EntryRepository::persist(Entry entry)
{
	//char buffer[2048];
	bool isUpdate = entry.id > 0;

	sqlite3_stmt *stmt;
	const char *tail;

	const char *sqlInsert = R"sql(
insert into entries (
	remote_id, 
    local_is_archived, remote_is_archived, 
    local_is_starred, remote_is_starred, 
	title, url, content, 
	remote_created_at, remote_updated_at,
	local_created_at, local_updated_at,
	reading_time, preview_picture_url,
    local_content_file_html, local_content_file_epub
)
values (
	:remote_id, 
    :local_is_archived, :remote_is_archived, 
    :local_is_starred, :remote_is_starred, 
	:title, :url, :content,
	:remote_created_at, :remote_updated_at,
	:local_created_at, :local_updated_at,
	:reading_time, :preview_picture_url,
    :local_content_file_html, :local_content_file_epub
)
)sql";

	const char *sqlUpdate = R"sql(
update entries
set
    remote_id = :remote_id, 
    local_is_archived = :local_is_archived,
    remote_is_archived = :remote_is_archived, 
    local_is_starred = :local_is_starred,
    remote_is_starred = :remote_is_starred, 
	title = :title, 
    url = :url, 
    content = :content, 
	remote_created_at = :remote_created_at, 
    remote_updated_at = :remote_updated_at, 
    local_created_at = :local_created_at,
    local_updated_at = :local_updated_at,
	reading_time = :reading_time, 
    preview_picture_url = :preview_picture_url,
    local_content_file_html = :local_content_file_html, 
    local_content_file_epub = :local_content_file_epub
where
    local_id = :local_id
)sql";

	if (isUpdate) {
		if (sqlite3_prepare(this->db.getDb(), sqlUpdate, -1, &stmt, &tail) != SQLITE_OK) {
			//snprintf(buffer, sizeof(buffer), "Fail preparing (update): %s", sqlite3_errmsg(this->db.getDb()));
			//log_message(buffer);
		}
		if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":local_id"), entry.id) != SQLITE_OK) {
			//snprintf(buffer, sizeof(buffer), "Fail binding local_id : %s", sqlite3_errmsg(this->db.getDb()));
			//log_message(buffer);
		}
	}
	else {
		if (sqlite3_prepare(this->db.getDb(), sqlInsert, -1, &stmt, &tail) != SQLITE_OK) {
			//snprintf(buffer, sizeof(buffer), "Fail preparing (insert): %s", sqlite3_errmsg(this->db.getDb()));
			//log_message(buffer);
		}
	}

	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":remote_id"), entry.remote_id.c_str(), entry.remote_id.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding remote_id : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":local_is_archived"), entry.local_is_archived) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding local_is_archived : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":remote_is_archived"), entry.remote_is_archived) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding remote_is_archived : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":local_is_starred"), entry.local_is_starred) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding local_is_starred : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":remote_is_starred"), entry.remote_is_starred) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding remote_is_starred : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":title"), entry.title.c_str(), entry.title.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding title : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":url"), entry.url.c_str(), entry.url.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding url : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":content"), entry.content.c_str(), entry.content.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding content : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":local_created_at"), entry.local_created_at) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding local_created_at : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":remote_created_at"), entry.remote_created_at) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding remote_created_at : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":local_updated_at"), entry.local_updated_at) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding local_updated_at : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":remote_updated_at"), entry.remote_updated_at) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding remote_updated_at : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":reading_time"), entry.reading_time) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding reading_time : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (entry.preview_picture_url.length() > 0) {
		if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":preview_picture_url"), entry.preview_picture_url.c_str(), entry.preview_picture_url.length(), SQLITE_STATIC) != SQLITE_OK) {
			//snprintf(buffer, sizeof(buffer), "Fail binding preview_picture_url : %s", sqlite3_errmsg(this->db.getDb()));
			//log_message(buffer);
		}
	}
	else {
		if (sqlite3_bind_null(stmt, sqlite3_bind_parameter_index(stmt, ":preview_picture_url")) != SQLITE_OK) {
			//snprintf(buffer, sizeof(buffer), "Fail binding preview_picture_url : %s", sqlite3_errmsg(this->db.getDb()));
			//log_message(buffer);
		}
	}

	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":local_content_file_html"), entry.local_content_file_html.c_str(), entry.local_content_file_html.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding local_content_file_html : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	if (sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":local_content_file_epub"), entry.local_content_file_epub.c_str(), entry.local_content_file_epub.length(), SQLITE_STATIC) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding local_content_file_epub : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		//snprintf(buffer, sizeof(buffer), "Fail saving : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}
	sqlite3_finalize(stmt);
}


void EntryRepository::deleteAll()
{
	char *err_msg;
	//char buffer[2048];
	const char *sql = "delete from entries";
	if (sqlite3_exec(this->db.getDb(), sql, NULL, 0, &err_msg) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail deleting all from 'entries' table : %s", err_msg);
		//log_message(buffer);
	}
}


void EntryRepository::listUnread(std::vector<Entry> &entries, int limit, int offset)
{
	return list(entries, limit, offset, 0, 1);
}


void EntryRepository::listArchived(std::vector<Entry> &entries, int limit, int offset)
{
	return list(entries, limit, offset, 2, 1);
}


void EntryRepository::listStarred(std::vector<Entry> &entries, int limit, int offset)
{
	return list(entries, limit, offset, 1, 2);
}


void EntryRepository::list(std::vector<Entry> &entries, int limit, int offset,
		int archived, int starred
)
{
	entries.clear();
	entries.reserve(limit + 10);

	std::vector<std::string> conditions;

	if (archived == 0) {
		conditions.push_back("local_is_archived = 0");
	}
	else if (archived == 1) {
		conditions.push_back("(local_is_archived = 0 or local_is_archived = 1)");
	}
	else if (archived == 2) {
		conditions.push_back("local_is_archived = 1");
	}

	if (starred == 0) {
		conditions.push_back("local_is_starred = 0");
	}
	else if (starred == 1) {
		conditions.push_back("(local_is_starred = 0 or local_is_starred = 1)");
	}
	else if (starred == 2) {
		conditions.push_back("local_is_starred = 1");
	}

	const char *sqlTemplate = R"sql(
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
%s
order by remote_created_at desc 
limit :limit 
offset :offset
)sql";

	sqlite3_stmt *stmt;
	const char *tail;
	int result;

	char sql[2048];
	if (conditions.empty()) {
		snprintf(sql, sizeof(sql), sqlTemplate, "");
	}
	else {
		std::string plop = "where ";
		for (unsigned int i=0 ; i<conditions.size() ; i++) {
			plop = plop.append(conditions.at(i));
			if (i + 1 < conditions.size()) {
				plop.append(" and ");
			}
		}
		snprintf(sql, sizeof(sql), sqlTemplate, plop.c_str());
	}


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
		entry.local_created_at = sqlite3_column_int(stmt, 9);
		entry.remote_created_at = sqlite3_column_int(stmt, 10);
		entry.local_updated_at = sqlite3_column_int(stmt, 11);
		entry.remote_updated_at = sqlite3_column_int(stmt, 12);
		entry.reading_time = sqlite3_column_int(stmt, 13);
		entry.preview_picture_url = ((tmp = (const char *)sqlite3_column_text(stmt, 14))) ? tmp : std::string();
		entry.preview_picture_type = sqlite3_column_int(stmt, 15);
		entry.preview_picture_path = ((tmp = (const char *)sqlite3_column_text(stmt, 16))) ? tmp : std::string();
		entry.local_content_file_html = ((tmp = (const char *)sqlite3_column_text(stmt, 17))) ? tmp : std::string();
		entry.local_content_file_epub = ((tmp = (const char *)sqlite3_column_text(stmt, 18))) ? tmp : std::string();

		entries.push_back(entry);
	}
	sqlite3_finalize(stmt);
}


int EntryRepository::countUnread()
{
	return count(0, 1);
}

int EntryRepository::countArchived()
{
	return count(2, 1);
}

int EntryRepository::countStarred()
{
	return count(1, 2);
}


int EntryRepository::count(int archived, int starred)
{
	std::vector<std::string> conditions;

	if (archived == 0) {
		conditions.push_back("local_is_archived = 0");
	}
	else if (archived == 1) {
		conditions.push_back("(local_is_archived = 0 or local_is_archived = 1)");
	}
	else if (archived == 2) {
		conditions.push_back("local_is_archived = 1");
	}

	if (starred == 0) {
		conditions.push_back("local_is_starred = 0");
	}
	else if (starred == 1) {
		conditions.push_back("(local_is_starred = 0 or local_is_starred = 1)");
	}
	else if (starred == 2) {
		conditions.push_back("local_is_starred = 1");
	}

	const char *sqlTemplate = "select count(*) from entries %s";

	char sql[2048];
	if (conditions.empty()) {
		snprintf(sql, sizeof(sql), sqlTemplate, "");
	}
	else {
		std::string plop = "where ";
		for (unsigned int i=0 ; i<conditions.size() ; i++) {
			plop = plop.append(conditions.at(i));
			if (i + 1 < conditions.size()) {
				plop.append(" and ");
			}
		}
		snprintf(sql, sizeof(sql), sqlTemplate, plop.c_str());
	}

	sqlite3_stmt *stmt;
	const char *tail;

	if (sqlite3_prepare(this->db.getDb(), sql, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	int result = -1;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		result = sqlite3_column_int(stmt, 0);
	}

	sqlite3_finalize(stmt);

	return result;
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

	int result = -1;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		result = sqlite3_column_int(stmt, 0);
	}

	sqlite3_finalize(stmt);

	return result;
}


Entry EntryRepository::get(int entryId)
{
	Entry entry;
	entry.id = 0;

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
where local_id = :id
)sql";

	sqlite3_stmt *stmt;
	const char *tail;

	if (sqlite3_prepare(this->db.getDb(), sql, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":id"), entryId) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_step(stmt) == SQLITE_ROW) {
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
		entry.local_created_at = sqlite3_column_int(stmt, 9);
		entry.remote_created_at = sqlite3_column_int(stmt, 10);
		entry.local_updated_at = sqlite3_column_int(stmt, 11);
		entry.remote_updated_at = sqlite3_column_int(stmt, 12);
		entry.reading_time = sqlite3_column_int(stmt, 13);
		entry.preview_picture_url = ((tmp = (const char *)sqlite3_column_text(stmt, 14))) ? tmp : std::string();
		entry.preview_picture_type = sqlite3_column_int(stmt, 15);
		entry.preview_picture_path = ((tmp = (const char *)sqlite3_column_text(stmt, 16))) ? tmp : std::string();
		entry.local_content_file_html = ((tmp = (const char *)sqlite3_column_text(stmt, 17))) ? tmp : std::string();
		entry.local_content_file_epub = ((tmp = (const char *)sqlite3_column_text(stmt, 18))) ? tmp : std::string();
	}
	sqlite3_finalize(stmt);

	return entry;
}


// TODO less copy-paste between get() and findByRemoteId()
Entry EntryRepository::findByRemoteId(int remoteId)
{
	Entry entry;
	entry.id = 0;

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
where remote_id = :id
)sql";

	sqlite3_stmt *stmt;
	const char *tail;

	//char buffer[2048];
	if (sqlite3_prepare(this->db.getDb(), sql, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":id"), remoteId) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail binding : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	if (sqlite3_step(stmt) == SQLITE_ROW) {
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
		entry.local_created_at = sqlite3_column_int(stmt, 9);
		entry.remote_created_at = sqlite3_column_int(stmt, 10);
		entry.local_updated_at = sqlite3_column_int(stmt, 11);
		entry.remote_updated_at = sqlite3_column_int(stmt, 12);
		entry.reading_time = sqlite3_column_int(stmt, 13);
		entry.preview_picture_url = ((tmp = (const char *)sqlite3_column_text(stmt, 14))) ? tmp : std::string();
		entry.preview_picture_type = sqlite3_column_int(stmt, 15);
		entry.preview_picture_path = ((tmp = (const char *)sqlite3_column_text(stmt, 16))) ? tmp : std::string();
		entry.local_content_file_html = ((tmp = (const char *)sqlite3_column_text(stmt, 17))) ? tmp : std::string();
		entry.local_content_file_epub = ((tmp = (const char *)sqlite3_column_text(stmt, 18))) ? tmp : std::string();
	}
	sqlite3_finalize(stmt);

	return entry;
}


// TODO less copy-paste with other methods
// TODO instead of loading all entries to the vector, maybe use a callback, invoked on each entry ?
void EntryRepository::findUpdatedLocallyMoreRecentlyThanRemotely(std::vector<Entry> &entries)
{
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
where
    local_updated_at > remote_updated_at
    and (
        local_is_archived <> remote_is_archived
        or local_is_starred <> remote_is_starred
    )
)sql";

	sqlite3_stmt *stmt;
	const char *tail;

	entries.clear();

	if (sqlite3_prepare(this->db.getDb(), sql, -1, &stmt, &tail) != SQLITE_OK) {
		//snprintf(buffer, sizeof(buffer), "Fail preparing : %s", sqlite3_errmsg(this->db.getDb()));
		//log_message(buffer);
	}

	int result;
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
		entry.local_created_at = sqlite3_column_int(stmt, 9);
		entry.remote_created_at = sqlite3_column_int(stmt, 10);
		entry.local_updated_at = sqlite3_column_int(stmt, 11);
		entry.remote_updated_at = sqlite3_column_int(stmt, 12);
		entry.reading_time = sqlite3_column_int(stmt, 13);
		entry.preview_picture_url = ((tmp = (const char *)sqlite3_column_text(stmt, 14))) ? tmp : std::string();
		entry.preview_picture_type = sqlite3_column_int(stmt, 15);
		entry.preview_picture_path = ((tmp = (const char *)sqlite3_column_text(stmt, 16))) ? tmp : std::string();
		entry.local_content_file_html = ((tmp = (const char *)sqlite3_column_text(stmt, 17))) ? tmp : std::string();
		entry.local_content_file_epub = ((tmp = (const char *)sqlite3_column_text(stmt, 18))) ? tmp : std::string();

		entries.push_back(entry);
	}
	sqlite3_finalize(stmt);
}

