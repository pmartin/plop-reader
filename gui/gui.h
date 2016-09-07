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

	void statusBarText(const char *format, va_list args);
	void statusBarText(const char *format...);

private:
	ifont *titleFont = NULL;
	int titleFontSize = 0;

	ifont *entryTitleFont = NULL;
	int entryTitleFontSize = 0;

	ifont *entryInfosFont = NULL;
	int entryInfosFontSize = 0;

	ifont *statusBarFont = NULL;
	int statusBarFontSize = 0;

	int screenWidth = 0;
	int screenHeight = 0;

};


#endif /* GUI_GUI_H_ */
