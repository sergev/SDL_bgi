# Top level Makefile for SDL_bgi
# Builds the library in src/ and the test programs in test/.
# Test programs are linked against the library in src/, so they can
# be run without installing it first.

TOPDIR  := $(CURDIR)
INCDIR  = build/include

# graphics.h includes <SDL2/SDL_bgi.h>, so stage the headers
# in the same layout they have when installed.
HEADERS = $(INCDIR)/graphics.h $(INCDIR)/SDL2/SDL_bgi.h

BGI_CFLAGS = -I$(TOPDIR)/$(INCDIR)
BGI_LIBS   = -L$(TOPDIR)/src -Wl,-rpath,$(TOPDIR)/src

.PHONY: all lib test install clean

all: test

lib:
	$(MAKE) -C src

test: lib $(HEADERS)
	$(MAKE) -C test BGI_CFLAGS="$(BGI_CFLAGS)" BGI_LIBS="$(BGI_LIBS)"

$(INCDIR)/graphics.h: src/graphics.h
	/usr/bin/install -d $(INCDIR)
	/usr/bin/install -m 644 $< $@

$(INCDIR)/SDL2/SDL_bgi.h: src/SDL_bgi.h
	/usr/bin/install -d $(INCDIR)/SDL2
	/usr/bin/install -m 644 $< $@

install: lib
	$(MAKE) -C src install

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	/bin/rm -rf $(INCDIR)

# --- end of Makefile
