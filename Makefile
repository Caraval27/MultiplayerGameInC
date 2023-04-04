MAKEFILE_OS := makefile_windows
ifneq ($(OS),Windows_NT)
	UNAME := $(shell uname)
	ifeq ($(UNAME),Linux)
		MAKEFILE_OS := makefile_linux
	endif
	ifeq ($(UNAME),Darwin)
		MAKEFILE_OS := makefile_mac
	endif
endif

.PHONY: all clean

all:
	$(MAKE) -f $(MAKEFILE_OS)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f game
