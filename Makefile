PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -g -gdwarf-3 -std=gnu++11 -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function
#CFLAGS = -O3 -std=gnu++11 -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function

all: "Plop! reader.app"


src/api/wallabag_api.o: src/main.h src/translate.h src/api/wallabag_api.h src/api/wallabag_api.cpp
	$(CC) -c src/api/wallabag_api.cpp $(CFLAGS) -o src/api/wallabag_api.o

src/api/wallabag_config_loader.o: src/main.h src/api/wallabag_config_loader.h src/api/wallabag_config_loader.cpp
	$(CC) -c src/api/wallabag_config_loader.cpp $(CFLAGS) -o src/api/wallabag_config_loader.o

src/api/wallabag_config.o: src/main.h src/api/wallabag_config.h src/api/wallabag_config.cpp
	$(CC) -c src/api/wallabag_config.cpp $(CFLAGS) -o src/api/wallabag_config.o

src/api/wallabag_entities_factory.o: src/main.h src/api/wallabag_entities_factory.h src/api/wallabag_entities_factory.cpp
	$(CC) -c src/api/wallabag_entities_factory.cpp $(CFLAGS) -o src/api/wallabag_entities_factory.o

src/api/wallabag_oauth_token.o: src/main.h src/api/wallabag_oauth_token.h src/api/wallabag_oauth_token.cpp
	$(CC) -c src/api/wallabag_oauth_token.cpp $(CFLAGS) -o src/api/wallabag_oauth_token.o

src/database/database.o: src/main.h src/database/database.h src/database/database.cpp
	$(CC) -c src/database/database.cpp $(CFLAGS) -o src/database/database.o

src/entities/entry.o: src/main.h src/entities/entry.h src/entities/entry.cpp
	$(CC) -c src/entities/entry.cpp $(CFLAGS) -o src/entities/entry.o

src/entities/epub_download.o: src/main.h src/entities/epub_download.h src/entities/epub_download.cpp
	$(CC) -c src/entities/epub_download.cpp $(CFLAGS) -o src/entities/epub_download.o

src/gui/gui_button.o: src/main.h src/gui/gui_button.h src/gui/gui_button.cpp
	$(CC) -c src/gui/gui_button.cpp $(CFLAGS) -o src/gui/gui_button.o

src/gui/gui_list_item_entry.o: src/main.h src/gui/gui_list_item_entry.h src/gui/gui_list_item_entry.cpp
	$(CC) -c src/gui/gui_list_item_entry.cpp $(CFLAGS) -o src/gui/gui_list_item_entry.o

src/gui/gui.o: src/main.h src/translate.h src/gui/gui.h src/gui/gui.cpp
	$(CC) -c src/gui/gui.cpp $(CFLAGS) -o src/gui/gui.o

src/repositories/entry_repository.o: src/main.h src/repositories/entry_repository.h src/repositories/entry_repository.cpp
	$(CC) -c src/repositories/entry_repository.cpp $(CFLAGS) -o src/repositories/entry_repository.o

src/repositories/epub_download_queue_repository.o: src/main.h src/repositories/epub_download_queue_repository.h src/repositories/epub_download_queue_repository.cpp
	$(CC) -c src/repositories/epub_download_queue_repository.cpp $(CFLAGS) -o src/repositories/epub_download_queue_repository.o

src/application.o: src/application.h src/translate.h src/application.cpp
	$(CC) -c src/application.cpp $(CFLAGS) -o src/application.o

src/main.o: src/main.h src/main.cpp
	$(CC) -c src/main.cpp $(CFLAGS) -o src/main.o

src/log.o: src/main.h src/log.h src/log.cpp
	$(CC) -c src/log.cpp $(CFLAGS) -o src/log.o

src/libs/thpool/thpool.o: src/libs/thpool/thpool.h src/libs/thpool/thpool.c
	$(CC) -c src/libs/thpool/thpool.c $(CFLAGS) -fpermissive -o src/libs/thpool/thpool.o

"Plop! reader.app": src/api/wallabag_api.o src/api/wallabag_config_loader.o src/api/wallabag_config.o src/api/wallabag_entities_factory.o src/api/wallabag_oauth_token.o src/database/database.o src/entities/entry.o src/entities/epub_download.o src/gui/gui_button.o src/gui/gui_list_item_entry.o src/gui/gui.o src/repositories/entry_repository.o src/repositories/epub_download_queue_repository.o src/application.o src/main.o src/log.o src/libs/thpool/thpool.o
	$(CC) src/api/wallabag_api.o src/api/wallabag_config_loader.o src/api/wallabag_config.o src/api/wallabag_entities_factory.o src/api/wallabag_oauth_token.o src/database/database.o src/entities/entry.o src/entities/epub_download.o src/gui/gui_button.o src/gui/gui_list_item_entry.o src/gui/gui.o src/repositories/entry_repository.o src/repositories/epub_download_queue_repository.o src/application.o src/main.o src/log.o src/libs/thpool/thpool.o -o "Plop! reader.app" -linkview -lcurl -ljson-c -lsqlite3 -lpthread


clean:
	rm -f src/*.o
	rm -f src/api/*.o
	rm -f src/database/*.o
	rm -f src/entities/*.o
	rm -f src/gui/*.o
	rm -f src/repositories/*.o
	rm -f src/libs/thpool/*.o
	rm -f plop-reader.app
