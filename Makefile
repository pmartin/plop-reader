PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -g -gdwarf-3 -std=gnu++11 -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: belladonna.app


api/wallabag_api.o: belladonna.h api/wallabag_api.h api/wallabag_api.cpp
	$(CC) -c api/wallabag_api.cpp $(CFLAGS) -o api/wallabag_api.o

api/wallabag_config_loader.o: belladonna.h api/wallabag_config_loader.h api/wallabag_config_loader.cpp
	$(CC) -c api/wallabag_config_loader.cpp $(CFLAGS) -o api/wallabag_config_loader.o

api/wallabag_config.o: belladonna.h api/wallabag_config.h api/wallabag_config.cpp
	$(CC) -c api/wallabag_config.cpp $(CFLAGS) -o api/wallabag_config.o

api/wallabag_entities_factory.o: belladonna.h api/wallabag_entities_factory.h api/wallabag_entities_factory.cpp
	$(CC) -c api/wallabag_entities_factory.cpp $(CFLAGS) -o api/wallabag_entities_factory.o

api/wallabag_oauth_token.o: belladonna.h api/wallabag_oauth_token.h api/wallabag_oauth_token.cpp
	$(CC) -c api/wallabag_oauth_token.cpp $(CFLAGS) -o api/wallabag_oauth_token.o

database/database.o: belladonna.h database/database.h database/database.cpp
	$(CC) -c database/database.cpp $(CFLAGS) -o database/database.o

entities/entry.o: belladonna.h entities/entry.h entities/entry.cpp
	$(CC) -c entities/entry.cpp $(CFLAGS) -o entities/entry.o

repositories/entry_repository.o: belladonna.h repositories/entry_repository.h repositories/entry_repository.cpp
	$(CC) -c repositories/entry_repository.cpp $(CFLAGS) -o repositories/entry_repository.o

belladonna.o: belladonna.h belladonna.cpp
	$(CC) -c belladonna.cpp $(CFLAGS) -o belladonna.o

database.o: belladonna.h database.cpp
	$(CC) -c database.cpp $(CFLAGS) -o database.o

log.o: belladonna.h log.cpp
	$(CC) -c log.cpp $(CFLAGS) -o log.o


belladonna.app: api/wallabag_api.o api/wallabag_config_loader.o api/wallabag_config.o api/wallabag_entities_factory.o api/wallabag_oauth_token.o database/database.o entities/entry.o repositories/entry_repository.o belladonna.o database.o log.o
	$(CC) api/wallabag_api.o api/wallabag_config_loader.o api/wallabag_config.o api/wallabag_entities_factory.o api/wallabag_oauth_token.o database/database.o entities/entry.o repositories/entry_repository.o belladonna.o database.o log.o -o belladonna.app -linkview -lcurl -ljson-c -lsqlite3


clean:
	rm -f *.o
	rm -f api/*.o
	rm -f database/*.o
	rm -f entities/*.o
	rm -f repositories/*.o
	rm -f belladonna.app
