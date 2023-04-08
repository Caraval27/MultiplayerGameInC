RUNTIME_OS := windows
ifneq ($(OS),Windows_NT)
	UNAME := $(shell uname)
	ifeq ($(UNAME),Linux)
		RUNTIME_OS := linux
	endif
	ifeq ($(UNAME),Darwin)
		RUNTIME_OS := mac
	endif
endif

CC := gcc
EXECUTABLE := game
SRCDIR := src
BUILDDIR := build
CFLAGS := -g -Werror
LDFLAGS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm

ifeq ($(RUNTIME_OS),windows)
	LDFLAGS := -lmingw32 -mwindows $(LDFLAGS)
endif

ifeq ($(RUNTIME_OS),mac)
	LDFLAGS := $(LDFLAGS) -L /opt/homebrew/lib
endif

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: all clean run

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
	cd build && ./$(EXECUTABLE)
