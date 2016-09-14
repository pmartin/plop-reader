#include "gui_list_item_entry.h"


int GuiListItemEntry::getHeight() {
	return titleFont->height + infosFont->height*2 + 2;
}

void GuiListItemEntry::draw(bool clearBeforeDraw, bool updateScreen)
{
	char buffer[2048];
	int yy = y;
	int initialY = y;

	if (clearBeforeDraw) {
		FillArea(x, initialY, screenWidth, getHeight(), WHITE);
	}

	if (_hasEntry == false) {
		if (updateScreen) {
			PartialUpdate(x, initialY, screenWidth, getHeight());
		}
		return;
	}

	SetFont(titleFont, BLACK);
	snprintf(buffer, sizeof(buffer), "%s", entry.title.c_str());
	DrawString(90, yy, buffer);
	yy += titleFont->height;


	SetFont(infosFont, BLACK);
	snprintf(buffer, sizeof(buffer), "l#%d r#%s la%d ra%d l*%d r*%d lu%d ru%d", entry.id, entry.remote_id.c_str(), entry.local_is_archived, entry.remote_is_archived, entry.local_is_starred, entry.remote_is_starred, entry.local_updated_at, entry.remote_updated_at);
	DrawString(90, yy, buffer);
	yy += infosFont->height;

	SetFont(infosFont, BLACK);
	snprintf(buffer, sizeof(buffer), "%s", entry.url.c_str());
	DrawString(90, yy, buffer);
	yy += infosFont->height;

	DrawLine(0, yy, screenWidth, yy, LGRAY);
	yy += 2;

	if (updateScreen) {
		PartialUpdate(x, initialY, screenWidth, getHeight());
	}
}

