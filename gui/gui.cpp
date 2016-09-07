#include "gui.h"

#include "../application.h"
extern Application app;


Gui::Gui()
{

}


Gui::~Gui()
{
	CloseFont(titleFont);
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


void Gui::show(int countAllEntries, std::vector<Entry> entries)
{
	char buffer[2048];

	SetClip(0, 0, screenWidth, screenHeight);
	ClearScreen();

	int y = 0;


	// Chrome : title, button to sync, ...

	SetFont(titleFont, BLACK);

	//FillArea(5, y+5, 50+5, 50+10, WHITE);
	//DrawRect(5, y+5, 50+5, 50+10, DGRAY);
	//DrawSymbol(10, y, ARROW_LEFT);
	exitButton.draw();

	snprintf(buffer, sizeof(buffer), "Belladonna - %d/%d", entries.size(), countAllEntries);
	DrawString(90, y, buffer);

	syncButton.draw();
	menuButton.draw();

	PartialUpdate(0, y, screenWidth, y + titleFont->height);
	y += titleFont->height;

	DrawLine(0, y, screenWidth, y, BLACK);
	DrawLine(0, y + 1, screenWidth, y +1, BLACK);

	PartialUpdate(0, y, screenWidth, y + 1);
	y += 2;


	// List of articles in local DB


	// TODO déplacer tout ça vers une méthode de changement / chargement de page
	entriesItems.clear();
	entriesItems.resize(8);
	for (unsigned int i=0 ; i<entriesItems.size() ; i++) {
		GuiListItemEntry item;

		item.x = 0;
		item.y = y;

		item.titleFont = entryTitleFont;
		item.infosFont = entryInfosFont;

		item.screenWidth = screenWidth;
		item.screenHeight = screenHeight;

		if (i < entries.size()) {
			Entry entry = entries.at(i);
			item.hasEntry = true;
			item.setEntry(entry);
		}
		else {
			item.hasEntry = false;
		}

		entriesItems[i] = item;

		y += item.getHeight();
	}


	for (unsigned int i=0 ; i<entriesItems.size() ; i++) {
		GuiListItemEntry item = entriesItems.at(i);
		item.draw();
	}

	FullUpdateHQ();
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
				if (item.hasEntry) {
					statusBarText("Touch START event at (%d;%d) => entry#%d - %s", x, y, item.entry.id, item.entry.title.c_str());
				}
				else {
					statusBarText("Touch START event at (%d;%d) => no entry", x, y);
				}
			}
		}
	}

	exitButton.draw();
	syncButton.draw();
	menuButton.draw();
}


void Gui::touchEndEvent(int x, int y)
{
	if (exitButton.hit(x, y)) {
		statusBarText("Touch END event at (%d;%d) => exit", x, y);

		CloseApp();
	}
	else if (syncButton.hit(x, y)) {
		statusBarText("Touch END event at (%d;%d) => sync", x, y);

		Message(ICON_INFORMATION, "TODO!", "One day, this will sync posts between this ereader and the server...", 2*1000);
	}
	else if (menuButton.hit(x, y)) {
		statusBarText("Touch END event at (%d;%d) => menu", x, y);

		Message(ICON_INFORMATION, "TODO!", "One day, there will be a menu, here...", 2*1000);
	}
	else {
		for (unsigned int i=0 ; i<entriesItems.size() ; i++) {
			GuiListItemEntry item = entriesItems.at(i);
			if (item.hit(x, y)) {
				if (item.hasEntry) {
					statusBarText("Touch END event at (%d;%d) => entry#%d - %s", x, y, item.entry.id, item.entry.title.c_str());

					char buffer[2048];
					snprintf(buffer, sizeof(buffer), "This should open entry li=%d / ri=%s\n-> %s", item.entry.id, item.entry.remote_id.c_str(), item.entry.title.c_str());
					Message(ICON_INFORMATION, "Opening entry... One day!", buffer, 3*1000);
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

	exitButton.draw();
	syncButton.draw();
	menuButton.draw();
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

	FillArea(0, screenHeight - 40, screenWidth, 40, WHITE);

	SetFont(statusBarFont, DGRAY);
	DrawString(0, screenHeight - 40, buffer);
	PartialUpdate(0, screenHeight - 40, screenWidth, 40);
}

void Gui::statusBarText(const char *format...)
{
	va_list args;

	va_start(args, format);
	statusBarText(format, args);
	va_end(args);
}

