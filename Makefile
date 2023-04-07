DETECTED_OS := windows
ifneq ($(OS),Windows_NT)
	UNAME := $(shell uname)
	ifeq ($(UNAME),Linux)
		DETECTED_OS := linux
	endif
	ifeq ($(UNAME),Darwin)
		DETECTED_OS := mac
	endif
endif

CC := gcc
EXECUTABLE := game
SRCDIR := src
BUILDDIR := build
CFLAGS := -g -Werror
LDFLAGS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm

ifeq ($(DETECTED_OS),windows)
	LDFLAGS := -lmingw32 -mwindows $(LDFLAGS)
endif

ifeq ($(DETECTED_OS),mac)
	LDFLAGS := $(LDFLAGS) -L /opt/homebrew/lib
endif

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: all clean run

all: $(BUILDDIR)/$(EXECUTABLE)

$(BUILDDIR)/$(EXECUTABLE): $(OBJS) | $(BUILDDIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -f -r $(BUILDDIR)/*

run:
	cd build && ./$(EXECUTABLE)
