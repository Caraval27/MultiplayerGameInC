CURRENT_OS := windows
ifneq ($(OS),Windows_NT)
	UNAME := $(shell uname)
	ifeq ($(UNAME),Linux)
		CURRENT_OS := linux
	endif
	ifeq ($(UNAME),Darwin)
		CURRENT_OS := mac
	endif
endif

CC := gcc
EXECUTABLE := game
SRCDIR := src
BUILDDIR := build

CFLAGS := -g -Werror
LDFLAGS := -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
LDFLAGS_WINDOWS := -lmingw32 # -mwindows
LDFLAGS_MAC := -L /opt/homebrew/lib

ifeq ($(CURRENT_OS),windows)
	LDFLAGS := $(LDFLAGS_WINDOWS) $(LDFLAGS)
endif

ifeq ($(CURRENT_OS),mac)
	LDFLAGS := $(LDFLAGS_MAC) $(LDFLAGS)
endif

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: all clean run launch

all: $(BUILDDIR)/$(EXECUTABLE)

$(BUILDDIR)/$(EXECUTABLE): $(OBJS) | $(BUILDDIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) -o $@ -c $< $(CFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -f -r $(BUILDDIR)/*

run:
	$(MAKE) all
	cd $(BUILDDIR) && ./$(EXECUTABLE)

launch:
	cd $(BUILDDIR) && ./$(EXECUTABLE)
