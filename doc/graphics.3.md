% GRAPHICS(3) SDL_bgi 3.0.0
% \ 
% November 2022

[1]: # To turn this file to manpage:
[2]: # pandoc -s -t man graphics.3.md -o graphics.3
[3]: # gzip graphics.3

# NAME

graphics.h - compatibility header for SDL_bgi

# SYNOPSIS

**\#include <graphics.h>**

# DESCRIPTION

This header file must be included by C or C++ programs that use
the *SDL_bgi* library. `graphics.h` includes `SDL_bgi.h`, usually
installed in `/usr/include/SDL2`, which contains all definitions and
function prototypes.

*SDL bgi* is a graphics library (GRAPHICS.H) for C, C++, WebAssembly,
and Python. It's based on SDL2 and it's portable on many platforms.

This library strictly emulates BGI functions, making it possible
to compile SDL2 versions of programs written for Turbo C/Borland C++.
ARGB colours, vector fonts, mouse support, and multiple windows are
also implemented; further, native SDL2 functions may be used in
*SDL_bgi* programs.

Python programs can use *SDL_bgi* by means of module `sdl_bgi.py`.

# VARIABLES

```
extern SDL_Window   *bgi_window;
extern SDL_Renderer *bgi_renderer;
extern SDL_Texture  *bgi_texture;
extern Uint32        PALETTE_SIZE;
```

# DEFINITIONS

\ 

## Font definitions

\ 
```
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
```

## Colour definitions

\ 
```
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

enum {
  ARGB_FG_COL   = 16,
  ARGB_BG_COL   = 17,
  ARGB_FILL_COL = 18,
  ARGB_TMP_COL  = 19,
  TMP_COLORS    = 4
};
```

## Drawing and filling styles

\ 
```
enum { NORM_WIDTH = 1, THICK_WIDTH = 3 };

enum { SOLID_LINE, DOTTED_LINE, CENTER_LINE, DASHED_LINE, USERBIT_LINE };

enum { COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT };

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
```

## Mouse buttons

\ 
```
#define WM_LBUTTONDOWN  SDL_BUTTON_LEFT
#define WM_MBUTTONDOWN  SDL_BUTTON_MIDDLE
#define WM_RBUTTONDOWN  SDL_BUTTON_RIGHT
#define WM_WHEEL        SDL_MOUSEWHEEL
#define WM_WHEELUP      SDL_USEREVENT
#define WM_WHEELDOWN    SDL_USEREVENT + 1
#define WM_MOUSEMOVE    SDL_MOUSEMOTION
```

## Key codes

\ 
```
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
```

## Graphics modes

\ 
```
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
```

## Error messages

\ 
```
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
```

## Struct definitions

\ 
```
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
  signed char colors[MAXCOLORS + 1];
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
```

# FUNCTIONS

\ 

## Standard BGI functions

\ 

void *arc* (int x, int y, int stangle, int endangle, int radius);
: Draws a circular arc centered at (*x, y*), with a radius given
  by *radius*, traveling counterclockwise from *stangle* to *endangle*.

void *bar* (int left, int top, int right, int bottom);
: Draws a filled-in rectangle (bar), using the current fill colour and
  fill pattern.

void *bar3d* (int left, int top, int right, int bottom, int depth, int topflag);
: Draws a three-dimensional, filled-in rectangle (bar), using the
  current fill colour and fill pattern.

void *circle* (int x, int y, int radius);
: Draws a circle of the given *radius* at (*x, y*).

void *cleardevice* (void);
: Clears the graphics screen, filling it with the current background
  colour.

void *clearviewport* (void);
:  Clears the viewport, filling it with the current background colour.

void *closegraph* (void);
: Closes the graphics system.

void *delay* (int millisec)
: Waits for *millisec* milliseconds.

void *detectgraph* (int \*graphdriver, int \*graphmode);
: Detects the default graphics driver and graphics mode to use; *SDL*
  and *SDL_FULLSCREEN*, respectively.

void *drawpoly* (int numpoints, int \*polypoints);
: Draws a polygon of *numpoints* vertices.

void *ellipse* (int x, int y, int stangle, int endangle, int xradius, int yradius);
: Draws an elliptical arc centered at (*x*, *y*), with axes given by
  *xradius* and *yradius*, traveling from *stangle* to *endangle*.

void *fillellipse* (int x, int y, int xradius, int yradius);
: Draws an elliptical arc centered at (*x*, *y*), with axes given by
  *xradius* and *yradius*, and fills it using the current fill
  colour and fill pattern.

void *fillpoly* (int numpoints, int \*polypoints);
: Draws a polygon of *numpoints* vertices and fills it using the
  current fill colour.

void *floodfill* (int x, int y, int border);
: Fills an enclosed area, starting from point (*x*, *y*) bounded by
  the *border* colour. The area is filled using the current fill colour.

int  *getactivepage* (void);
: Returns the active page number.

void *getarccoords* (struct arccoordstype \*arccoords);
: Gets the coordinates of the last call to *arc*(), filling the
  *arccoords* structure.

void *getaspectratio* (int \*xasp, int \*yasp);
: Retrieves the current graphics mode's aspect ratio. *xasp* and
  *yasp* are always 10000 (i.e. pixels are square).

int *getbkcolor* (void);
: Returns the current background colour.

int *getch* (void);
: Waits for a key and returns its ASCII or key code.

int *getcolor* (void);
: Returns the current drawing (foreground) colour.

struct palettetype\* *getdefaultpalette* (void);
: Returns the default palette definition structure.

char\* *getdrivername* (void);
: Returns a pointer to a string containing the name of the current
  graphics driver.

void *getfillpattern* (char \*pattern);
: Copies the user-defined fill pattern, as set by *setfillpattern*(),
  into the 8-byte area pointed to by *pattern*.

void *getfillsettings* (struct fillsettingstype \*fillinfo);
: Fills the *fillsettingstype* structure pointed to by
  *fillinfo* with information about the current fill pattern and fill
  colour.

int *getgraphmode* (void);
: Returns the current graphics mode.

void *getimage* (int left, int top, int right, int bottom, void \*bitmap);
: Copies a bit image of the specified region into the memory pointed
  to by *bitmap*.

void *getlinesettings* (struct linesettingstype \*lineinfo);
: Fills the *linesettingstype* structure pointed to by *lineinfo*
  with information about the current line style, pattern, and thickness.

int *getmaxcolor* (void);
: Returns the maximum colour value available (*MAXCOLORS*). If ARGB 
  colours are being used, it returns *PALETTE\_SIZE* (default is 4096).

int *getmaxmode* (void);
: Returns the maximum mode number for the current driver. In *SDL_bgi*,
  the default is *SDL\_FULLSCREEN*. 

int *getmaxx* (void);
: Returns the maximum *x* screen coordinate.

int *getmaxy* (void);
: Returns the maximum *y* screen coordinate.

char\* *getmodename* (int mode_number);
: Returns a pointer to a string containing the name of the specified
  graphics mode.

void *getmoderange* (int graphdriver, int \*lomode, int \*himode);
: Returns the range of valid graphics modes. The *graphdriver* parameter
  is ignored.

void *getpalette* (struct palettetype \*palette);
: Fills the *palettetype* structure pointed to by *palette* with
  information about the current palette size and colours.

int *getpalettesize* (void);
: Returns the size of the palette (*MAXCOLORS* + 1 or
  *MAXRGBCOLORS* + 1).

unsigned int *getpixel* (int x, int y);
: Returns the colour of the pixel located at (*x*, *y*).

void *gettextsettings* (struct textsettingstype \*texttypeinfo);
: Fills the *textsettingstype* structure pointed to by
  *texttypeinfo* with information about the current text font,
  direction, size, and justification.

void *getviewsettings* (struct viewporttype \*viewport);
: Fills the *viewporttype* structure pointed to by *viewport*
  with information about the current viewport.

int  *getvisualpage* (void);
: Returns the visual page number.

int *getx* (void);
: Returns the current CP *x* coordinate, relative to the viewport.

int *gety* (void);
: Returns the current CP *y* coordinate, relative to the viewport.

void *graphdefaults* (void);
: Resets all graphics settings to their default values.

char\* *grapherrormsg* (int errorcode);
: Returns a pointer to the error message string associated with
  *errorcode*, returned by *graphresult*().

int *graphresult* (void);
: Returns the error code for the last unsuccessful graphics operation
  and resets the error level to *grOk*.

unsigned *imagesize* (int left, int top, int right, int bottom);
: Returns the size in bytes of the memory area required to store a bit
  image.

void *initgraph* (int \*graphdriver, int \*graphmode, char \*pathtodriver);
: Initializes the graphics system. Use *initgraph (NULL, NULL, "");* for
  default settings.

int *installuserdriver* (char \*name, int huge (\*detect)(void));
: Unimplemented.

int *installuserfont* (char \*name);
: Loads and installs a *CHR* font from disk. The function returns
  an integer to be used as first argument in *settextstyle*().

int *kbhit* (void);
: Returns 1 when a key is pressed, excluding special keys (Ctrl, Shift,
  etc.)

int *lastkey* (void);
: Returns the last key that was detected by *kbhit*().

void *line* (int x1, int y1, int x2, int y2);
: Draws a line between two specified points.

void *linerel* (int dx, int dy);
: Draws a line from the CP to a point that is (*dx*, *dy*) pixels
  from the CP. The CP is then advanced by (*dx*, *dy*).

void *lineto* (int x, int y);
: Draws a line from the CP to (*x*, *y*), then moves the CP to
  (*dx*, *dy*).

void *moverel* (int dx, int dy);
: Moves the CP by (*dx*, *dy*) pixels.

void *moveto* (int x, int y);
: Moves the CP to the position (*x*, *y*), relative to the
  viewport.

void *outtext* (char \*textstring);
: Outputs *textstring* at the CP.

void *outtextxy* (int x, int y, char \*textstring);
: Outputs *textstring* at (*x*, *y*).

void *pieslice* (int x, int y, int stangle, int endangle, int radius);
: Draws and fills a pie slice centered at (*x*, *y*), with a radius
  given by *radius*, traveling from *stangle* to *endangle*.

void *putimage* (int left, int top, void \*bitmap, int op);
: Puts the bit image pointed to by *bitmap* onto the screen, with the
  upper left corner of the image placed at (*left*, *top*). *op*
  specifies the drawing mode (*COPY_PUT*, etc).

void *putpixel* (int x, int y, int color);
: Plots a pixel at (*x*, *y*) using *color*.

int *random* (int range) (macro)
: Returns an integer random number between 0 and *range - 1*.

void *rectangle* (int left, int top, int right, int bottom);
: Draws a rectangle delimited by (*left*, *top*) and
  (*right*, *bottom*).

int *registerbgidriver* (void (\*driver)(void));
: Unimplemented; not used by *SDL_bgi*.

int *registerbgifont* (void (\*font)(void));
: Unimplemented; not used by *SDL_bgi*.

void *restorecrtmode* (void);
: Hides the graphics window.

void *sector* (int x, int y, int stangle, int endangle, int xradius, int yradius);
: Draws and fills an elliptical pie slice centered at (*x*, *y*),
  horizontal and vertical radii given by *xradius* and *yradius*,
  traveling from *stangle* to *endangle*.

void *setactivepage* (int page);
: Makes *page* the active page for all subsequent graphics output.

void *setallpalette* (struct palettetype \*palette);
: Sets the current palette to the values given in *palette*.

void *setaspectratio* (int xasp, int yasp);
: Changes the default aspect ratio of the graphics.

void *setbkcolor* (int color);
: Sets the current background colour.

void *setcolor* (int color);
: Sets the current drawing colour.

void *setfillpattern* (char \*upattern, int color);
: Sets a user-defined fill pattern.

void *setfillstyle* (int pattern, int color);
: Sets the fill pattern and fill colour.

unsigned *setgraphbufsize* (unsigned bufsize);
: Unimplemented; not used by *SDL_bgi*.

void *setgraphmode* (int mode);
: Shows the window that was hidden by *restorecrtmode*().

void *setlinestyle* (int linestyle, unsigned upattern, int thickness);
: Sets the line width and style.

void *setpalette* (int colornum, int color);
: Changes the standard palette *colornum* to *color*, which can also
  be specified using the *COLOR()* function.

void *settextjustify* (int horiz, int vert);
: Sets text justification. Text output will be justified around the CP
  horizontally and vertically.

void *settextstyle* (int font, int direction, int charsize);
: Sets the text style.

void *setusercharsize* (int multx, int divx, int multy, int divy);
: Lets the user change the character width and height.

void *setviewport* (int left, int top, int right, int bottom, int clip);
: Sets the current viewport for graphics output.

void *setvisualpage* (int page);
: Sets the visual graphics page number to *page*.

void *setwritemode* (int mode);
: Sets the writing mode for line drawing (COPY_PUT, etc.)

int *textheight* (char \*textstring);
: Returns the height in pixels *textstring*.

int *textwidth* (char \*textstring);
: Returns the width in pixels of *textstring*.


## *SDL_bgi* extensions

\ 

int *ALPHA_VALUE* (int color);
: Returns the alpha (transparency) component of *color* in the
  ARGB palette.

int *BLUE_VALUE* (int color);
: Returns the blue component of *color* in the ARGB palette.

void *closewindow* (int id);
: Closes the window identified by *id*.

int *COLOR* (int r, int g, int b);
: Can be used as an argument for *putpixel*(), *setalpha*(),
  *setcolor*(), *setbkcolor*(), *setfillpattern*(), and *setfillstyle*()
  to set a colour specifying its ARGB components.

int *COLOR32* (Uint32 color);
: Can be used as an argument for *putpixel*(), *setalpha*(),
  *setcolor*(), *setbkcolor*(), *setfillpattern*(), and *setfillstyle*()
  to set a colour as ARGB integer.

char \* *colorname* (int color);
: Returns a string containing the colour name.

Uint32 *colorRGB* (int r, int g, int b) (macro)
: Can be used to compose a 32 bit colour with *r* *g* *b*
  components.

void *copysurface* (SDL_Surface \*surface, int x1, int y1, int x2, int y2);
: Copies *surface* to the rectangle defined by *x1*, *y1*, *x2*, *y2*.

int *doubleclick* (void);
: Returns 1 if the last mouse click was a double click.

int *edelay* (int msec);
: Waits for *msec* milliseconds. This function returns 1 if an event
  occurs during the delay, otherwise it returns 0.

int *event* (void);
: Returns 1 if an event (mouse click, key press, or *QUIT*) has
  occurred.

int *eventtype* (void);
: Returns the type of the last event: *SDL_KEYDOWN*, *SDL_MOUSEMOTION*,
  *SDL_MOUSEBUTTONDOWN*, *SDL_MOUSEBUTTONUP*, *SDL_MOUSEWHEEL*, 
  *SDL_QUIT*

void *fputpixel* (int x, int y);
: Plots a point at (*x*, *y*) using the current drawing colour.

void *getbuffer* (Uint32 *buffer);
: Copies the contents of the active window to *buffer*.

int *getclick* (void);
: Waits for a mouse button click and returns its code.

int *getcurrentwindow* (void);
: Returns the *id* of the current window.

int *getevent* (void);
: Waits for a keypress, mouse click, or *SDL_QUIT* event, and returns
  the code of the key, mouse button, or *SDL_QUIT*.

void *getleftclick* (void);
: Waits for the left mouse button to be clicked and released.

void *getlinebuffer* (int y, Uint32 \*linebuffer);
: Copies the *y*-th screen line to *linebuffer*.

int *getmaxheight* (void);
: Returns the maximum possible height for a new window (actual screen
  height in pixels).
  
int *getmaxwidth* (void);
: Returns the maximum possible width for a new window (actual screen
  width in pixels).

void *getmiddleclick* (void);
: Waits for the middle mouse button to be clicked and released.

void *getmouseclick* (int btn, int \*x, int \*y);
: Sets the *x*, *y* coordinates of the last *btn* button click
  expected by *ismouseclick*().

void *getrgbpalette* (struct rgbpalettetype\* palette, int size);
: Fills the *rgbpalettetype* structure pointed to by *palette* with
information about the current ARGB palette's *size* and colours.

int *getrgbpalettesize* (void);
: Returns the current size of the ARGB palette.

void *getrightclick* (void);
: Waits for the right mouse button to be clicked and released.

void *getscreensize* (int \*width, int \*height);
: Reports the screen width and height in *width* and *height*,
  regardless of current window dimensions.

void *getwindowheight* ();
: Equivalent to *getmaxy*() (WinBGIm compatibility).

void *getwindowwidth* ();
: Equivalent to *getmaxx*() (WinBGIm compatibility).

int *GREEN_VALUE* (int color);
: Returns the green component of *color* in the ARGB palette.

void *initpalette* (void);
: Initialises the BGI palette to the standard 16 colours.

int *initwindow* (int width, int height);
: Initializes the graphics system, opening a *width* x *height*
  window. Set *width* or *height* equal to 0 for fullscreen.

int *IS_BGI_COLOR* (int color);
: Returns 1 if the *current* drawing colour is a standard BGI
  colour (that is, not ARGB). The *color* argument is actually redundant.

int *ismouseclick* (int btn);
: Returns 1 if the *btn* mouse button was clicked.

int *IS_RGB_COLOR* (int color);
: Returns 1 if the *current* drawing colour is ARGB. The *color*
  argument is actually redundant.

int *kdelay* (int msec);
: Waits for *msec* milliseconds. This function returns 1 if a key
  is pressed during the delay, otherwise it returns 0.

int *mouseclick* (void);
: Returns the code of the mouse button that is being clicked, or 
  *SDL_MOUSEMOTION*, or 0.

int *mousex* (void);
: Returns the *X* coordinate of the last mouse click.

int *mousey* (void);
: Returns the *Y* coordinate of the last mouse click.

void *putbuffer* (Uint32 \*buffer);
: Copies *buffer* to the current window.

void *putlinebuffer* (int y, Uint32 \*linebuffer);
: Copies *linebuffer* to the *y* coordinate in the current window.

void _putpixel* (int x, int y);
: Same as fputpixel().

void *readimagefile* (char \*filename, int x1, int y1, int x2, int y2);
: Reads a *.bmp* file and displays it immediately at (*x1*, *y1*).

int *RED_VALUE* (int color);
: Returns the red component of *color* in the ARGB palette.

void *refresh* (void);
: Updates the screen contents, i.e. displays all graphics.

void *resetwinoptions* (int id, char \*title, int x, int y);
: Resets title and position of window *id* to *title*, (*x*, *y*).

int  *resizepalette* (Uint32 newsize);
: Resizes the ARGB palette to *newsize*; returns 0 if successful, 1
  otherwise.

int *RGBPALETTE* (int color);
: Can be used as an argument for *putpixel*(), *setalpha*(),
  *setcolor*(), *setbkcolor*(), *setfillpattern*(), and *setfillstyle*()
  to set a colour from the ARGB palette *color* entry.

void *sdlbgiauto* (void);
: Triggers *auto mode*, i.e. *refresh*() is performed
  automatically.

void *sdlbgifast* (void);
: Triggers *fast mode*, i.e. *refresh*() is needed to display
  graphics.

void *sdlbgislow* (void);
: Triggers *slow mode* i.e. *refresh*() is not needed to
  display graphics.

void *setallrgbpalette* (struct rgbpalettetype \*palette);
: Sets the current ARGB palette to the values given in *palette*.

void *setalpha* (int col, Uint8 alpha);
: Sets alpha transparency for colour *col* to *alpha* (0-255); 0
  means full transparecy, 255 full opacity.

void *setbkrgbcolor* (int color);
: Sets the current background colour to the *color* entry in the
  ARGB palette.

void *setblendmode* (int blendmode);
: Sets the blend mode to be used with screen refresh; either
  SDL_BLENDMODE_NONE or SDL_BLENDMODE_BLEND.

void *setcurrentwindow* (int id);
: Sets the current active window to *id*.

void *setrgbcolor* (int color);
: Sets the current drawing colour to the *color* entry in the
  ARGB palette.

void *setrgbpalette* (int colornum, int red, int green, int blue);
: Sets the *colornum* entry in the ARGB palette specifying the *r*,
  *g*, and *b* components.

void *setwinoptions* (char \*title, int x, int y, Uint32 flags);
: Sets the window title *title*, the initial position to (*x*,
  *y*), and SDL2 flags OR'ed together.

void *setwintitle* (int id, char \*title);
: Sets the title of the window identified by *id*.

void *showerrorbox* (const char \*message);
: Opens an error message box with the specified message.

void *showinfobox* (const char \*message);
: Opens an information message box with the specified message.

void *swapbuffers* (void);
: Swaps the current active and the current visual graphics pages.

void *writeimagefile* (char \*filename, int left, int top, int right, int bottom);
: Writes a *.bmp* file from the screen rectangle defined by
  *left*, *top*, *right*, *bottom*.

int *xkbhit* (void);
: Returns 1 when any key is pressed, including special keys (Ctrl, Shift,
  etc.)


# ENVIRONMENT

Native code and Python:

*SDL_BGI_RES*: when set to *VGA*, default resolution will be 640
x 480 instead of default 800 x 600.

*SDL_BGI_RATE*: when set to *auto*, automatic screen refresh
will be performed.

*SDL_BGI_PALETTE*: when set to *BGI*, the first 16 colours will
use the same RGB values as Turbo C 2.01.

Compiling programs with *emscripten*, variables *SDL_BGI_RES* and
*SDL_BGI_PALETTE* can be replaced by files by the same name, possibly
containing the strings *VGA* and *BGI*.

When run in a browser, the user can provide "environment files" in the
assets directory. These text files have the same name as the
corresponding environment variable, and contain the desired value
(e.g. *VGA* for assets/SDL_BGI_RES).


# KNOWN BUGS

- Visualisation problems on NVIDIA GK208BM (GeForce 920M) with 
nvidia-driver-\* on GNU/Linux. As far as I can say, this is an NVIDIA
driver problem.

- On Raspios 10 ARM, the letter 'p' is drawn incorrectly in
SCRIPT_FONT. This bug does not affect Raspios 10 i386.

Please report bugs if you find any.


# EXAMPLE

```
#include <graphics.h>

int main (int argc, char *argv[])
{
  int i, gd = DETECT, gm;
  initgraph (&gd, &gm, "");
  setbkcolor (BLACK);
  cleardevice ();
  outtextxy (0, 0, "Drawing 1000 lines...");
  for (i = 0; i < 1000; i++) {
    setcolor (1 + random (15));
    line (random(getmaxx()), random(getmaxy()),
    random (getmaxx()), random(getmaxy()) );
  }
  getch ();
  closegraph ();
  return 0;
}
```

To compile this  program on GNU/Linux, macOS, or Raspios:

```
$ gcc -o program program.c -lSDL_bgi -lSDL2
```

To compile this program on MSYS2 + mingw-w64:

```
$ gcc -o program.exe program.c -lmingw32 -L/mingw64/bin \
  -lSDL_bgi -lSDL2main -lSDL2 # -mwindows
```

To compile this program to WebAssembly using *emcc*:

```
$ emcc --emrun -o program.html program.c -lSDL_bgi \
    -std=gnu99 -O2 -Wall -lm \
    -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY -s SINGLE_FILE
```

To initialise the graphics, you may try one of the following methods:

```
// open a 400x300 window
initwindow (400, 300);

// open a 400x300 window, setting position, title, and flags
setwinoptions ("SDL_bgi window", 100, 100, SDL_WINDOW_BORDERLESS);
initwindow (400, 300);

// go fullscreen
initwindow (0, 0);

// go fullscreen or VGA if SDL_BGI_RES is set
gd = DETECT;
gm = getmaxmode ();
initgraph(&gd, &gm, "");

// go fullscreen with non native resolution
setwinoptions ("", -1, -1, SDL_WINDOW_FULLSCREEN);
initwindow (800, 600);
```

# AUTHORS

Main library: Guido Gonzato, PhD *guido dot gonzato at gmail dot com*

Automatic refresh patch, CHR font support:
Marco Diego Aurélio Mesquita, *marcodiegomesquita at gmail dot com*

# LICENSE

*SDL_bgi* is released under the ZLib License.
opyright (c) 2014-2022 Guido Gonzato, PhD

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
