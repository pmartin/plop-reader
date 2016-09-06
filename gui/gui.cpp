#include "gui.h"



Gui::Gui()
{

}


Gui::~Gui()
{
	CloseFont(titleFont);
}


// There are things (like opening a font?) it seems we cannot do in the ctor: the ereader is not ready yet...
// So, we need another initialization method, called later -- like when the ereader is ready to let us use its API...
void Gui::init()
{
	titleFont = OpenFont("LiberationSans", titleFontSize, 1);
}


void Gui::show()
{
	SetClip(0, 0, ScreenWidth(), ScreenHeight());
	ClearScreen();

	SetFont(titleFont, BLACK);

	DrawTextRect(0, 0, ScreenWidth(), titleFontSize, "Belladonna", ALIGN_CENTER | VALIGN_TOP);
	PartialUpdate(0, 0, ScreenWidth(), 0 + titleFontSize);

	DrawLine(0, titleFontSize + 7, ScreenWidth(), titleFontSize + 7, BLACK);
	DrawLine(0, titleFontSize + 8, ScreenWidth(), titleFontSize + 8, BLACK);
	PartialUpdate(0, titleFontSize + 10, ScreenWidth(), titleFontSize + 10);

	FullUpdateHQ();
}

