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

	void list(std::vector<Entry> &entries, int limit, int offset);
	int countAllEntries();

	Entry get(int entryId);

private:
	Database &db;

};


#endif /* REPOSITORIES_ENTRY_REPOSITORY_H_ */
