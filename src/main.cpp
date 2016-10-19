#include "main.h"


Application app;


static int main_handler(int event_type, int param_one, int param_two)
{
	int result = 0;
	iv_mtinfo *touch_info;

	switch (event_type) {
	case EVT_INIT:
		//log_init();
		result = 1;
		break;
	case EVT_SHOW:

		try {
			app.init();
		}
		catch (const AbortApplication &e) {
			ERROR("Fatal Exception: %s => aborting application!", e.what());
			DialogSynchro(ICON_ERROR, PLOP_APPLICATION_FULLNAME, e.what(), "Close the application", NULL, NULL);

			app.deinit();
			result = 1;
		}
		app.show();

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
	case EVT_POINTERLONG:
		touch_info = GetTouchInfo();
		app.touchLong(touch_info->x, touch_info->y);
		result = 1;
		break;
	case EVT_EXIT:
		//log_close();

		app.deinit();

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
