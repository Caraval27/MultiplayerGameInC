MAKEFILE_OS = makefile_windows

UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
	MAKEFILE_OS := makefile_linux
endif
ifeq ($(UNAME),Darwin)
	MAKEFILE_OS := makefile_mac
endif

all:
	@$(MAKE) -f $(MAKEFILE_OS)
