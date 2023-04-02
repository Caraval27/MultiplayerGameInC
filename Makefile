# Makefile for Windows
SRCDIR=./source
CC=gcc
INCLUDE = C:\msys64\mingw64\include

CFLAGS = -g -I$(INCLUDE) -c 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -mwindows -lm

ourgame: main.o keybinds.o platforms.o
	$(CC) main.o keybinds.o platforms.o -o ourgame $(LDFLAGS)

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c

keybinds.o: $(SRCDIR)/keybinds.c ./include/keybinds.h
	    $(CC) $(CFLAGS) $(SRCDIR)/keybinds.c

platforms.o: $(SRCDIR)/platforms.c ./include/platforms.h
	    $(CC) $(CFLAGS) $(SRCDIR)/platforms.c

clean:
	rm *.exe
	rm *.o
