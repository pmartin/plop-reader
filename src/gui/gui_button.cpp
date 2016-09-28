#include "gui_button.h"


void GuiButton::draw(bool updateScreen)
{
	if (mustRedraw == false) {
		return;
	}

	SetFont(font, BLACK);

	FillArea(x, y, w, h, pressed ? LGRAY : WHITE);
	DrawRect(x, y, w, h, pressed ? BLACK : DGRAY);

	if (symbol != -1) {
		DrawSymbol(x+5, y-5, symbol);
	}
	else if (str != NULL) {
		DrawString(x+5, y, str);
	}

	if (updateScreen) {
		PartialUpdate(x, y, x+w, y+h);
	}

	mustRedraw = false;
}
