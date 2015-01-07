all:mem.so 
#Which complier
CC = gcc	-O2 -fpic 
#CC = gcc	-fpic 
#Where to install
INSTDIR     = 
#Where are include files kept
INCLUDE_BASE = .
#Options for development
CFLAGS = -Werror -Wall -ansi 
#Options for Release
#CFLAGS = -ljson -lpthread -ldl -lm -llua -lfcgame  -Wall -ansi

mem.so:mem.o 
	$(CC)  -shared -o mem.so mem.o  $(CFLAGS)
mem.o:mem.c 
	$(CC) -I$(INCLUDE_BASE)  -c mem.c  -Wall

clean:
	-rm -f  *.o
