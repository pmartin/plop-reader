#include "belladonna.h"

// debugging function, to display what's in the DB,
// until we have some better to do with it...
void database_display_entries(EntryRepository entryRepository)
{
	log_message("Entries from database:");

	entryRepository.list();

}
