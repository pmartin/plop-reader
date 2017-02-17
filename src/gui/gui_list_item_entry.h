#ifndef GUI_GUI_LIST_ITEM_ENTRY_H_
#define GUI_GUI_LIST_ITEM_ENTRY_H_


#include "inkview.h"
#include <math.h>


#include "../entities/entry.h"
#include "../defines.h"


class GuiListItemEntry
{
public:
	GuiListItemEntry() : x(0), y(0), _hasEntry(false), screenWidth(0), screenHeight(0), titleFont(NULL), infosFont(NULL) {
		GuiListItemEntry(NULL, NULL);
	}

	GuiListItemEntry(ifont *ttitleFont, ifont *iinfosFont) {
		_hasEntry = false;

		screenWidth = ScreenWidth();
		screenHeight = ScreenHeight();

		titleFont = ttitleFont;
		infosFont = iinfosFont;

		x = y = 0;
	}

	void setCoordinates(int xx, int yy) {
		x = xx;
		y = yy;
	}

	void draw(bool clearBeforeDraw, bool mustUpdateScreen, bool highlight = false);
	void updateScreen();

	void setEntry(Entry &e) {
		_hasEntry = true;
		entry = e;
	}

	Entry &getEntry() {
		return entry;
	}

	bool hasEntry() {
		return _hasEntry;
	}

	static int getHeight() {
		return 114;
	}

	bool hit(int xx, int yy) {
		return xx >= x
				&& xx <= x + screenWidth
				&& yy >= y
				&& yy <= y + getHeight()
		;
	}

	void setSmallClock(ibitmap *bmp) {
		smallClock = bmp;
	}

	void setSmallEvent(ibitmap *bmp) {
		smallEvent = bmp;
	}

	void setSmallEpub(ibitmap *bmp) {
		smallEpub = bmp;
	}

	void setSmallHtml(ibitmap *bmp) {
		smallHtml = bmp;
	}

	void setSmallStarred(ibitmap *bmp) {
		smallStarred = bmp;
	}

	int x, y;

private:
	bool _hasEntry;
	Entry entry;

	int screenWidth, screenHeight;

	ifont *titleFont;
	ifont *infosFont;

	ibitmap *smallClock, *smallEvent;
	ibitmap *smallEpub, *smallHtml;
	ibitmap *smallStarred;
};


#endif /* GUI_GUI_LIST_ITEM_ENTRY_H_ */
