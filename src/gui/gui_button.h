#ifndef GUI_GUI_BUTTON_H_
#define GUI_GUI_BUTTON_H_


#include "inkview.h"


class GuiButton
{
public:

	void setCoordinates(int xx, int yy, int ww, int hh) {
		mustRedraw = true;
		x = xx;
		y = yy;
		w = ww;
		h = hh;
	}

	void setFont(ifont *ff) {
		mustRedraw = true;
		font = ff;
	}

	void setSymbol(int s) {
		mustRedraw = true;
		symbol = s;
		str = NULL;
	}

	void setString(const char *s) {
		mustRedraw = true;
		str = s;
		symbol = -1;
	}

	bool hit(int xx, int yy) {
		return xx >= x
				&& xx <= x + w
				&& yy >= y
				&& yy <= y + h
		;
	}

	void setPressed(bool p) {
		mustRedraw = true;
		pressed = p;
	}

	void forceRedraw() {
		mustRedraw = true;
	}

	void draw(bool updateScreen);


private:
	bool mustRedraw;

	int x, y;
	int w, h;

	ifont *font;

	int symbol = -1;
	const char *str = NULL;

	bool pressed = false;
};


#endif /* GUI_GUI_BUTTON_H_ */
