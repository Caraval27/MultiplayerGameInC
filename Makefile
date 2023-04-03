MAKEFILE_OS := makefile_windows

ifeq ($(OS),Windows_NT)
	MAKEFILE_OS := makefile_windows
else
	UNAME := $(shell uname)
	ifeq ($(UNAME),Linux)
		MAKEFILE_OS := makefile_linux
	endif
	ifeq ($(UNAME),Darwin)
		MAKEFILE_OS := makefile_mac
	endif
endif

all:
	$(MAKE) -f $(MAKEFILE_OS)
