PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -std=gnu++11 -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: belladonna.app


belladonna.o: belladonna.h belladonna.cpp
	$(CC) -c belladonna.cpp $(CFLAGS) -o belladonna.o -g -gdwarf-3

config.o: belladonna.h config.cpp
	$(CC) -c config.cpp $(CFLAGS) -o config.o -g -gdwarf-3

log.o: belladonna.h log.cpp
	$(CC) -c log.cpp $(CFLAGS) -o log.o -g -gdwarf-3

oauth.o: belladonna.h oauth.cpp
	$(CC) -c oauth.cpp $(CFLAGS) -o oauth.o -g -gdwarf-3


belladonna.app: belladonna.o config.o log.o oauth.o
	$(CC) belladonna.o config.o log.o oauth.o -o belladonna.app -linkview -lcurl -ljson-c -lsqlite3


clean:
	rm -f *.o
	rm -f belladonna.app
