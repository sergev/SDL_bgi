// SDL_bgi.h	-*- C -*-

// SDL_bgi is a graphics library (GRAPHICS.H) for C, C++, WebAssembly,
// and Python. It's based on SDL2 and it's portable on many platforms.
// Easy to use, pretty fast, useful for porting old programs
// and for teaching introductory computer graphics.
//
// By Guido Gonzato, PhD
// Automatic refresh patch, CHR font support:
// Marco Diego Aurélio Mesquita
// Latest update: December 12, 2022

// ZLib License

/*

Copyright (c) 2014-2022 Guido Gonzato, PhD

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

#ifndef _SDL_BGI_H
#define _SDL_BGI_H

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

// SDL2 stuff
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <stdio.h>   // for fprintf()
#include <stdint.h>  // for uint* types
#include <stdlib.h>  // for exit(), calloc()
#include <math.h>    // for sin(), cos()
#include <string.h>  // for strlen(), memcpy()

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define SDL_BGI_VERSION 3.0.0

enum { NOPE, YEAH } ;
#define BGI_WINTITLE_LEN 512 // more than enough

// number of concurrent windows that can be created

#define NUM_BGI_WIN 16

// everything gets drawn here

extern SDL_Window   *bgi_window;
extern SDL_Renderer *bgi_renderer;
extern SDL_Texture  *bgi_texture;
// !!! deprecated! Use getpalettesize() instead.
// extern Uint32        PALETTE_SIZE;

// available visual pages

#define VPAGES 4

// BGI fonts

enum {
  DEFAULT_FONT,      // 8x8 bitmap
  TRIPLEX_FONT,      // trip.h
  SMALL_FONT,        // litt.h
  SANS_SERIF_FONT,   // sans.h
  GOTHIC_FONT,       // goth.h
  SCRIPT_FONT,       // scri.h
  SIMPLEX_FONT,      // simp.h
  TRIPLEX_SCR_FONT,  // tscr.h
  COMPLEX_FONT,      // lcom.h
  EUROPEAN_FONT,     // euro.h
  BOLD_FONT,         // bold.h
  LAST_SPEC_FONT
};

enum { HORIZ_DIR, VERT_DIR };

#define USER_CHAR_SIZE 0

enum {
  LEFT_TEXT, CENTER_TEXT, RIGHT_TEXT,
  BOTTOM_TEXT = 0, TOP_TEXT = 2
};

// BGI colours, including CGA and EGA palettes

enum {
  BLACK        = 0,                         EGA_BLACK        = 0,
  BLUE         = 1,                         EGA_BLUE         = 1,
  GREEN        = 2,  CGA_GREEN        = 2,  EGA_GREEN        = 2,
  CYAN         = 3,  CGA_CYAN         = 3,  EGA_CYAN         = 3,
  RED          = 4,  CGA_RED          = 4,  EGA_RED          = 4,
  MAGENTA      = 5,  CGA_MAGENTA      = 5,  EGA_MAGENTA      = 5,
  BROWN        = 6,  CGA_BROWN        = 6,  EGA_BROWN        = 6,
  LIGHTGRAY    = 7,  CGA_LIGHTGRAY    = 7,  EGA_LIGHTGRAY    = 7,
  DARKGRAY     = 8,                         EGA_DARKGRAY     = 8,
  LIGHTBLUE    = 9,                         EGA_LIGHTBLUE    = 9,
  LIGHTGREEN   = 10, CGA_LIGHTGREEN   = 10, EGA_LIGHTGREEN   = 10,
  LIGHTCYAN    = 11, CGA_LIGHTCYAN    = 11, EGA_LIGHTCYAN    = 11,
  LIGHTRED     = 12, CGA_LIGHTRED     = 12, EGA_LIGHTRED     = 12,
  LIGHTMAGENTA = 13, CGA_LIGHTMAGENTA = 13, EGA_LIGHTMAGENTA = 13,
  YELLOW       = 14, CGA_YELLOW       = 14, EGA_YELLOW       = 14,
  WHITE        = 15, CGA_WHITE        = 15, EGA_WHITE        = 15,
  MAXCOLORS    = 15
};

// ARGB colours, set by COLOR (), COLOR32 (), and RGBPALETTE ()

enum {
  ARGB_FG_COL   = 16,
  ARGB_BG_COL   = 17,
  ARGB_FILL_COL = 18,
  ARGB_TMP_COL  = 19,
  TMP_COLORS    = 4
};

// line style, thickness, and drawing mode

enum { NORM_WIDTH = 1, THICK_WIDTH = 3 };

enum { SOLID_LINE, DOTTED_LINE, CENTER_LINE, DASHED_LINE, USERBIT_LINE };

enum { COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT };

// fill styles

enum {
  EMPTY_FILL,       // fills area in background color
  SOLID_FILL,       // fills area in solid fill color
  LINE_FILL,        // --- fill
  LTSLASH_FILL,     // /// fill
  SLASH_FILL,       // /// fill with thick lines
  BKSLASH_FILL,     // \\\ fill with thick lines
  LTBKSLASH_FILL,   // \\\ fill
  HATCH_FILL,       // light hatch fill
  XHATCH_FILL,      // heavy cross hatch fill
  INTERLEAVE_FILL,  // interleaving line fill
  WIDE_DOT_FILL,    // Widely spaced dot fill
  CLOSE_DOT_FILL,   // Closely spaced dot fill
  USER_FILL         // user defined fill
};

// mouse events - compatible with WinBGIm

#define WM_MOUSEMOVE       SDL_MOUSEMOTION

#define WM_LBUTTONDOWN     SDL_BUTTON_LEFT
#define WM_LBUTTONUP       SDL_MOUSEBUTTONUP   + SDL_BUTTON_LEFT
#define WM_LBUTTONDBLCLK   SDL_MOUSEBUTTONDOWN + SDL_BUTTON_LEFT + 2

#define WM_MBUTTONDOWN     SDL_BUTTON_MIDDLE
#define WM_MBUTTONUP       SDL_MOUSEBUTTONUP   + 10*SDL_BUTTON_MIDDLE
#define WM_MBUTTONDBLCLK   SDL_MOUSEBUTTONDOWN + 10*SDL_BUTTON_MIDDLE + 2

#define WM_RBUTTONDOWN     SDL_BUTTON_RIGHT
#define WM_RBUTTONUP       SDL_MOUSEBUTTONUP   + 20*SDL_BUTTON_RIGHT
#define WM_RBUTTONDBLCLK   SDL_MOUSEBUTTONDOWN + 20*SDL_BUTTON_RIGHT + 2

#define WM_WHEEL           SDL_MOUSEWHEEL
#define WM_WHEELUP         SDL_BUTTON_RIGHT + 1
#define WM_WHEELDOWN       SDL_BUTTON_RIGHT + 2

// keys
#define KEY_HOME        SDLK_HOME
#define KEY_LEFT        SDLK_LEFT
#define KEY_UP          SDLK_UP
#define KEY_RIGHT       SDLK_RIGHT
#define KEY_DOWN        SDLK_DOWN
#define KEY_PGUP        SDLK_PAGEUP
#define KEY_PGDN        SDLK_PAGEDOWN
#define KEY_END         SDLK_END
#define KEY_INSERT      SDLK_INSERT
#define KEY_DELETE      SDLK_DELETE
#define KEY_F1          SDLK_F1
#define KEY_F2          SDLK_F2
#define KEY_F3          SDLK_F3
#define KEY_F4          SDLK_F4
#define KEY_F5          SDLK_F5
#define KEY_F6          SDLK_F6
#define KEY_F7          SDLK_F7
#define KEY_F8          SDLK_F8
#define KEY_F9          SDLK_F9
#define KEY_F10         SDLK_F10
#define KEY_F11         SDLK_F11
#define KEY_F12         SDLK_F12
#define KEY_CAPSLOCK    SDLK_CAPSLOCK
#define KEY_LEFT_CTRL   SDLK_LCTRL
#define KEY_RIGHT_CTRL  SDLK_RCTRL
#define KEY_LEFT_SHIFT  SDLK_LSHIFT
#define KEY_RIGHT_SHIFT SDLK_RSHIFT
#define KEY_LEFT_ALT    SDLK_LALT
#define KEY_RIGHT_ALT   SDLK_RALT
#define KEY_ALT_GR      SDLK_MODE
#define KEY_LGUI        SDLK_LGUI
#define KEY_RGUI        SDLK_RGUI
#define KEY_MENU        SDLK_MENU
#define KEY_TAB         SDLK_TAB
#define KEY_BS          SDLK_BACKSPACE
#define KEY_RET         SDLK_RETURN
#define KEY_PAUSE       SDLK_PAUSE
#define KEY_SCR_LOCK    SDLK_SCROLLOCK
#define KEY_ESC         SDLK_ESCAPE

#define QUIT            SDL_QUIT

// graphics modes. Expanded from the original GRAPHICS.H

enum {
  DETECT = -1,
  SDL = 0,
  // all modes @ 320x200
  SDL_320x200 = 1, SDL_CGALO = 1, CGA = 1, CGAC0 = 1, CGAC1 = 1,
  CGAC2 = 1, CGAC3 = 1, MCGAC0 = 1, MCGAC1 = 1, MCGAC2 = 1,
  MCGAC3 = 1, ATT400C0 = 1, ATT400C1 = 1, ATT400C2 = 1, ATT400C3 = 1,
  // all modes @ 640x200
  SDL_640x200 = 2, SDL_CGAHI = 2, CGAHI = 2, MCGAMED = 2,
  EGALO = 2, EGA64LO = 2,
  // all modes @ 640x350
  SDL_640x350 = 3, SDL_EGA = 3, EGA = 3, EGAHI = 3,
  EGA64HI = 3, EGAMONOHI = 3,
  // all modes @ 640x480
  SDL_640x480 = 4, SDL_VGA = 4, VGA = 4, MCGAHI = 4, VGAHI = 4,
  IBM8514LO = 4,
  // all modes @ 720x348
  SDL_720x348 = 5, SDL_HERC = 5,
  // all modes @ 720x350
  SDL_720x350 = 6, SDL_PC3270 = 6, HERCMONOHI = 6,
  // all modes @ 800x600
  SDL_800x600 = 7, SDL_SVGALO = 7, SVGA = 7,
  // all modes @ 1024x768
  SDL_1024x768 = 8, SDL_SVGAMED1 = 8,
  // all modes @ 1152x900
  SDL_1152x900 = 9, SDL_SVGAMED2 = 9,
  // all modes @ 1280x1024
  SDL_1280x1024 = 10, SDL_SVGAHI = 10,
  // all modes @ 1366x768
  SDL_1366x768 = 11, SDL_WXGA = 11,
  // other
  SDL_USER = 12, SDL_FULLSCREEN = 13
};

// error messages
enum graphics_errors {
  grOk               =   0,
  grNoInitGraph      =  -1,
  grNotDetected      =  -2,
  grFileNotFound     =  -3,
  grInvalidDriver    =  -4,
  grNoLoadMem        =  -5,
  grNoScanMem        =  -6,
  grNoFloodMem       =  -7,
  grFontNotFound     =  -8,
  grNoFontMem        =  -9,
  grInvalidMode      = -10,
  grError            = -11,
  grIOerror          = -12,
  grInvalidFont      = -13,
  grInvalidFontNum   = -14,
  grInvalidVersion   = -18
};

// libXbgi compatibility

#define X11_CGALO       SDL_CGALO
#define X11_CGAHI       SDL_CGAHI
#define X11_EGA         SDL_EGA
#define X11             SDL
#define X11_VGA         SDL_VGA
#define X11_640x480     SDL_640x480
#define X11_HERC        SDL_HERC
#define X11_PC3270      SDL_PC3270
#define X11_SVGALO      SDL_SVGALO
#define X11_800x600     SDL_800x600
#define X11_SVGAMED1    SDL_SVGAMED1
#define X11_1024x768    SDL_1024x768
#define X11_SVGAMED2    SDL_SVGAMED2
#define X11_1152x900    SDL_1152x900
#define X11_SVGAHI      SDL_SVGAHI
#define X11_1280x1024   SDL_1280x1024
#define X11_WXGA        SDL_WXGA
#define X11_1366x768    SDL_1366x768
#define X11_USER        SDL_USER
#define X11_FULLSCREEN  SDL_FULLSCREEN

// structs

struct arccoordstype {
  int x;
  int y;
  int xstart;
  int ystart;
  int xend;
  int yend;
};

struct date {
  int da_year;
  int da_day;
  int da_mon;
};

struct fillsettingstype {
  int pattern;
  int color;
};

struct linesettingstype {
  int linestyle;
  unsigned int upattern;
  int thickness;
};

struct palettetype {
  unsigned char size;
  Uint32 colors[MAXCOLORS + 1];
};

// SDL_bgi extension
struct rgbpalettetype {
  Uint32 size;
  Uint32 *colors;
};

struct textsettingstype {
  int font;
  int direction;
  int charsize;
  int horiz;
  int vert;
};

struct viewporttype {
  int left;
  int top;
  int right;
  int bottom;
  int clip;
};

// prototypes - standard BGI
// let's make them C++ compatible

#ifdef __cplusplus
extern "C" {
#endif

void arc (int, int, int, int, int);
void bar3d (int, int, int, int, int, int);
void bar (int, int, int, int);
void circle (int, int, int);
void cleardevice ();
void clearviewport ();
void closegraph (void);
void delay (int);
void detectgraph (int *, int *);
void drawpoly (int, int *);
void ellipse (int, int, int, int, int, int);
void fillellipse (int, int, int, int);
void fillpoly (int, int *);
void floodfill (int, int, int);
int  getactivepage (void);
void getarccoords (struct arccoordstype *);
void getaspectratio (int *, int *);
int  getbkcolor (void);
int  bgi_getch (void);
// for Msys / Mingw64
#define getch bgi_getch
int  getcolor (void);
struct palettetype
     *getdefaultpalette (void);
char *getdrivername (void);
void getfillpattern (char *);
void getfillsettings (struct fillsettingstype *);
int  getgraphmode (void);
void getimage (int, int, int, int, void *);
void getlinesettings (struct linesettingstype *);
int  getmaxcolor (void);
int  getmaxmode (void);
int  getmaxx (void);
int  getmaxy (void);
char *getmodename (int);
void getmoderange (int, int *, int *);
void getpalette (struct palettetype *);
int  getpalettesize (void);
unsigned int
     getpixel (int, int);
void gettextsettings (struct textsettingstype *);
void getviewsettings (struct viewporttype *);
int  getvisualpage (void);
int  getx (void);
int  gety (void);
void graphdefaults ();
char *grapherrormsg (int);
int  graphresult (void);
unsigned
     imagesize (int, int, int, int);
void initgraph (int *, int *, char *);
int  installuserdriver (char *, int (*)(void));
int  installuserfont (char *);
int  bgi_kbhit (void);
// fix for MSYS / Mingw64
#define kbhit k_bhit
int  lastkey (void);
void line (int, int, int, int);
void linerel (int, int);
void lineto (int, int);
void moverel (int, int);
void moveto (int, int);
void outtext (char *);
void outtextxy (int, int, char *);
void pieslice (int, int, int, int, int);
void putimage (int, int, void *, int);
void putpixel (int, int, int);
#define random(range) (rand() % (range))
void rectangle (int, int, int, int);
int  registerbgidriver (void (*)(void));
int  registerbgifont (void (*)(void));
void restorecrtmode (void);
void sector (int, int, int, int, int, int);
void setactivepage (int);
void setallpalette (struct palettetype *);
void setaspectratio (int, int);
void setbkcolor (int);
void setcolor (int);
void setfillpattern (char *, int);
void setfillstyle (int, int);
unsigned
     setgraphbufsize (unsigned);
void setgraphmode (int);
void setlinestyle (int, unsigned, int);
void setpalette (int, int);
void settextjustify (int, int);
void settextstyle (int, int, int);
void setusercharsize (int, int, int, int);
void setviewport (int, int, int, int, int);
void setvisualpage (int);
void setwritemode (int);
int  textheight (char *);
int  textwidth (char *);

// SDL_bgi extensions

int  ALPHA_VALUE (int);
int  BLUE_VALUE (int);
void closewindow (int);
int  COLOR (int, int, int);
int  COLOR32 (Uint32);
char *colorname (int);
#define colorRGB(r,g,b)  0xff000000 | ((r) << 16) | ((g) << 8) | (b)
void copysurface (SDL_Surface *, int, int, int, int);
int doubleclick (void);
int  edelay (int);
int  event (void);
int  eventtype (void);
void fputpixel (int, int);
void getbuffer (Uint32 *);
int  getclick (void);
int  getcurrentwindow (void);
void getleftclick (void);
int  getevent (void);
void getleftclick (void);
void getlinebuffer (int, Uint32 *);
int  getmaxheight (void);
int  getmaxwidth (void);
void getmiddleclick (void);
void getmouseclick (int, int *, int *);
void getrgbpalette (struct rgbpalettetype *, int);
int  getrgbpalettesize (void);
void getrightclick (void);
void getscreensize (int *, int *);
#define getwindowwidth  getmaxx
#define getwindowheight getmaxy
int  GREEN_VALUE (int);
void initpalette (void);
int  initwindow (int, int);
int  IS_BGI_COLOR (int color);
int  ismouseclick (int);
int  IS_RGB_COLOR (int color);
int  kdelay (int);
int  mouseclick (void);
int  mousex (void);
int  mousey (void);
void putbuffer (Uint32 *);
void putlinebuffer (int, Uint32 *);
#define _putpixel fputpixel
int  RED_VALUE (int );
int  RGBPALETTE (int);
void readimagefile (char *, int, int, int, int);
void refresh (void);
void resetwinoptions (int, char *, int, int);
int  resizepalette (Uint32);
void sdlbgiauto (void);
void sdlbgifast (void);
void sdlbgislow (void);
void setallrgbpalette (struct rgbpalettetype *);
void setalpha (int, Uint8);
void setbkrgbcolor (int);
void setblendmode (int);
void setcurrentwindow (int);
void setrgbcolor (int);
void setrgbpalette (int, int, int, int);
void setwinoptions (char *, int, int, Uint32);
void setwintitle (int, char *);
void showinfobox (const char *);
void showerrorbox (const char *);
void swapbuffers (void);
void writeimagefile (char *, int, int, int, int);
int  xkb_hit (void);
// fix for for MSYS / Mingw64 bug
#define xkbhit xkb_hit
  
#ifdef __cplusplus
}
#endif

#endif // __GRAPHICS_H

#endif // _SDL_BGI_H

// --- End of file SDL_bgi.h
