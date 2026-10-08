# Makefile for test commands

# C compiler: gcc, clang, or tcc
# CC = tcc -w -D SDL_DISABLE_IMMINTRIN_H
CC = gcc

# Detect the platform: GNU/Linux, Darwin (macOS), Mingw-w64
PLATFORM := $(shell uname -s)

# GNU/Linux
ifeq ($(PLATFORM),Linux)
CFLAGS = -std=gnu99 -O2 -g -I. -I/usr/include/SDL2 -Wall
LIBS = -lSDL_bgi -lSDL2
endif

# macOS - SDL2 from Homebrew, in /usr/local or /opt/homebrew
ifeq ($(PLATFORM),Darwin)
SDL_PREFIX := $(shell sdl2-config --prefix 2> /dev/null || echo /usr/local)
CFLAGS = -std=gnu99 -O2 -g -I. -I/usr/local/include \
	 -I$(SDL_PREFIX)/include -I$(SDL_PREFIX)/include/SDL2 -Wall
LIBS = -L/usr/local/lib -L$(SDL_PREFIX)/lib -lSDL_bgi -lSDL2 \
       -Wl,-rpath,/usr/local/lib
endif

# Latest MSYS2 + Mingw-w64
msys := $(findstring _NT, $(PLATFORM))
ifeq ($(msys),_NT)
# use MinGW-64 native SDL2 packages
CFLAGS = -std=gnu99 -O2 -g -I. -I/mingw64/include -I/mingw64/include/SDL
# -mconsole = open a console alongside the program
# -mwindows = Windows-only program (no console)
LIBS = -lmingw32 -L/mingw64/bin -lSDL_bgi -lSDL2main -lSDL2 -lm # -mwindows
endif

PROGRAMS = arc bar3d bar circle cleardevice clearviewport closegraph \
	   delay detectgraph drawpoly ellipse fillellipse fillpoly \
	   floodfill getarccoords getaspectratio getbkcolor getcolor \
	   getdefaultpalette getdrivername getfillpattern \
	   getfillsettings getgraphmode getimage getlinesettings \
	   getmaxcolor getmaxmode getmaxx getmaxy getmodename \
	   getmoderange getpalette getpalettesize getpixel \
	   gettextsettings getvisualpage getviewsettings getx gety \
	   graphdefaults grapherrormsg graphresult imagesize initgraph \
	   installuserfont kbhit line linerel lineto moverel moveto \
	   outtext outtextxy pieslice putimage putpixel rectangle \
	   restorecrtmode sector setactivepage setallpalette \
	   setaspectratio setbkcolor setcolor setfillpattern  \
	   setfillstyle setgraphbufsize setgraphmode setlinestyle  \
	   setpalette settextjustify settextstyle setusercharsize \
	   setviewport setvisualpage setwritemode textheight \
	   textwidth

all: $(PROGRAMS)

# BGI_CFLAGS and BGI_LIBS may point to an uninstalled SDL_bgi;
# see the top level Makefile
$(PROGRAMS): %: %.c
	$(CC) $(BGI_CFLAGS) $(CFLAGS) -o $@ $< $(BGI_LIBS) $(LIBS)

clean:
	/bin/rm -rf $(PROGRAMS) *.dSYM

# --- end of Makefile
