#include "belladonna.h"


ifont *font;
const int kFontSize = 16;


WallabagApi wallabag_api;


static int main_handler(int event_type, int param_one, int param_two)
{
	int result = 0;

	static int step = 0;

	switch (event_type) {
	case EVT_INIT:
		font = OpenFont("LiberationSans", kFontSize, 1);
		SetFont(font, BLACK);
		log_reset();
		ClearScreen();
		FullUpdate();
		break;
	case EVT_SHOW:

		break;
	case EVT_KEYPRESS:
		if (param_one == KEY_PREV) {
			CloseApp();
			return 1;
		}
		else if (param_one == KEY_NEXT) {
			database_drop();
			database_open();

			WallabagConfigLoader configLoader;
			WallabagConfig config = configLoader.load();

			wallabag_api.setConfig(config);
			wallabag_api.createOAuthToken();
			wallabag_api.loadRecentArticles();

			database_display_entries();

			/*
			if (step == 0) {

			}
			else {
				CloseApp();
			}
			//*/

			step++;
			return 1;
		}

		break;
	case EVT_EXIT:
		database_close();

		CloseFont(font);
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
