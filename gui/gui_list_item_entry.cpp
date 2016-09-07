#include "gui_list_item_entry.h"


void GuiListItemEntry::draw()
{
	char buffer[2048];
	int yy = y;

	if (hasEntry == false) {
		FillArea(x, yy, screenWidth, getHeight(), WHITE);
		PartialUpdate(x, yy, screenWidth, getHeight());
		return;
	}

	SetFont(titleFont, BLACK);
	snprintf(buffer, sizeof(buffer), "%s", entry.title.c_str());
	DrawString(90, yy, buffer);
	PartialUpdate(90, yy, screenWidth, yy + titleFont->height);
	yy += titleFont->height;


	SetFont(infosFont, BLACK);
	snprintf(buffer, sizeof(buffer), "l#=%d ; r#=%s ; la=%d ; ra=%d ; l*=%d ; r*=%d", entry.id, entry.remote_id.c_str(), entry.local_is_archived, entry.remote_is_archived, entry.local_is_starred, entry.remote_is_starred);
	DrawString(90, yy, buffer);
	PartialUpdate(90, yy, screenWidth, yy + infosFont->height);
	yy += infosFont->height;

	SetFont(infosFont, BLACK);
	snprintf(buffer, sizeof(buffer), "%s", entry.url.c_str());
	DrawString(90, yy, buffer);
	PartialUpdate(90, yy, screenWidth, yy + infosFont->height);
	yy += infosFont->height;

	DrawLine(0, yy, screenWidth, yy, LGRAY);
	PartialUpdate(0, yy, screenWidth, yy);
	yy += 2;
}

