PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -std=gnu++11 -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: belladonna.app

api/wallabag_api.o: belladonna.h api/wallabag_api.h api/wallabag_api.cpp
	$(CC) -c api/wallabag_api.cpp $(CFLAGS) -o api/wallabag_api.o -g -gdwarf-3

api/wallabag_oauth_token.o: belladonna.h api/wallabag_oauth_token.h api/wallabag_oauth_token.cpp
	$(CC) -c api/wallabag_oauth_token.cpp $(CFLAGS) -o api/wallabag_oauth_token.o -g -gdwarf-3

entities/entry.o: belladonna.h entities/entry.h entities/entry.cpp
	$(CC) -c entities/entry.cpp $(CFLAGS) -o entities/entry.o -g -gdwarf-3

belladonna.o: belladonna.h belladonna.cpp
	$(CC) -c belladonna.cpp $(CFLAGS) -o belladonna.o -g -gdwarf-3

config.o: belladonna.h config.cpp
	$(CC) -c config.cpp $(CFLAGS) -o config.o -g -gdwarf-3

database.o: belladonna.h database.cpp
	$(CC) -c database.cpp $(CFLAGS) -o database.o -g -gdwarf-3

log.o: belladonna.h log.cpp
	$(CC) -c log.cpp $(CFLAGS) -o log.o -g -gdwarf-3


belladonna.app: api/wallabag_api.o api/wallabag_oauth_token.o entities/entry.o belladonna.o config.o database.o log.o
	$(CC) api/wallabag_api.o api/wallabag_oauth_token.o entities/entry.o belladonna.o config.o database.o log.o -o belladonna.app -linkview -lcurl -ljson-c -lsqlite3


clean:
	rm -f *.o api/*.o
	rm -f *.o entities/*.o
	rm -f belladonna.app
