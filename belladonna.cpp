#include "belladonna.h"


ifont *font;
const int kFontSize = 16;


Application app;


static int main_handler(int event_type, int param_one, int param_two)
{
	int result = 0;
	iv_mtinfo *touch_info;

	switch (event_type) {
	case EVT_INIT:
		font = OpenFont("LiberationSans", kFontSize, 1);
		SetFont(font, BLACK);
		log_reset();
		//ClearScreen();
		//FullUpdate();

		result = 1;
		break;
	case EVT_SHOW:

		app.init();
		app.show();

		app.debug("Welcome! Touch a post to read it, or use the buttons in the header.");

		result = 1;
		break;
	case EVT_FOREGROUND:
		app.foreground();
		break;
	case EVT_BACKGROUND:
		app.background();
		break;
	case EVT_KEYPRESS:
		app.keypressEvent(param_one);
		result = 1;
		break;
	case EVT_POINTERDOWN:
		touch_info = GetTouchInfo();
		app.touchStartEvent(touch_info->x, touch_info->y);
		result = 1;
		break;
	case EVT_POINTERUP:
		touch_info = GetTouchInfo();
		app.touchEndEvent(touch_info->x, touch_info->y);
		result = 1;
		break;
	case EVT_POINTERMOVE:
		touch_info = GetTouchInfo();
		if (touch_info->active) {
			app.touchStartEvent(touch_info->x, touch_info->y);
		}
		else {
			app.touchEndEvent(touch_info->x, touch_info->y);
		}
		result = 1;
		break;
	case EVT_EXIT:
		CloseFont(font);
		result = 1;
		break;
	default:
		break;
	}

    return result;
}


int main (int argc, char* argv[])
{
    InkViewMain(main_handler);

    return 0;
}
