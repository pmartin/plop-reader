#ifndef SRC_REPOSITORIES_EPUB_DOWNLOAD_QUEUE_REPOSITORY_H_
#define SRC_REPOSITORIES_EPUB_DOWNLOAD_QUEUE_REPOSITORY_H_


#include <vector>

#include "../database/database.h"

#include "../entities/entry.h"
#include "../entities/epub_download.h"

class EpubDownloadQueueRepository
{
public:
	EpubDownloadQueueRepository(Database &d) : db(d) {}

	//void persist(EpubDownload item);

	void enqueueDownloadForEntry(Entry &entry);

	void listEntryIdsToDownload(std::vector<int> &ids, int limit, int offset);


private:
	Database &db;
};


#endif /* SRC_REPOSITORIES_EPUB_DOWNLOAD_QUEUE_REPOSITORY_H_ */
