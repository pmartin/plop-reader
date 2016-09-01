PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -std=gnu++11 -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: belladonna.app

belladonna.app: belladonna.cpp
	$(CC) belladonna.cpp $(CFLAGS) -o belladonna.app -linkview -g -gdwarf-3

clean:
	rm -f belladonna.app
