#include "gui.h"



Gui::Gui()
{

}


Gui::~Gui()
{
	CloseFont(titleFont);
	CloseFont(entryTitleFont);
	CloseFont(entryInfosFont);
}


// There are things (like opening a font?) it seems we cannot do in the ctor: the ereader is not ready yet...
// So, we need another initialization method, called later -- like when the ereader is ready to let us use its API...
void Gui::init()
{
	titleFontSize = 52;
	titleFont = OpenFont("LiberationSans", titleFontSize, 1);

	entryTitleFontSize = 32;
	entryTitleFont = OpenFont("LiberationSans", entryTitleFontSize, 1);

	entryInfosFontSize = 24;
	entryInfosFont = OpenFont("LiberationSans", entryInfosFontSize, 1);
}


void Gui::show(int countAllEntries, std::vector<Entry> entries)
{
	char buffer[2048];

	SetClip(0, 0, ScreenWidth(), ScreenHeight());
	ClearScreen();

	int y = 0;


	// Chrome : title, button to sync, ...

	SetFont(titleFont, BLACK);

	DrawRect(5, y+5, 50+5, 50+10, DGRAY);
	DrawSymbol(10, y, ARROW_LEFT);

	snprintf(buffer, sizeof(buffer), "Belladonna - %d/%d", entries.size(), countAllEntries);
	DrawString(90, y, buffer);

	DrawRect(ScreenWidth() - 125 - 10, y+5, 50+5, 50+10, DGRAY);
	DrawSymbol(ScreenWidth() - 125, y, ARROW_UPDOWN);

	DrawRect(ScreenWidth() - 50 - 10, y+5, 50+5, 50+10, DGRAY);
	DrawSymbol(ScreenWidth() - 50, y, SYMBOL_MENU);

	PartialUpdate(0, y, ScreenWidth(), y + titleFont->height);
	y += titleFont->height;

	DrawLine(0, y, ScreenWidth(), y, BLACK);
	DrawLine(0, y + 1, ScreenWidth(), y +1, BLACK);

	PartialUpdate(0, y, ScreenWidth(), y + 1);
	y += 2;


	// List of articles in local DB

	for (unsigned int i=0 ; i<entries.size() ; i++) {
		Entry entry = entries.at(i);

		SetFont(entryTitleFont, BLACK);
		snprintf(buffer, sizeof(buffer), "%s", entry.title.c_str());
		DrawString(90, y, buffer);
		PartialUpdate(90, y, ScreenWidth(), y + entryTitleFont->height);
		y += entryTitleFont->height;

		SetFont(entryInfosFont, BLACK);
		snprintf(buffer, sizeof(buffer), "l#=%d ; r#=%s ; la=%d ; ra=%d ; l*=%d ; r*=%d", entry.id, entry.remote_id.c_str(), entry.local_is_archived, entry.remote_is_archived, entry.local_is_starred, entry.remote_is_starred);
		DrawString(90, y, buffer);
		PartialUpdate(90, y, ScreenWidth(), y + entryInfosFont->height);
		y += entryInfosFont->height;

		SetFont(entryInfosFont, BLACK);
		snprintf(buffer, sizeof(buffer), "%s", entry.url.c_str());
		DrawString(90, y, buffer);
		PartialUpdate(90, y, ScreenWidth(), y + entryInfosFont->height);
		y += entryInfosFont->height;

		DrawLine(0, y, ScreenWidth(), y, LGRAY);
		PartialUpdate(0, y, ScreenWidth(), y);
		y += 2;
	}


	FullUpdateHQ();
}

