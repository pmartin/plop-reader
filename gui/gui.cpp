#include "gui.h"

#include "../application.h"


Gui::Gui(Application &aapp) : app_ref(aapp)
{
	mode = Application::entries_mode::MODE_UNREAD;
}


Gui::~Gui()
{
	CloseFont(titleFont);
	CloseFont(smallTitleFont);
	CloseFont(entryTitleFont);
	CloseFont(entryInfosFont);
	CloseFont(statusBarFont);
}


// There are things (like opening a font?) it seems we cannot do in the ctor: the ereader is not ready yet...
// So, we need another initialization method, called later -- like when the ereader is ready to let us use its API...
void Gui::init()
{
	OpenScreen();
	iv_fullscreen();

	// Allows to effectively be full-screen, even when redrawing
	SetCurrentApplicationAttribute(APPLICATION_READER, 1);

	titleFontSize = 52;
	titleFont = OpenFont("LiberationSans", titleFontSize, 1);

	smallTitleFontSize = 28;
	smallTitleFont = OpenFont("LiberationSans", smallTitleFontSize, 1);

	entryTitleFontSize = 32;
	entryTitleFont = OpenFont("LiberationSans", entryTitleFontSize, 1);

	entryInfosFontSize = 24;
	entryInfosFont = OpenFont("LiberationSans", entryInfosFontSize, 1);

	statusBarFontSize = 22;
	statusBarFont = OpenFont("LiberationSans", statusBarFontSize, 1);

	screenWidth = ScreenWidth();
	screenHeight = ScreenHeight();

	exitButton.setCoordinates(5, 5, 55, 60);
	exitButton.setFont(titleFont);
	exitButton.setSymbol(ARROW_LEFT);

	syncButton.setCoordinates(screenWidth-135, 5, 55, 60);
	syncButton.setFont(titleFont);
	syncButton.setSymbol(ARROW_UPDOWN);

	menuButton.setCoordinates(screenWidth-60, 5, 55, 60);
	menuButton.setFont(titleFont);
	menuButton.setSymbol(SYMBOL_MENU);
}


void Gui::show(int numPage, int numberOfPages, int countAllEntries, std::vector<Entry> &entries)
{
	char buffer[2048];

	iv_fullscreen();
	SetCurrentApplicationAttribute(APPLICATION_READER, 1);

	SetClip(0, 0, screenWidth, screenHeight);
	ClearScreen();

	int y = 0;


	// Chrome : title, button to sync, ...

	exitButton.forceRedraw();
	syncButton.forceRedraw();
	menuButton.forceRedraw();

	exitButton.draw(false);
	syncButton.draw(false);
	menuButton.draw(false);

	SetFont(smallTitleFont, BLACK);

	if (mode == 1) {
		snprintf(buffer, sizeof(buffer), "Belladonna - entrées non lues");
	}
	else if (mode == 2) {
		snprintf(buffer, sizeof(buffer), "Belladonna - entrées archivées");
	}
	else if (mode == 3) {
		snprintf(buffer, sizeof(buffer), "Belladonna - entrées favorites");
	}
	else {
		snprintf(buffer, sizeof(buffer), "Belladonna");
	}
	DrawString(90, y, buffer);
	y += 34;

	snprintf(buffer, sizeof(buffer), "Page %d / %d (%d entrées)", numPage, numberOfPages, countAllEntries);
	DrawString(90, y, buffer);
	y += 34;

	y += 4;

	DrawLine(0, y, screenWidth, y, BLACK);
	DrawLine(0, y + 1, screenWidth, y +1, BLACK);
	y += 2;

	PartialUpdate(0, 0, screenWidth, y);


	// List of articles in local DB


	// TODO déplacer tout ça vers une méthode de changement / chargement de page
	entriesItems.clear();
	entriesItems.resize(8);
	for (unsigned int i=0 ; i<entriesItems.size() ; i++) {
		GuiListItemEntry item(entryTitleFont, entryInfosFont);

		item.setCoordinates(0, y);
		if (i < entries.size()) {
			Entry entry = entries.at(i);
			item.setEntry(entry);
		}

		entriesItems[i] = item;

		y += item.getHeight();
	}


	for (unsigned int i=0 ; i<entriesItems.size() ; i++) {
		GuiListItemEntry item = entriesItems.at(i);
		item.draw(false, true);
	}

	//FullUpdateHQ();
}


void Gui::touchStartEvent(int x, int y)
{
	exitButton.setPressed(false);
	syncButton.setPressed(false);
	menuButton.setPressed(false);

	if (exitButton.hit(x, y)) {
		exitButton.setPressed(true);
		statusBarText("Touch START event at (%d;%d) => exit", x, y);
	}
	else if (syncButton.hit(x, y)) {
		syncButton.setPressed(true);
		statusBarText("Touch START event at (%d;%d) => sync", x, y);
	}
	else if (menuButton.hit(x, y)) {
		menuButton.setPressed(true);
		statusBarText("Touch START event at (%d;%d) => menu", x, y);
	}
	else {
		for (unsigned int i=0 ; i<entriesItems.size() ; i++) {
			GuiListItemEntry item = entriesItems.at(i);
			if (item.hit(x, y)) {
				if (item.hasEntry()) {
					statusBarText("Touch START event at (%d;%d) => entry#%d - %s", x, y, item.getEntry().id, item.getEntry().title.c_str());
				}
				else {
					statusBarText("Touch START event at (%d;%d) => no entry", x, y);
				}
			}
		}
	}

	exitButton.draw(true);
	syncButton.draw(true);
	menuButton.draw(true);
}


// This needs to be global, so it can be accessed by the callback function of the menu ;-(
static imenu *menu;

void Gui::displayMainMenu()
{
	const char *str0 = PLOP_APPLICATION_FULLNAME;
	const char *str1 = "Mode = entrées non lues";
	const char *str2 = "Mode = entrées archivées";
	const char *str3 = "Mode = entrées starrées";
	const char *str4 = "A propos";

	menu = (imenu *)calloc(6, sizeof(imenu));

	menu[0].type = 1;
	menu[0].index = 0;
	menu[0].text = (char *)str0;
	menu[0].submenu = &menu[1];

	menu[1].type = 2;
	menu[1].index = 1;
	menu[1].text = (char *)str1;
	menu[1].submenu = &menu[2];

	menu[2].type = 2;
	menu[2].index = 2;
	menu[2].text = (char *)str2;
	menu[2].submenu = &menu[3];

	menu[3].type = 2;
	menu[3].index = 3;
	menu[3].text = (char *)str3;
	menu[3].submenu = &menu[4];

	menu[4].type = 2;
	menu[4].index = 4;
	menu[4].text = (char *)str4;
	menu[4].submenu = NULL;

	auto callback = [](int index) {
		free(menu);

		if (index == 1) {
			app.setMode(Application::MODE_UNREAD);
		}
		else if (index == 2) {
			app.setMode(Application::MODE_ARCHIVED);
		}
		else if (index == 3) {
			app.setMode(Application::MODE_STARRED);
		}
		else if (index == 4) {
			const char *text = PLOP_APPLICATION_FULLNAME " " PLOP_VERSION_STR "\n"
					PLOP_WEBSITE_URL "\n"
					"\n"
					"A Wallabag application for Pocketbook Touch Lux ereaders." "\n"
					"\n"
					"Developed by Pascal MARTIN.\n"
					"@pascal_martin\n"
					"https://blog.pascal-martin.fr\n"
					"\n"
					"Contribute (GPL-3.0):" "\n"
					PLOP_OPENSOURCE_URL;
			DialogSynchro(ICON_INFORMATION, "Belladonna reader", text, "OK", NULL, NULL);
		}
	};

	SetMenuFont(entryTitleFont);
	irect rect = GetMenuRect(menu);
	OpenMenu(menu, 0, (screenWidth-rect.w)/2, (screenHeight-rect.h)/3, callback);
}


void Gui::touchEndEvent(int x, int y)
{
	if (exitButton.hit(x, y)) {
		statusBarText("Touch END event at (%d;%d) => exit", x, y);

		CloseApp();
	}
	else if (syncButton.hit(x, y)) {
		statusBarText("Touch END event at (%d;%d) => sync", x, y);

		app.loadRecentArticles();
	}
	else if (menuButton.hit(x, y)) {
		statusBarText("Touch END event at (%d;%d) => menu", x, y);

		displayMainMenu();
	}
	else {
		for (unsigned int i=0 ; i<entriesItems.size() ; i++) {
			GuiListItemEntry item = entriesItems.at(i);
			if (item.hit(x, y)) {
				if (item.hasEntry()) {
					statusBarText("Touch END event at (%d;%d) => entry#%d - %s", x, y, item.getEntry().id, item.getEntry().title.c_str());

					app.read(item.getEntry());
				}
				else {
					statusBarText("Touch END event at (%d;%d) => no entry", x, y);
				}
			}
		}
	}

	exitButton.setPressed(false);
	syncButton.setPressed(false);
	menuButton.setPressed(false);

	exitButton.draw(true);
	syncButton.draw(true);
	menuButton.draw(true);
}


void Gui::keypressEvent(int key)
{
	if (key == KEY_PREV) {
		statusBarText("Key pressed : %d -> previous", key);
	}
	else if (key == KEY_NEXT) {
		statusBarText("Key pressed : %d -> next", key);
	}
	else if (key == KEY_MENU) {
		statusBarText("Key pressed : %d -> menu", key);
	}
	else if (key == KEY_HOME) {
		statusBarText("Key pressed : %d -> home", key);
	}
	else {
		statusBarText("Key pressed : %d", key);
	}
}


void Gui::statusBarText(const char *format, va_list args)
{
	char buffer[2048];
	vsnprintf(buffer, sizeof(buffer), format, args);

	FillArea(0, screenHeight - 32, screenWidth, 32, WHITE);

	SetFont(statusBarFont, DGRAY);
	DrawString(0, screenHeight - 34, buffer);
	PartialUpdate(0, screenHeight - 32, screenWidth, 32);
}

void Gui::statusBarText(const char *format...)
{
	va_list args;

	va_start(args, format);
	statusBarText(format, args);
	va_end(args);
}



void Gui::openProgressBar(int icon, const char *title, const char *text, int percent, iv_dialoghandler hproc)
{
	OpenProgressbar(icon, title, text, percent, hproc);
}


int Gui::updateProgressBar(const char *text, int percent)
{
	return UpdateProgressbar(text, percent);
}


void Gui::closeProgressBar()
{
	CloseProgressbar();
}

