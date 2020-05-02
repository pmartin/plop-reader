#ifndef REPOSITORIES_ENTRY_REPOSITORY_H_
#define REPOSITORIES_ENTRY_REPOSITORY_H_


#include <vector>

#include "../database/database.h"
#include "../entities/entry.h"


class EntryRepository
{
public:
	EntryRepository(Database &d) : db(d) {}

	void persist(Entry entry);

	void deleteAll();

	void list(std::vector<Entry> &entries, int limit, int offset,
			int archived, int starred, int reading_time);

	int countAllEntries();

	int countUnread();
	int countArchived();
	int countStarred();
	int countLong();
	int count(int archived, int starred, int reading_time);

	void listUnread(std::vector<Entry> &entries, int limit, int offset);
	void listArchived(std::vector<Entry> &entries, int limit, int offset);
	void listStarred(std::vector<Entry> &entries, int limit, int offset);
	void listLong(std::vector<Entry> &entries, int limit, int offset);

	Entry get(int entryId);
	Entry findByRemoteId(int remoteId);

	void findUpdatedLocallyMoreRecentlyThanRemotely(std::vector<Entry> &entries);

private:
	Database &db;

};


#endif /* REPOSITORIES_ENTRY_REPOSITORY_H_ */
