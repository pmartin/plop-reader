#include "belladonna.h"


static int y_log;


void log_reset()
{
	y_log = 80;
	ClearScreen();
	FullUpdateHQ();
}


void log_message(const char *msg)
{
	if (strlen(msg) == 0) {
		return;
	}

	SetFont(font, BLACK);

	DrawTextRect(0, y_log, ScreenWidth(), kFontSize, msg, ALIGN_LEFT);
	PartialUpdate(0, y_log, ScreenWidth(), y_log + kFontSize + 2);
	y_log += kFontSize + 2;
}


