#include "gui_button.h"


void GuiButton::draw(bool updateScreen)
{
	if (mustRedraw == false) {
		return;
	}

	FillArea(x, y, w, h, pressed ? LGRAY : WHITE);
	DrawRect(x, y, w, h, pressed ? BLACK : DGRAY);

	if (symbol != -1 || str != NULL) {
		SetFont(font, BLACK);
		if (symbol != -1) {
			DrawSymbol(x+5, y-5, symbol);
		}
		else if (str != NULL) {
			DrawString(x+5, y, str);
		}
	}
	else if (icon != NULL) {

		DrawBitmap(x + (w - icon->width)/2, y + (h - icon->height)/2, icon);
	}

	if (updateScreen) {
		PartialUpdate(x, y, x+w, y+h);
	}

	mustRedraw = false;
}
