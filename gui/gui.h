#ifndef GUI_GUI_H_
#define GUI_GUI_H_


#include <vector>

#include "inkview.h"

#include "../entities/entry.h"

#include "gui_button.h"
#include "gui_list_item_entry.h"


class Application;

extern void log_message(const char *msg);


class Gui
{
public:
	Gui(Application &aapp);
	~Gui();

	void init();

	void show(int countAllEntries, std::vector<Entry> &entries);

	void touchStartEvent(int x, int y);
	void touchEndEvent(int x, int y);

	void keypressEvent(int key);

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

	GuiButton exitButton;
	GuiButton syncButton;
	GuiButton menuButton;

	std::vector<GuiListItemEntry> entriesItems;

	Application &app;

};


#endif /* GUI_GUI_H_ */
