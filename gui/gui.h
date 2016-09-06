#ifndef GUI_GUI_H_
#define GUI_GUI_H_


#include "inkview.h"


extern void log_message(const char *msg);


class Gui
{
public:
	Gui();
	~Gui();

	void init();

	void show();

private:
	ifont *titleFont = NULL;
	const int titleFontSize = 40;

};


#endif /* GUI_GUI_H_ */
