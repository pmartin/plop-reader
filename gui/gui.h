#ifndef GUI_GUI_H_
#define GUI_GUI_H_


#include <vector>

#include "inkview.h"

#include "../entities/entry.h"


extern void log_message(const char *msg);


class Gui
{
public:
	Gui();
	~Gui();

	void init();

	void show(int countAllEntries, std::vector<Entry> entries);

private:
	ifont *titleFont = NULL;
	int titleFontSize = 0;

	ifont *entryTitleFont = NULL;
	int entryTitleFontSize = 0;

	ifont *entryInfosFont = NULL;
	int entryInfosFontSize = 0;

};


#endif /* GUI_GUI_H_ */
