MAKEFILE_OS = makefile.win

ifeq ($(OS),Windows_NT)
	MAKEFILE_OS := makefile.win
else
	UNAME := $(shell uname)
	ifeq ($(UNAME),Linux)
		MAKEFILE_OS := makefile.lux
	endif
	ifeq ($(UNAME),Darwin)
		MAKEFILE_OS := makefile.mac
	endif
endif

all:
	@$(MAKE) -f $(MAKEFILE_OS)
