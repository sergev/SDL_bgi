# Makefile for SDL_bgi
# For GNU/Linux, macOS, and MSYS2 + MinGW-64
# Latest update: October 20, 2022

VERSION := $(shell cat ../VERSION)
NAME    = SDL_bgi
SRC     = $(NAME).c
OBJ     = $(NAME).o
HEADER1 = SDL_bgi.h 
HEADER2 = graphics.h
FONTS   = bold.h euro.h goth.h lcom.h \
          litt.h sans.h scri.h simp.h \
	  trip.h tscr.h
MANPAGE = ../doc/graphics.3.gz

SDLPY   = sdl_bgi.py
PYTHON  := $(shell command -v python3 2> /dev/null)
ifdef PYTHON
  PYUSERSITE := $(shell python -m site --user-site)
  $(shell mkdir -p $(PYUSERSITE))
endif

# Detect the platform: GNU/Linux, macOS, or MSYS2
# unfortunately, macOS doesn't support 'uname -o'
PLATFORM := $(shell uname -s)

# GNU/Linux
ifeq ($(PLATFORM),Linux)
INC_DIR = /usr/include
SDL_INC = $(INC_DIR)/SDL2
LIB_DIR = /usr/lib/
LIB     = lib$(NAME).so
LDFLAGS = -lSDL2
PIC     = -fPIC
STRIP   = strip -x
MANDIR  = /usr/share/man/man3/
endif

# macOS
ifeq ($(PLATFORM),Darwin)
INC_DIR = /usr/local/include
SDL_INC = /usr/local/include/SDL2
LIB_DIR = /usr/local/lib
LIB     = lib$(NAME).so
LDFLAGS = -lSDL2
PIC     = -fPIC
STRIP   = strip -x
MANDIR  = /usr/share/man/man3/
endif

# MSYS2
msys := $(findstring _NT, $(PLATFORM))
ifeq ($(msys),_NT)
PLATFORM = MSYS2
# use native MinGW-64 SDL2 packages
INC_DIR = /mingw64/include
SDL_INC = $(INC_DIR)/SDL2
LIB_DIR = /mingw64/bin
LIB     = $(NAME).dll
LDFLAGS = -L/mingw64/bin -lSDL2
PIC     =
STRIP   = strip
MANDIR  = /mingw64/share/man/man3/
endif

$(info *** Building on $(PLATFORM) ***)
$(info )

# C compiler: tested with gcc, clang, and tcc
# tcc is supported on GNU/Linux only
# CC = tcc -D SDL_DISABLE_IMMINTRIN_H
CC = gcc
# -Wextra
CFLAGS = -O2 -std=gnu99 -g -c -Wall \
	-I $(INC_DIR) -I $(SDL_INC) $(PIC)

.PHONY: all
all: $(LIB)

$(OBJ):	$(SRC) $(FONTS) $(HEADER1)
	$(CC) $(CFLAGS) $(SRC)

$(LIB): $(OBJ)
	$(CC) -shared -o $(LIB) $(OBJ) $(LDFLAGS) ; \
	$(STRIP) $(LIB)

ifdef EMSDK

EMINCLUDE=$(EMSDK)/upstream/emscripten/cache/sysroot/include
EMLIB=$(EMSDK)/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten

wasm: # tested on GNU/Linux only!
	emcc -std=gnu99 -O2 -Wall $(SRC) \
	-s USE_SDL=2 -c -o $(NAME).bc ; \
	emar rcs lib$(NAME).a $(NAME).bc ; \
	/usr/bin/install -m 644 $(HEADER2) $(EMINCLUDE) ; \
	/usr/bin/install -m 644 $(HEADER1) $(EMINCLUDE)/SDL2 ; \
	/usr/bin/install -m 644 lib$(NAME).a $(EMLIB)
unwasm:
	/bin/rm -f $(EMINCLUDE)/$(HEADER2) ; \
	/bin/rm -f $(EMINCLUDE)/SDL2/$(HEADER1) ; \
	/bin/rm -f $(EMLIB)/lib$(NAME).a

endif

install: $(LIB) $(HEADER1)
	/usr/bin/install -m 755 $(LIB) $(LIB_DIR) ; \
	/usr/bin/install -m 644 $(HEADER1) $(SDL_INC) ; \
	/usr/bin/install $(HEADER2) $(INC_DIR) ; \
	/usr/bin/install $(MANPAGE) $(MANDIR)

# remove this entry?
python:  $(SDLPY)
ifdef PYTHON
	/usr/bin/install $(SDLPY) $(PYUSERSITE)
endif

uninstall:
	/bin/rm -f $(SDL_INC)/$(HEADER1) ; \
	/bin/rm -f $(INC_DIR)/$(HEADER2) ; \
	/bin/rm -f $(LIB_DIR)/$(LIB) ; \
	/bin/rm -f $(PYUSERSITE)/$(SDLPY) ; \
	/bin/rm -f $(MANDIR)/$(MANPAGE)

test: all
	cd ../test; make

clean:
	/bin/rm -f $(OBJ) $(LIB) a.out *.bc *.a *~


# --- end of Makefile
