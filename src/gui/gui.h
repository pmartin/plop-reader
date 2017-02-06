#ifndef GUI_GUI_H_
#define GUI_GUI_H_


#include <vector>

#include "inkview.h"

#include "../entities/entry.h"
#include "../defines.h"
#include "../log.h"

#include "gui_button.h"
#include "gui_list_item_entry.h"


class Application;
extern Application app;

extern void log_message(const char *msg);


typedef void (*gui_update_progressbar)(const char *text, int percent, void *context);


class Gui
{
public:
	Gui(Application &aapp);
	~Gui();

	void init();

	void show(int pageNum, int numberOfPages, int countAllEntries, std::vector<Entry> &entries);

	void touchStartEvent(int x, int y);
	void touchEndEvent(int x, int y);
	void touchLong(int x, int y);

	void keypressEvent(int key);

	void statusBarText(const char *format, va_list args);
	void statusBarText(const char *format...);

	void displayMainMenu();
	void displayContextMenuOnEntry(GuiListItemEntry &item, int xTouch, int yTouch);

	void setMode(int m) {
		mode = m;
	}

	void openProgressBar(int icon, const char *title, const char *text, int percent, iv_dialoghandler hproc);
	int updateProgressBar(const char *text, int percent);
	void closeProgressBar();

	static const int SYNC_PROGRESS_PERCENTAGE_ALL_START = 0;

	static const int SYNC_PROGRESS_PERCENTAGE_OAUTH_START = 4;
	static const int SYNC_PROGRESS_PERCENTAGE_OAUTH_END = 7;

	static const int SYNC_PROGRESS_PERCENTAGE_FETCH_SERVER_VERSION_START = 7;
	static const int SYNC_PROGRESS_PERCENTAGE_FETCH_SERVER_VERSION_END = 11;

	static const int SYNC_PROGRESS_PERCENTAGE_DOWN_HTTP_START = 11;
	static const int SYNC_PROGRESS_PERCENTAGE_DOWN_HTTP_END = 20;

	static const int SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_START = 20;
	static const int SYNC_PROGRESS_PERCENTAGE_DOWN_SAVE_END = 50;

	static const int SYNC_PROGRESS_PERCENTAGE_DOWN_FILES_START = 50;
	static const int SYNC_PROGRESS_PERCENTAGE_DOWN_FILES_END = 80;

	static const int SYNC_PROGRESS_PERCENTAGE_UP_START = 80;
	static const int SYNC_PROGRESS_PERCENTAGE_UP_END = 95;

	static const int SYNC_PROGRESS_PERCENTAGE_ALL_DONE = 100;

private:
	ifont *titleFont = NULL;
	int titleFontSize = 0;

	ifont *smallTitleFont = NULL;
	int smallTitleFontSize = 0;

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

	Application &app_ref;

	int mode;

};


#endif /* GUI_GUI_H_ */
