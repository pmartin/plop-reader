#include "belladonna.h"

// debugging function, to display what's in the DB,
// until we have some better to do with it...
void database_display_entries(EntryRepository entryRepository)
{
	log_message("Entries from database:");

	std::vector<Entry> entries = entryRepository.list();

	char buffer[2048];
	for (unsigned int i=0 ; i<entries.size() ; i++) {
		Entry entry = entries.at(i);
		snprintf(buffer, sizeof(buffer), "lid=%d ; rid=%s ; title=%s", entry.id, entry.remote_id.c_str(), entry.title.c_str());
		log_message(buffer);
	}


}
