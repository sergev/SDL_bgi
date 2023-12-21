// SDL_bgi.c        -*- C -*-

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

#include "SDL_bgi.h"

// Fonts

#include "trip.h"    // TRIPLEX_FONT
#include "litt.h"    // SMALL_FONT
#include "sans.h"    // SANS_SERIF_FONT
#include "goth.h"    // GOTHIC_FONT
#include "scri.h"    // SCRIPT_FONT
#include "simp.h"    // SIMPLEX_FONT
#include "tscr.h"    // TRIPLEX_SCR_FONT
#include "lcom.h"    // COMPLEX_FONT
#include "euro.h"    // EUROPEAN_FONT
#include "bold.h"    // BOLD_FONT

// stuff gets drawn here; these variables are available to the
// programmer. All the rest is hidden.

SDL_Window
  *bgi_window;
SDL_Renderer
  *bgi_renderer;
SDL_Texture
  *bgi_texture;

// ARGB palette initial size; it can be resized using resizepalette().

// static global variables. I am not afraid of global variables.

static SDL_Window
  *bgi_win[NUM_BGI_WIN];
static SDL_Renderer
  *bgi_rnd[NUM_BGI_WIN];
static SDL_Texture
  *bgi_txt[NUM_BGI_WIN];

// !!! compatibility! This variable was public in previous
// releases - very dumb idea.
static Uint32
  PALETTE_SIZE = 4096;

// multiple windows

static int
  bgi_active_windows[NUM_BGI_WIN]; // window IDs

// explanation: up to NUM_BGI_WIN windows can be created and deleted
// as needed. 'bgi_active_windows[]' keeps track of created (1) and
// closed (0) windows; 'bgi_current_window' is the ID of the
// current (= being drawn on) window; 'bgi_num_windows' keeps track
//  of the current number of windows

static SDL_Surface
  *bgi_vpage[VPAGES]; // array of visual pages; single window only

// Note: 'Uint32' and 'int' are the same on modern machines

// pixel data of active and visual pages

static Uint32
  *bgi_activepage[NUM_BGI_WIN]; // active (= being drawn on) page;
                                // may be hidden
static Uint32
  *bgi_visualpage[NUM_BGI_WIN]; // visualised page

// This is how we draw stuff on the screen. Pixels pointed to by
// bgi_activepage (a pointer to pixel data in the active surface)
// are modified by functions like putpixel_copy(); bgi_texture is
// updated with the new bgi_activepage contents; bgi_texture is then
// copied to bgi_renderer, and finally bgi_renderer is made present.

// The palette contains the BGI colors, entries 0:MAXCOLORS;
// then four entries for temporary fg, bg, fill, and ARGB color
// allocated with COLOR(); then user-defined ARGB colors

#define BGI_COLORS  MAXCOLORS + 1

// this is the global palette, containing all colours:
// size is BGI_COLORS + TMP_COLORS + PALETTE_SIZE

static Uint32
  *bgi_argb_palette;

// default 16-colour palette; values can't be changed.
// RGB colour definitions are taken from https://www.colorhexa.com/
static const Uint32
  bgi_std_palette[BGI_COLORS] = { // 0 - 15
    0xff000000, // BLACK
    0xff0000ff, // BLUE
    0xff00ff00, // GREEN
    0xff00ffff, // CYAN
    0xffff0000, // RED
    0xffff00ff, // MAGENTA
    0xffa52a2a, // BROWN
    0xffd3d3d3, // LIGHTGRAY
    0xffa9a9a9, // DARKGRAY
    0xffadd8e6, // LIGHTBLUE
    0xff90ee90, // LIGHTGREEN
    0xffe0ffff, // LIGHTCYAN
    0xfff08080, // LIGHTRED
    0xffdb7093, // LIGHTMAGENTA
    0xffffff00, // YELLOW
    0xffffffff  // WHITE
  };

// original Borland RGB colour definitions, picked up from a screenshot
// of Turbo C 2.01 running in DosBox. Values can't be changed.
static const Uint32
  bgi_orig_palette[BGI_COLORS] = { // 0 - 15
    0xff000000, // BLACK
    0xff0000aa, // BLUE
    0xff00aa00, // GREEN
    0xff00aaaa, // CYAN
    0xffaa0000, // RED
    0xffaa00aa, // MAGENTA
    0xffaa5500, // BROWN
    0xffaaaaaa, // LIGHTGRAY
    0xff555555, // DARKGRAY
    0xff5555ff, // LIGHTBLUE
    0xff55ff55, // LIGHTGREEN
    0xff55ffff, // LIGHTCYAN
    0xffff5555, // LIGHTRED
    0xffdb7093, // LIGHTMAGENTA
    0xffffff55, // YELLOW
    0xffffffff  // WHITE
  };

static Uint16
  bgi_line_patterns[1 + USERBIT_LINE] = {
    0xffff,  // SOLID_LINE  = 1111111111111111
    0xcccc,  // DOTTED_LINE = 1100110011001100
    0xf1f8,  // CENTER_LINE = 1111000111111000
    0xf8f8,  // DASHED_LINE = 1111100011111000
    0xffff   // USERBIT_LINE
  };

static Uint32
  bgi_window_flags = 0;      // window flags

static int
  bgi_current_window = -1,   // id of current window
  bgi_num_windows = 0,       // number of created windows
  bgi_window_x =             // window initial position
    SDL_WINDOWPOS_CENTERED,
  bgi_window_y =
    SDL_WINDOWPOS_CENTERED,
  bgi_fg_color = WHITE,      // index of BGI foreground color
  bgi_bg_color = BLACK,      // index of BGI background color
  bgi_fill_color = WHITE,    // index of BGI fill color
  bgi_font = DEFAULT_FONT,   // default font
  bgi_bitmap_font_width = 8, // font width and font height
  bgi_bitmap_font_height = 8,
  bgi_last_event = 0,        // mouse click, keyboard event, or QUIT
  bgi_cp_x = 0,              // current position
  bgi_cp_y = 0,
  bgi_maxx,                  // screen size
  bgi_maxy,
  bgi_gm,                    // graphics mode
  bgi_writemode,             // plotting method (COPY_PUT, XOR_PUT...)
  bgi_blendmode =
    SDL_BLENDMODE_BLEND,     // blending mode
  bgi_ap,                    // active page number
  bgi_vp,                    // visual page number
  bgi_np = 0,                // number of actual pages
  bgi_refresh_rate = 0,      // window refresh rate
  bgi_error_code =
  grNoInitGraph,             // graphics error code
  bgi_last_key_pressed = 0,  // last key pressed
  bgi_internal_font_height,  // font height
  bgi_internal_offset,       // offset for outtextxy()
  bgi_internal_font_desc;

// internal fonts

// pointer to current font
static const char
  **bgi_internal_font;

// pointer to current array of font size
static const int
  *bgi_internal_font_size;

// pointer to current array of width
static char
  *bgi_internal_font_width;

#define NUM_FONTS  11
#define FONT_SIZES 11

static float
  bgi_font_magnification[NUM_FONTS][FONT_SIZES];

static float
  bgi_user_size_x = 1.0,
  bgi_user_size_y = 1.0;

// mouse structure
struct {
  int x;        // coordinates of last mouse click
  int y;
  Uint8 btn;    // button clicked
  int clicks;   // -1=release, 0=no click, 1 or 2 = single or double click
  int wheel;    // 1=away from user, -1=towards the user
} bgi_mouse;

// BGI window title
static char
  bgi_win_title[BGI_WINTITLE_LEN] = "SDL_bgi";

// booleans
SDL_bool
  bgi_fast_mode        = SDL_TRUE,   // needs screen update?
  bgi_auto_mode        = SDL_FALSE,  // automatic refresh?
  bgi_argb_mode        = SDL_FALSE,  // BGI or ARGB colors?
  bgi_window_is_hidden = SDL_FALSE,  // is window hidden?
  bgi_key_pressed      = SDL_FALSE,  // any key pressed?
  bgi_xkey_pressed     = SDL_FALSE,  // any ext. key pressed?
  bgi_refresh_needed   = SDL_FALSE,  // is refresh needed?
  bgi_use_newpalette   = SDL_TRUE,   // use the new palette?
  bgi_use_tmp_color    = SDL_FALSE;  // use temporary colour?

static float
  bgi_font_mag_x = 1.0,  // font magnification
  bgi_font_mag_y = 1.0;

// pointer to 8x8 font array, dumped from DOSBox

static unsigned char bgi_bitmap_font[8 * 256] = {

  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //  0 0x00
  0x7e,0x81,0xa5,0x81,0xbd,0x99,0x81,0x7e, //  1 0x01
  0x7e,0xff,0xdb,0xff,0xc3,0xe7,0xff,0x7e, //  2 0x02
  0x6c,0xfe,0xfe,0xfe,0x7c,0x38,0x10,0x00, //  3 0x03
  0x10,0x38,0x7c,0xfe,0x7c,0x38,0x10,0x00, //  4 0x04
  0x38,0x7c,0x38,0xfe,0xfe,0x7c,0x38,0x7c, //  5 0x05
  0x10,0x10,0x38,0x7c,0xfe,0x7c,0x38,0x7c, //  6 0x06
  0x00,0x00,0x18,0x3c,0x3c,0x18,0x00,0x00, //  7 0x07
  0xff,0xff,0xe7,0xc3,0xc3,0xe7,0xff,0xff, //  8 0x08
  0x00,0x3c,0x66,0x42,0x42,0x66,0x3c,0x00, //  9 0x09
  0xff,0xc3,0x99,0xbd,0xbd,0x99,0xc3,0xff, // 10 0x0a
  0x0f,0x07,0x0f,0x7d,0xcc,0xcc,0xcc,0x78, // 11 0x0b
  0x3c,0x66,0x66,0x66,0x3c,0x18,0x7e,0x18, // 12 0x0c
  0x3f,0x33,0x3f,0x30,0x30,0x70,0xf0,0xe0, // 13 0x0d
  0x7f,0x63,0x7f,0x63,0x63,0x67,0xe6,0xc0, // 14 0x0e
  0x99,0x5a,0x3c,0xe7,0xe7,0x3c,0x5a,0x99, // 15 0x0f
  0x80,0xe0,0xf8,0xfe,0xf8,0xe0,0x80,0x00, // 16 0x10
  0x02,0x0e,0x3e,0xfe,0x3e,0x0e,0x02,0x00, // 17 0x11
  0x18,0x3c,0x7e,0x18,0x18,0x7e,0x3c,0x18, // 18 0x12
  0x66,0x66,0x66,0x66,0x66,0x00,0x66,0x00, // 19 0x13
  0x7f,0xdb,0xdb,0x7b,0x1b,0x1b,0x1b,0x00, // 20 0x14
  0x3e,0x63,0x38,0x6c,0x6c,0x38,0xcc,0x78, // 21 0x15
  0x00,0x00,0x00,0x00,0x7e,0x7e,0x7e,0x00, // 22 0x16
  0x18,0x3c,0x7e,0x18,0x7e,0x3c,0x18,0xff, // 23 0x17
  0x18,0x3c,0x7e,0x18,0x18,0x18,0x18,0x00, // 24 0x18
  0x18,0x18,0x18,0x18,0x7e,0x3c,0x18,0x00, // 25 0x19
  0x00,0x18,0x0c,0xfe,0x0c,0x18,0x00,0x00, // 26 0x1a
  0x00,0x30,0x60,0xfe,0x60,0x30,0x00,0x00, // 27 0x1b
  0x00,0x00,0xc0,0xc0,0xc0,0xfe,0x00,0x00, // 28 0x1c
  0x00,0x24,0x66,0xff,0x66,0x24,0x00,0x00, // 29 0x1d
  0x00,0x18,0x3c,0x7e,0xff,0xff,0x00,0x00, // 30 0x1e
  0x00,0xff,0xff,0x7e,0x3c,0x18,0x00,0x00, // 31 0x1f
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 32 0x20 ' '
  0x30,0x78,0x78,0x30,0x30,0x00,0x30,0x00, // 33 0x21 '!'
  0x6c,0x6c,0x6c,0x00,0x00,0x00,0x00,0x00, // 34 0x22 '"'
  0x6c,0x6c,0xfe,0x6c,0xfe,0x6c,0x6c,0x00, // 35 0x23 '#'
  0x30,0x7c,0xc0,0x78,0x0c,0xf8,0x30,0x00, // 36 0x24 '$'
  0x00,0xc6,0xcc,0x18,0x30,0x66,0xc6,0x00, // 37 0x25 '%'
  0x38,0x6c,0x38,0x76,0xdc,0xcc,0x76,0x00, // 38 0x26 '&'
  0x60,0x60,0xc0,0x00,0x00,0x00,0x00,0x00, // 39 0x27 '''
  0x18,0x30,0x60,0x60,0x60,0x30,0x18,0x00, // 40 0x28 '('
  0x60,0x30,0x18,0x18,0x18,0x30,0x60,0x00, // 41 0x29 ')'
  0x00,0x66,0x3c,0xff,0x3c,0x66,0x00,0x00, // 42 0x2a '*'
  0x00,0x30,0x30,0xfc,0x30,0x30,0x00,0x00, // 43 0x2b '+'
  0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x60, // 44 0x2c ','
  0x00,0x00,0x00,0xfc,0x00,0x00,0x00,0x00, // 45 0x2d '-'
  0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00, // 46 0x2e '.'
  0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00, // 47 0x2f '/'
  0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00, // 48 0x30 '0'
  0x30,0x70,0x30,0x30,0x30,0x30,0xfc,0x00, // 49 0x31 '1'
  0x78,0xcc,0x0c,0x38,0x60,0xcc,0xfc,0x00, // 50 0x32 '2'
  0x78,0xcc,0x0c,0x38,0x0c,0xcc,0x78,0x00, // 51 0x33 '3'
  0x1c,0x3c,0x6c,0xcc,0xfe,0x0c,0x1e,0x00, // 52 0x34 '4'
  0xfc,0xc0,0xf8,0x0c,0x0c,0xcc,0x78,0x00, // 53 0x35 '5'
  0x38,0x60,0xc0,0xf8,0xcc,0xcc,0x78,0x00, // 54 0x36 '6'
  0xfc,0xcc,0x0c,0x18,0x30,0x30,0x30,0x00, // 55 0x37 '7'
  0x78,0xcc,0xcc,0x78,0xcc,0xcc,0x78,0x00, // 56 0x38 '8'
  0x78,0xcc,0xcc,0x7c,0x0c,0x18,0x70,0x00, // 57 0x39 '9'
  0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00, // 58 0x3a ':'
  0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x60, // 59 0x3b ';'
  0x18,0x30,0x60,0xc0,0x60,0x30,0x18,0x00, // 60 0x3c '<'
  0x00,0x00,0xfc,0x00,0x00,0xfc,0x00,0x00, // 61 0x3d '='
  0x60,0x30,0x18,0x0c,0x18,0x30,0x60,0x00, // 62 0x3e '>'
  0x78,0xcc,0x0c,0x18,0x30,0x00,0x30,0x00, // 63 0x3f '?'
  0x7c,0xc6,0xde,0xde,0xde,0xc0,0x78,0x00, // 64 0x40 '@'
  0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0x00, // 65 0x41 'A'
  0xfc,0xc6,0xc6,0xfc,0xc6,0xc6,0xfc,0x00, // 66 0x42 'B'
  0x7c,0xc6,0xc6,0xc0,0xc0,0xc6,0x7c,0x00, // 67 0x43 'C'
  0xf8,0xcc,0xc6,0xc6,0xc6,0xcc,0xf8,0x00, // 68 0x44 'D'
  0xfe,0xc0,0xc0,0xfc,0xc0,0xc0,0xfe,0x00, // 69 0x45 'E'
  0xfe,0xc0,0xc0,0xfc,0xc0,0xc0,0xc0,0x00, // 70 0x46 'F'
  0x7c,0xc6,0xc0,0xce,0xc6,0xc6,0x7e,0x00, // 71 0x47 'G'
  0xc6,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0x00, // 72 0x48 'H'
  0x78,0x30,0x30,0x30,0x30,0x30,0x78,0x00, // 73 0x49 'I'
  0x1e,0x06,0x06,0x06,0xc6,0xc6,0x7c,0x00, // 74 0x4a 'J'
  0xc6,0xcc,0xd8,0xf0,0xd8,0xcc,0xc6,0x00, // 75 0x4b 'K'
  0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xfe,0x00, // 76 0x4c 'L'
  0xc6,0xee,0xfe,0xd6,0xc6,0xc6,0xc6,0x00, // 77 0x4d 'M'
  0xc6,0xe6,0xf6,0xde,0xce,0xc6,0xc6,0x00, // 78 0x4e 'N'
  0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00, // 79 0x4f 'O'
  0xfc,0xc6,0xc6,0xfc,0xc0,0xc0,0xc0,0x00, // 80 0x50 'P'
  0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x06, // 81 0x51 'Q'
  0xfc,0xc6,0xc6,0xfc,0xc6,0xc6,0xc6,0x00, // 82 0x52 'R'
  0x78,0xcc,0x60,0x30,0x18,0xcc,0x78,0x00, // 83 0x53 'S'
  0xfc,0x30,0x30,0x30,0x30,0x30,0x30,0x00, // 84 0x54 'T'
  0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00, // 85 0x55 'U'
  0xc6,0xc6,0xc6,0xc6,0xc6,0x6c,0x38,0x00, // 86 0x56 'V'
  0xc6,0xc6,0xc6,0xd6,0xfe,0xee,0xc6,0x00, // 87 0x57 'W'
  0xc6,0xc6,0x6c,0x38,0x6c,0xc6,0xc6,0x00, // 88 0x58 'X'
  0xc3,0xc3,0x66,0x3c,0x18,0x18,0x18,0x00, // 89 0x59 'Y'
  0xfe,0x0c,0x18,0x30,0x60,0xc0,0xfe,0x00, // 90 0x5a 'Z'
  0x3c,0x30,0x30,0x30,0x30,0x30,0x3c,0x00, // 91 0x5b '['
  0xc0,0x60,0x30,0x18,0x0c,0x06,0x03,0x00, // 92 0x5c '\'
  0x3c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3c,0x00, // 93 0x5d ']'
  0x00,0x38,0x6c,0xc6,0x00,0x00,0x00,0x00, // 94 0x5e '^'
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff, // 95 0x5f '_'
  0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00, // 96 0x60 '`'
  0x00,0x00,0x7c,0x06,0x7e,0xc6,0x7e,0x00, // 97 0x61 'a'
  0xc0,0xc0,0xfc,0xc6,0xc6,0xe6,0xdc,0x00, // 98 0x62 'b'
  0x00,0x00,0x7c,0xc6,0xc0,0xc0,0x7e,0x00, // 99 0x63 'c'
  0x06,0x06,0x7e,0xc6,0xc6,0xce,0x76,0x00, // 100 0x64 'd'
  0x00,0x00,0x7c,0xc6,0xfe,0xc0,0x7e,0x00, // 101 0x65 'e'
  0x1e,0x30,0x7c,0x30,0x30,0x30,0x30,0x00, // 102 0x66 'f'
  0x00,0x00,0x7e,0xc6,0xce,0x76,0x06,0x7c, // 103 0x67 'g'
  0xc0,0xc0,0xfc,0xc6,0xc6,0xc6,0xc6,0x00, // 104 0x68 'h'
  0x18,0x00,0x38,0x18,0x18,0x18,0x3c,0x00, // 105 0x69 'i'
  0x18,0x00,0x38,0x18,0x18,0x18,0x18,0xf0, // 106 0x6a 'j'
  0xc0,0xc0,0xcc,0xd8,0xf0,0xd8,0xcc,0x00, // 107 0x6b 'k'
  0x38,0x18,0x18,0x18,0x18,0x18,0x3c,0x00, // 108 0x6c 'l'
  0x00,0x00,0xcc,0xfe,0xd6,0xc6,0xc6,0x00, // 109 0x6d 'm'
  0x00,0x00,0xfc,0xc6,0xc6,0xc6,0xc6,0x00, // 110 0x6e 'n'
  0x00,0x00,0x7c,0xc6,0xc6,0xc6,0x7c,0x00, // 111 0x6f 'o'
  0x00,0x00,0xfc,0xc6,0xc6,0xe6,0xdc,0xc0, // 112 0x70 'p'
  0x00,0x00,0x7e,0xc6,0xc6,0xce,0x76,0x06, // 113 0x71 'q'
  0x00,0x00,0x6e,0x70,0x60,0x60,0x60,0x00, // 114 0x72 'r'
  0x00,0x00,0x7c,0xc0,0x7c,0x06,0xfc,0x00, // 115 0x73 's'
  0x30,0x30,0x7c,0x30,0x30,0x30,0x1c,0x00, // 116 0x74 't'
  0x00,0x00,0xc6,0xc6,0xc6,0xc6,0x7e,0x00, // 117 0x75 'u'
  0x00,0x00,0xc6,0xc6,0xc6,0x6c,0x38,0x00, // 118 0x76 'v'
  0x00,0x00,0xc6,0xc6,0xd6,0xfe,0x6c,0x00, // 119 0x77 'w'
  0x00,0x00,0xc6,0x6c,0x38,0x6c,0xc6,0x00, // 120 0x78 'x'
  0x00,0x00,0xc6,0xc6,0xce,0x76,0x06,0x7c, // 121 0x79 'y'
  0x00,0x00,0xfc,0x18,0x30,0x60,0xfc,0x00, // 122 0x7a 'z'
  0x1c,0x30,0x30,0xe0,0x30,0x30,0x1c,0x00, // 123 0x7b '{'
  0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00, // 124 0x7c '|'
  0xe0,0x30,0x30,0x1c,0x30,0x30,0xe0,0x00, // 125 0x7d '}'
  0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00, // 126 0x7e '~'
  0x00,0x10,0x38,0x6c,0xc6,0xc6,0xfe,0x00, // 127 0x7f
  0x3c,0x66,0xc0,0xc0,0x66,0x3c,0x18,0x70, // 128 0x80
  0x00,0xcc,0x00,0xcc,0xcc,0xcc,0xcc,0x76, // 129 0x81
  0x0c,0x18,0x00,0x7c,0xc6,0xfe,0xc0,0x7c, // 130 0x82
  0x38,0x6c,0x00,0x78,0x0c,0x7c,0xcc,0x76, // 131 0x83
  0x00,0xcc,0x00,0x78,0x0c,0x7c,0xcc,0x76, // 132 0x84
  0x60,0x30,0x00,0x78,0x0c,0x7c,0xcc,0x76, // 133 0x85
  0x38,0x6c,0x38,0x78,0x0c,0x7c,0xcc,0x76, // 134 0x86
  0x00,0x7c,0xc6,0xc0,0xc6,0x7c,0x18,0x70, // 135 0x87
  0x38,0x6c,0x00,0x7c,0xc6,0xfe,0xc0,0x7c, // 136 0x88
  0x00,0xc6,0x00,0x7c,0xc6,0xfe,0xc0,0x7c, // 137 0x89
  0x30,0x18,0x00,0x7c,0xc6,0xfe,0xc0,0x7c, // 138 0x8a
  0x00,0x66,0x00,0x38,0x18,0x18,0x18,0x3c, // 139 0x8b
  0x38,0x6c,0x00,0x38,0x18,0x18,0x18,0x3c, // 140 0x8c
  0x30,0x18,0x00,0x38,0x18,0x18,0x18,0x3c, // 141 0x8d
  0xc6,0x10,0x38,0x6c,0xc6,0xfe,0xc6,0xc6, // 142 0x8e
  0x38,0x6c,0x38,0x7c,0xc6,0xfe,0xc6,0xc6, // 143 0x8f
  0x0c,0x18,0xfe,0xc0,0xf8,0xc0,0xc0,0xfe, // 144 0x90
  0x00,0x00,0x00,0xec,0x36,0x7e,0xd8,0x6e, // 145 0x91
  0x00,0x3e,0x6c,0xcc,0xfe,0xcc,0xcc,0xce, // 146 0x92
  0x38,0x6c,0x00,0x7c,0xc6,0xc6,0xc6,0x7c, // 147 0x93
  0x00,0xc6,0x00,0x7c,0xc6,0xc6,0xc6,0x7c, // 148 0x94
  0x30,0x18,0x00,0x7c,0xc6,0xc6,0xc6,0x7c, // 149 0x95
  0x78,0xcc,0x00,0xcc,0xcc,0xcc,0xcc,0x76, // 150 0x96
  0x60,0x30,0x00,0xcc,0xcc,0xcc,0xcc,0x76, // 151 0x97
  0x00,0xc6,0x00,0xc6,0xc6,0x7e,0x06,0xfc, // 152 0x98
  0xc6,0x00,0x38,0x6c,0xc6,0xc6,0x6c,0x38, // 153 0x99
  0xc6,0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c, // 154 0x9a
  0x00,0x02,0x7c,0xce,0xd6,0xe6,0x7c,0x80, // 155 0x9b
  0x38,0x6c,0x64,0xf0,0x60,0x60,0x66,0xfc, // 156 0x9c
  0x00,0x3a,0x6c,0xce,0xd6,0xe6,0x6c,0xb8, // 157 0x9d
  0x00,0x00,0xc6,0x6c,0x38,0x6c,0xc6,0x00, // 158 0x9e
  0x0e,0x1b,0x18,0x3c,0x18,0x18,0xd8,0x70, // 159 0x9f
  0x18,0x30,0x00,0x78,0x0c,0x7c,0xcc,0x76, // 160 0xa0
  0x0c,0x18,0x00,0x38,0x18,0x18,0x18,0x3c, // 161 0xa1
  0x0c,0x18,0x00,0x7c,0xc6,0xc6,0xc6,0x7c, // 162 0xa2
  0x18,0x30,0x00,0xcc,0xcc,0xcc,0xcc,0x76, // 163 0xa3
  0x76,0xdc,0x00,0xdc,0x66,0x66,0x66,0x66, // 164 0xa4
  0x76,0xdc,0x00,0xe6,0xf6,0xde,0xce,0xc6, // 165 0xa5
  0x00,0x3c,0x6c,0x6c,0x36,0x00,0x7e,0x00, // 166 0xa6
  0x00,0x38,0x6c,0x6c,0x38,0x00,0x7c,0x00, // 167 0xa7
  0x00,0x30,0x00,0x30,0x30,0x60,0xc6,0x7c, // 168 0xa8
  0x7c,0x82,0xb2,0xaa,0xb2,0xaa,0x82,0x7c, // 169 0xa9
  0x00,0x00,0x00,0xfe,0x06,0x06,0x00,0x00, // 170 0xaa
  0x63,0xe6,0x6c,0x7e,0x33,0x66,0xcc,0x0f, // 171 0xab
  0x63,0xe6,0x6c,0x7a,0x36,0x6a,0xdf,0x06, // 172 0xac
  0x00,0x18,0x00,0x18,0x18,0x3c,0x3c,0x18, // 173 0xad
  0x00,0x00,0x00,0x33,0x66,0xcc,0x66,0x33, // 174 0xae
  0x00,0x00,0x00,0xcc,0x66,0x33,0x66,0xcc, // 175 0xaf
  0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88, // 176 0xb0
  0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa, // 177 0xb1
  0x77,0xdd,0x77,0xdd,0x77,0xdd,0x77,0xdd, // 178 0xb2
  0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18, // 179 0xb3
  0x18,0x18,0x18,0x18,0xf8,0x18,0x18,0x18, // 180 0xb4
  0x0c,0x18,0x38,0x6c,0xc6,0xfe,0xc6,0xc6, // 181 0xb5
  0x7c,0xc6,0x38,0x6c,0xc6,0xfe,0xc6,0xc6, // 182 0xb6
  0x60,0x30,0x38,0x6c,0xc6,0xfe,0xc6,0xc6, // 183 0xb7
  0x7c,0x82,0x9a,0xa2,0xa2,0x9a,0x82,0x7c, // 184 0xb8
  0x36,0x36,0xf6,0x06,0xf6,0x36,0x36,0x36, // 185 0xb9
  0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36, // 186 0xba
  0x00,0x00,0xfe,0x06,0xf6,0x36,0x36,0x36, // 187 0xbb
  0x36,0x36,0xf6,0x06,0xfe,0x00,0x00,0x00, // 188 0xbc
  0x18,0x18,0x7e,0xc0,0xc0,0x7e,0x18,0x18, // 189 0xbd
  0x66,0x66,0x3c,0x7e,0x18,0x7e,0x18,0x18, // 190 0xbe
  0x00,0x00,0x00,0x00,0xf8,0x18,0x18,0x18, // 191 0xbf
  0x18,0x18,0x18,0x18,0x1f,0x00,0x00,0x00, // 192 0xc0
  0x18,0x18,0x18,0x18,0xff,0x00,0x00,0x00, // 193 0xc1
  0x00,0x00,0x00,0x00,0xff,0x18,0x18,0x18, // 194 0xc2
  0x18,0x18,0x18,0x18,0x1f,0x18,0x18,0x18, // 195 0xc3
  0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00, // 196 0xc4
  0x18,0x18,0x18,0x18,0xff,0x18,0x18,0x18, // 197 0xc5
  0x76,0xdc,0x00,0x78,0x0c,0x7c,0xcc,0x76, // 198 0xc6
  0x76,0xdc,0x38,0x6c,0xc6,0xfe,0xc6,0xc6, // 199 0xc7
  0x36,0x36,0x37,0x30,0x3f,0x00,0x00,0x00, // 200 0xc8
  0x00,0x00,0x3f,0x30,0x37,0x36,0x36,0x36, // 201 0xc9
  0x36,0x36,0xf7,0x00,0xff,0x00,0x00,0x00, // 202 0xca
  0x00,0x00,0xff,0x00,0xf7,0x36,0x36,0x36, // 203 0xcb
  0x36,0x36,0x37,0x30,0x37,0x36,0x36,0x36, // 204 0xcc
  0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00, // 205 0xcd
  0x36,0x36,0xf7,0x00,0xf7,0x36,0x36,0x36, // 206 0xce
  0x00,0x00,0xc6,0x7c,0xc6,0xc6,0x7c,0xc6, // 207 0xcf
  0x76,0x18,0x6c,0x06,0x3e,0x66,0x66,0x3c, // 208 0xd0
  0x00,0xf8,0x6c,0x66,0xf6,0x66,0x6c,0xf8, // 209 0xd1
  0x38,0x6c,0x00,0xfe,0xc0,0xf8,0xc0,0xfe, // 210 0xd2
  0xc6,0x00,0xfe,0xc0,0xf8,0xc0,0xc0,0xfe, // 211 0xd3
  0x30,0x18,0xfe,0xc0,0xf8,0xc0,0xc0,0xfe, // 212 0xd4
  0x7c,0xc2,0xf8,0xc0,0xf0,0xc0,0xc2,0x7c, // 213 0xd5
  0x0c,0x18,0x3c,0x18,0x18,0x18,0x18,0x3c, // 214 0xd6
  0x3c,0x66,0x00,0x3c,0x18,0x18,0x18,0x3c, // 215 0xd7
  0x66,0x00,0x3c,0x18,0x18,0x18,0x18,0x3c, // 216 0xd8
  0x18,0x18,0x18,0x18,0xf8,0x00,0x00,0x00, // 217 0xd9
  0x00,0x00,0x00,0x00,0x1f,0x18,0x18,0x18, // 218 0xda
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 219 0xdb
  0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff, // 220 0xdc
  0x18,0x18,0x18,0x00,0x00,0x18,0x18,0x18, // 221 0xdd
  0x30,0x18,0x3c,0x18,0x18,0x18,0x18,0x3c, // 222 0xde
  0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00, // 223 0xdf
  0x0c,0x18,0x38,0x6c,0xc6,0xc6,0x6c,0x38, // 224 0xe0
  0x00,0x78,0xcc,0xd8,0xcc,0xc6,0xc6,0xcc, // 225 0xe1
  0x7c,0xc6,0x38,0x6c,0xc6,0xc6,0x6c,0x38, // 226 0xe2
  0x60,0x30,0x38,0x6c,0xc6,0xc6,0x6c,0x38, // 227 0xe3
  0x76,0xdc,0x00,0x7c,0xc6,0xc6,0xc6,0x7c, // 228 0xe4
  0x76,0xdc,0x38,0x6c,0xc6,0xc6,0x6c,0x38, // 229 0xe5
  0x00,0x00,0x00,0x66,0x66,0x66,0x7c,0xc0, // 230 0xe6
  0xe0,0x60,0x7c,0x66,0x66,0x7c,0x60,0xf0, // 231 0xe7
  0x00,0xf0,0x60,0x7c,0x66,0x7c,0x60,0xf0, // 232 0xe8
  0x0c,0x18,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c, // 233 0xe9
  0x38,0x6c,0x00,0xc6,0xc6,0xc6,0xc6,0x7c, // 234 0xea
  0x30,0x18,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c, // 235 0xeb
  0x0c,0x18,0x00,0xc6,0xc6,0x7e,0x06,0xfc, // 236 0xec
  0x0c,0x18,0x66,0x66,0x3c,0x18,0x18,0x3c, // 237 0xed
  0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 238 0xee
  0x0c,0x18,0x00,0x00,0x00,0x00,0x00,0x00, // 239 0xef
  0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x00, // 240 0xf0
  0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x7e, // 241 0xf1
  0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff, // 242 0xf2
  0xe1,0x32,0xe4,0x3a,0xf6,0x2a,0x5f,0x86, // 243 0xf3
  0x00,0x7f,0xdb,0xdb,0x7b,0x1b,0x1b,0x1b, // 244 0xf4
  0x3e,0x61,0x3c,0x66,0x66,0x3c,0x86,0x7c, // 245 0xf5
  0x00,0x00,0x18,0x00,0x7e,0x00,0x18,0x00, // 246 0xf6
  0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x70, // 247 0xf7
  0x00,0x38,0x6c,0x6c,0x38,0x00,0x00,0x00, // 248 0xf8
  0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 249 0xf9
  0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00, // 250 0xfa
  0x00,0x18,0x38,0x18,0x18,0x3c,0x00,0x00, // 251 0xfb
  0x00,0x78,0x0c,0x38,0x0c,0x78,0x00,0x00, // 252 0xfc
  0x00,0x78,0x0c,0x18,0x30,0x7c,0x00,0x00, // 253 0xfd
  0x00,0x00,0x3c,0x3c,0x3c,0x3c,0x00,0x00, // 254 0xfe
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 255 0xff

}; // bgi_bitmap_font[]

static struct arccoordstype    bgi_last_arc;
static struct fillsettingstype bgi_fill_style;
static struct linesettingstype bgi_line_style;
static struct textsettingstype bgi_txt_style;
static struct viewporttype vp;
// copy of the original palette
static struct palettetype      bgi_pal;

// utility functions

static void putpixel_copy    (int, int, Uint32);
static void putpixel_xor     (int, int, Uint32);
static void putpixel_and     (int, int, Uint32);
static void putpixel_or      (int, int, Uint32);
static void putpixel_not     (int, int, Uint32);
static void ff_putpixel      (int, int);
static Uint32 getpixel_raw   (int, int);

static void line_copy        (int, int, int, int);
static void line_xor         (int, int, int, int);
static void line_and         (int, int, int, int);
static void line_or          (int, int, int, int);
static void line_not         (int, int, int, int);
static void line_fill        (int, int, int, int);
static void _floodfill       (int, int, int);
static void _setcolor        (int);

static void line_fast        (int, int, int, int);
static void updaterect       (int, int, int, int);
static void update	     (void);
static void update_pixel     (int, int);

static void unimplemented    (char *);
static int  is_in_range      (int, int, int);
static void swap_if_greater  (int *, int *);
static void circle_bresenham (int, int, int);
static int  octant           (int, int);
static void refresh_window   (void);

// Uint32 pixel value

#define PIXEL(X,Y) \
   bgi_activepage[bgi_current_window][Y * (bgi_maxx + 1) + X]

// -----

// Let's start!

// -----

// for Python
int sizeofint ()
{
  return sizeof (int);
}

// -----

// Error handling

static void check_initgraph ()
{
  // Checks if the graphics system was initialised.

  char *error;

  if (grOk != bgi_error_code) {
     error = grapherrormsg (bgi_error_code);
     fprintf (stderr, "BGI error: %s\n", error);
    exit (1);
  }

} // check_initgraph ()

// -----

// CHR fonts support. The following code was written by
// Marco Diego Aurélio Mesquita
// Some code cleanup by GG

enum opcode {
  OP_STOP = 0, OP_UNKNOW, OP_MOVETO, OP_LINETO
};

struct stroke {
  int x;
  int y;
  enum opcode op;
};

struct font {
  int capheight;
  int baseheight;
  int decheight;
  struct stroke *glyphs[256];
  int widths[256];
};

// -----

static struct font *decode (FILE *fp)
{
  // Decodes a CHR font and build its internal tables.

  char *fontinfo = NULL;
  size_t fontinfolen = 0;
  size_t tmp;

  /* All my small sample of borland fonts start with PK\X08\X08 */
  char magic[4];
  tmp = fread (magic, 4, 1, fp);
  if(magic[0] != 'P' ||
     magic[1] != 'K' ||
     magic[2] != 0x08 ||
     magic[3] != 0x08) {
    fprintf (stderr, "File does not seem to be a valid font.\n");
    return NULL;
  }

  /* Now we scan the file until we find 0x1a */
  while (!feof(fp)) {
    char c = fgetc(fp);
    // TODO: add checks for *realloc
    fontinfo = (char*)realloc(fontinfo, fontinfolen+1);
    if (c == 0x1a){
      fontinfo[fontinfolen] = '\0';
      break;
    } else
      fontinfo[fontinfolen] = c;
    fontinfolen++;
  }

  // printf("Font info is: %s\n", fontinfo);
  free (fontinfo);

  /* We'll now info about font version */
  uint16_t headerlen;
  char fontname[4];
  uint16_t fontlen;
  unsigned char ver_major;
  unsigned char ver_minor;
  unsigned char bgi_ver_major;
  unsigned char bgi_ver_minor;

  tmp = fread (&headerlen, sizeof(uint16_t), 1, fp);
  tmp = fread (&fontname, 1, 4, fp);
  tmp = fread (&fontlen, sizeof(uint16_t), 1, fp);
  ver_major = fgetc (fp);
  // mute "unused variable" compiler warning
  (void) ver_major;
  ver_minor = fgetc (fp);
  (void) ver_minor;
  bgi_ver_major = fgetc (fp);
  (void) bgi_ver_major;
  bgi_ver_minor = fgetc (fp);
  (void) bgi_ver_minor;

  /*
  printf("Header length: %d\n", headerlen);
  printf("Font name: %c%c%c%c\n",
	 fontname[0], fontname[1],
	 fontname[2], fontname[3]);
  printf("Font lenght: %d\n", fontlen);
  printf("Version: %d.%d\n", ver_major, ver_minor);
  printf("Min, bgi version: %d.%d\n", bgi_ver_major, bgi_ver_minor);
   */

  /* Inital header seems padded with zeroes */
  fseek (fp, 0x80, SEEK_SET);

  /* Now some info about offsets */
  char signature = fgetc (fp);
  (void) signature;
  // printf("Signature: %c\n", signature);

  uint16_t nglyphs;
  tmp = fread (&nglyphs, sizeof(uint16_t), 1, fp);
  // printf("Number of glyphs: %d\n", nglyphs);

  char unknown0 = fgetc (fp);
  (void) unknown0;
  // printf("unknown 0: %d\n", (int)unknown0);

  char firstglyph = fgetc (fp);
  (void) firstglyph;
  // printf("First glyph is: %c (%d) \n", firstglyph, firstglyph);

  uint16_t glyphs_offset;
  tmp = fread (&glyphs_offset, sizeof(uint16_t), 1, fp);
  // printf("Glyphs offset: %d\n", glyphs_offset);

  char is_scan = fgetc (fp);
  (void) is_scan;
  // printf("Is scannable: %d\n", (int) is_scan);

  signed char capheight = fgetc (fp);
  // printf("Capitol height: %d\n", (int) capheight);

  signed char baseheight = fgetc (fp);
  // printf("Base height: %d\n", (int) baseheight);

  signed char decheight = fgetc (fp);
  // printf("Decender height: %d\n", (int) decheight);

  /* Don't know why it has 2 names */
  char fontname2[4];
  tmp = fread (&fontname2, 1, 4, fp);
  /*
  printf("Font name: %c%c%c%c\n",
	 fontname2[0], fontname2[1],
	 fontname2[2], fontname2[3]);
   */

  char unknown1 = fgetc (fp);
  (void) unknown1;
  // printf("unknown 1: %d\n", (int)unknown1);

  /* Let's load the offset table */
  // printf ("Loading offset table...");
  uint16_t *offset_table = (uint16_t*) malloc(nglyphs*sizeof(uint16_t));
  for (int i = 0; i < nglyphs; i++)
    tmp = fread(&offset_table[i], sizeof(uint16_t), 1, fp);
  // printf("done.\n");

  /*
  for(int i = 0; i < nglyphs; i++)
    printf("char %c, offset: 0x%04x\n", (char)(i+firstglyph), (int)offset_table[i]);
   */

  /* Let's load the width table */
  // printf("Loading width table...");
  char *width_table = (char*) calloc(nglyphs, sizeof(char));
  for (int i = 0; i < nglyphs; i++)
    tmp = fread (&width_table[i], sizeof(char), 1, fp);
  // printf("done.\n");

  (void) tmp;

  /*
  for(int i = 0; i < nglyphs; i++)
    printf("char %c, width: %d\n", (char)(i+firstglyph), (int)width_table[i]);
   */

  /* Let's allocate the font */
  struct font *ret = calloc(1, sizeof(struct font));
  ret->capheight = capheight;
  ret->baseheight = baseheight;
  ret->decheight = decheight;

  /* What follows now is the list of strokes */
  int i = firstglyph;
  int j = 0;

  // Find real first char
  int real_first = 0;
  while (!offset_table[++real_first + 1]);
  // printf("Real first char is: %d\n", real_first);

  if(real_first != 1)
    i += real_first;

  char b0, b1;

  while ( EOF != fscanf (fp, "%c%c", &b0, &b1) ) {

    int opcode = ((b0&0x80) >> 6) + ((b1&0x80) >> 7);
    int x = ((int)((signed char)((b0&0x7f) << 1)))/2;
    int y = ((int)((signed char)((b1&0x7f) << 1)))/2;
    // printf("Opcode: %d, x = %d, y = %d\n", opcode, x, y);

    j++;

    ret->widths[i] = 0;

    ret->glyphs[i] =
      realloc(ret->glyphs[i], j*sizeof(struct stroke));
    ret->glyphs[i][j - 1].x = x;
    ret->glyphs[i][j - 1].y = y;
    ret->glyphs[i][j - 1].op = opcode;

    if (opcode == OP_STOP) {
      ret->widths[i] = width_table[i - firstglyph];
      // printf("char %d(%c) should have width: %d\n", i, i, ret->widths[i]);
      i++;
      if (i == nglyphs + firstglyph)
	break;
      j = 0;
    } // if

  } // while

  free (offset_table);
  free (width_table);

  return ret;

} // decode ()

// -----

static void destroy_font (struct font *f)
{
  if (!f)
    return;

  for (int i = 0; i < 256; i++)
    free(f->glyphs[i]);

  free (f);

} // destroy_font ()

// -----

// fast line drawing routine, for display_glyph()
static void lineto_fast  (int x, int y);

// -----

static int display_glyph (struct font *f, int c, float scale_x,
			  float scale_y, int x, int y, int orientation)
{
  if (!f)
    return 0;

  if(!f->glyphs[c])
    return 0;

  // fix here
  // printf("Capital: %d, Base: %d, Desc: %d\n", f->capheight, f->baseheight, f->decheight);

  int offset_y = scale_y * (f->capheight + f->baseheight) -
    (scale_y - 1) * f->decheight;

  int offset_x = scale_x * (f->capheight + f->baseheight) -
    (scale_x - 1) * f->decheight;

  //offset = scale_y*(f->capheight + f->baseheight + f->decheight);
  // printf("Offset x: %d, y: %d\n", offset_x, offset_y);

  moveto (x, y /* + offset*scale_y */);

  for (int j = 0; f->glyphs[c][j].op != OP_STOP; j++) {

    struct stroke st = f->glyphs[c][j];
    int dx;
    int dy;

    if (orientation == HORIZ_DIR) {
      dx = st.x*scale_x + x;
      dy = -st.y*scale_y + y;
    }
    else {
      dx = -st.y*scale_x + x + offset_x;
      dy = -st.x*scale_y + y - offset_y;
    }

    if (st.op == OP_LINETO)
      lineto_fast (dx, dy);
    else
      moveto(dx, dy);

  } // for

  return f->widths[c];

} // display_glyph ()

// -----

static struct font *load_font(char *filename)
{
  // Loads a CHR font from disk.

  struct font *ret;
  FILE *fp;

  // !!! open the font file with "rb"
  // for compatibility with Mingw64

  fp = fopen(filename, "rb");
  if (!fp)
    return NULL;

  ret = decode (fp);

  fclose (fp);
  return ret;

} // load_font ()

#define MAX_FONTS (20 + LAST_SPEC_FONT)

// current CHR font
static struct font
  *chr_font = NULL;

// CHR fonts
static struct font
  *chr_fonts[MAX_FONTS] = {NULL};

// index of CHR font
static int
  chr_next_font = LAST_SPEC_FONT;

// ===== UNIMPLEMENTED STUFF =====

static void unimplemented (char *msg)
{
  fprintf (stderr, "%s() is not implemented and not required.\n", msg);
}

// -----

void _graphfreemem (void *ptr, unsigned int size)
{

  unimplemented ("_graphfreemem");

} // _graphfreemem ()

// -----

void * _graphgetmem (unsigned int size)
{

  unimplemented ("_graphgetmem");
  return NULL;

} // _graphgetmem ()

// -----

int installuserdriver (char *name, int (*detect)(void))
{

  unimplemented ("installuserdriver");
  return 0;

} // installuserdriver ()

// -----

int registerbgidriver (void (*driver)(void))
{

  unimplemented ("registerbgidriver");
  return 0;

} // registerbgidriver ()

// -----

int registerbgifont (void (*font)(void))
{

  unimplemented ("registerbgifont");
  return 0;

} // registerbgifont ()

// -----

unsigned int setgraphbufsize (unsigned bufsize)
{

  unimplemented ("setgraphbufsize");
  return 0;

} // setgraphbufsize ()

// -----

// ===== STANDARD BGI FUNCTIONS =====

#define PI_CONV (3.1415926 / 180.0)

void arc (int x, int y, int stangle, int endangle, int radius)
{
  // Draws a circular arc centered at (x, y), with a radius
  // given by radius, traveling from stangle to endangle.

  // Quick and dirty for now; maybe a faster implementation later,
  // http://www.realitypixels.com/turk/computergraphics/CircularArcSubdivision.pdf

  check_initgraph ();

  int
    angle;

  if (0 == radius)
    return;

  if (endangle < stangle)
    endangle += 360;

  bgi_last_arc.x = x;
  bgi_last_arc.y = y;
  bgi_last_arc.xstart = x + (radius * cos (stangle * PI_CONV));
  bgi_last_arc.ystart = y - (radius * sin (stangle * PI_CONV));
  bgi_last_arc.xend = x + (radius * cos (endangle * PI_CONV));
  bgi_last_arc.yend = y - (radius * sin (endangle * PI_CONV));

  for (angle = stangle; angle < endangle; angle++)
    line_fast (x + floor (0.5 + (radius * cos (angle * PI_CONV))),
               y - floor (0.5 + (radius * sin (angle * PI_CONV))),
               x + floor (0.5 + (radius * cos ((angle+1) * PI_CONV))),
               y - floor (0.5 + (radius * sin ((angle+1) * PI_CONV))));

  update ();

} // arc ()

// -----

void bar (int left, int top, int right, int bottom)
{
  // Draws a filled-in rectangle (bar), using the current fill colour
  // and fill pattern.

  check_initgraph ();

  int
    y, tmp, tmpcolor, tmpthickness;

  tmp = bgi_fg_color;

  if (EMPTY_FILL == bgi_fill_style.pattern)
    tmpcolor = bgi_bg_color;
  else // all other styles
    tmpcolor = bgi_fill_style.color;

  _setcolor (tmpcolor);
  tmpthickness = bgi_line_style.thickness;
  bgi_line_style.thickness = NORM_WIDTH;

  if (SOLID_FILL == bgi_fill_style.pattern)
    for (y = top; y <= bottom; y++)
      line_fast (left, y, right, y);
  else
    for (y = top; y <= bottom; y++)
      line_fill (left, y, right, y);

  _setcolor (tmp);
  bgi_line_style.thickness = tmpthickness;

  update ();

} // bar ()

// -----

void bar3d (int left, int top, int right,
	    int bottom, int depth, int topflag)
{
  // Draws a three-dimensional, filled-in rectangle (bar), using
  // the current fill colour and fill pattern.

  check_initgraph ();

  Uint32
    tmp, tmpcolor;

  swap_if_greater (&left, &right);
  swap_if_greater (&top, &bottom);

  tmp = bgi_fg_color;

  if (EMPTY_FILL == bgi_fill_style.pattern)
    tmpcolor = bgi_bg_color;
  else // all other styles
    tmpcolor = bgi_fill_style.color;

  _setcolor (tmpcolor); // fill
  bar (left, top, right, bottom);
  _setcolor (tmp); // outline
  if (depth > 0) {
    if (topflag) {
      line_fast (left, top, left + depth, top - depth + 4);
      line_fast (left + depth, top - depth + 4,
		 right + depth, top - depth + 4);
    }
    line_fast (right, top, right + depth, top - depth + 4);
    line_fast (right, bottom, right + depth, bottom - depth + 4);
    line_fast (right + depth, bottom - depth + 4,
	       right + depth, top - depth + 4);
  }
  rectangle (left, top, right, bottom);

  update ();

} // bar3d ()

// -----

static void circle_bresenham (int x, int y, int radius)
{
  // Draws a circle of the given radius at (x, y).
  // Adapted from:
  // http://members.chello.at/easyfilter/bresenham.html

  int
    xx = -radius,
    yy = 0,
    err = 2 - 2*radius;

  do {
    fputpixel (x - xx, y + yy); //  I  quadrant
    fputpixel (x - yy, y - xx); //  II quadrant
    fputpixel (x + xx, y - yy); //  III quadrant
    fputpixel (x + yy, y + xx); //  IV quadrant
    radius = err;

    if (radius <= yy)
      err += ++yy*2 + 1;

    if (radius > xx || err > yy)
      err += ++xx*2 + 1;

  } while (xx < 0);

  update ();

} // circle_bresenham ();

// -----

void circle (int x, int y, int radius)
{
  // Draws a circle of the given radius at (x, y).

  check_initgraph ();

  // the Bresenham algorithm draws a better-looking circle

  if (NORM_WIDTH == bgi_line_style.thickness)
    circle_bresenham (x, y, radius);
  else
    arc (x, y, 0, 360, radius);

} // circle ();

// -----

void cleardevice (void)
{
  // Clears the graphics screen, filling it with the current
  // background color.

  check_initgraph ();

  int
    x, y;

  bgi_cp_x = bgi_cp_y = 0;

  for (x = 0; x < bgi_maxx + 1; x++)
    for (y = 0; y < bgi_maxy + 1; y++)
      PIXEL (x, y) = bgi_argb_palette[bgi_bg_color];

  update ();

} // cleardevice ()

// -----

void clearviewport (void)
{
  // Clears the viewport, filling it with the current
  // background color.

  check_initgraph ();

  int
    x, y;

  bgi_cp_x = bgi_cp_y = 0;

  for (x = vp.left; x < vp.right + 1; x++)
    for (y = vp.top; y < vp.bottom + 1; y++)
      PIXEL (x, y) = bgi_argb_palette[bgi_bg_color];

  update ();

} // clearviewport ()

// -----

void closegraph (void)
{
  // Closes the graphics system.

  check_initgraph ();

#ifdef __EMSCRIPTEN__
  // for Emscripten output
  cleardevice ();
  refresh ();
#endif

  // waits for update callback to finish

  bgi_refresh_needed = SDL_FALSE;
  // really necessary?
  if (bgi_auto_mode)
    SDL_Delay (500);

  for (int i = 0; i < bgi_num_windows; i++)
    if (SDL_TRUE == bgi_active_windows[i]) {
      SDL_DestroyTexture (bgi_txt[i]);
      SDL_DestroyRenderer (bgi_rnd[i]);
      SDL_DestroyWindow (bgi_win[i]);
    }

  // free visual pages - causes segmentation fault!
  // for (int page = 0; page < bgi_np; page++)
    // SDL_FreeSurface (bgi_vpage[page]);

  // Destroy our loaded fonts
  for (int i = 0; i < MAX_FONTS; i++)
    destroy_font(chr_fonts[i]);

  // Only calls SDL_Quit if not running fullscreen
  if (SDL_FULLSCREEN != bgi_gm)
    SDL_Quit ();

  // the program might continue in text mode
  bgi_num_windows = 0;

#ifdef __EMSCRIPTEN__
  emscripten_run_script ("window.close()");
#endif

} // closegraph ()

// -----

void delay (int msec)
{
  // Waits for 'msec' milliseconds.

  Uint32
    stop;

  update ();
  stop = SDL_GetTicks () + msec;

  do {

    if (event ())
      ; // event recorded

  } while (SDL_GetTicks () < stop);

} // delay ()

// -----

void detectgraph (int *graphdriver, int *graphmode)
{
  // Detects the graphics driver and graphics mode to use.

  *graphdriver = SDL;
  *graphmode = SDL_FULLSCREEN;
  bgi_error_code = grOk;

} // detectgraph ()

// -----

void drawpoly (int numpoints, int *polypoints)
{
  // Draws a polygon of numpoints vertices.

  check_initgraph ();

  int
    n;

  for (n = 0; n < numpoints - 1; n++)
    line_fast (polypoints[2*n], polypoints[2*n + 1],
               polypoints[2*n + 2], polypoints[2*n + 3]);

  /* don't close the polygon!
  line_fast (polypoints[2*n], polypoints[2*n + 1],
             polypoints[0], polypoints[1]);
   */

  update ();

} // drawpoly ()

// -----

static void swap_if_greater (int *x1, int *x2)
{
  int
    tmp;

  if (*x1 > *x2) {
    tmp = *x1;
    *x1 = *x2;
    *x2 = tmp;
  }

} // swap_if_greater ()

// -----

static void _ellipse (int, int, int, int);

void ellipse (int x, int y, int stangle, int endangle,
              int xradius, int yradius)
{
  // Draws an elliptical arc centered at (x, y), with axes given by
  // xradius and yradius, traveling from stangle to endangle.
  // Bresenham-based if complete

  check_initgraph ();

  int
    angle;

  if (0 == xradius && 0 == yradius)
    return;

  if (endangle < stangle)
    endangle += 360;

  // draw complete ellipse
  if (0 == stangle && 360 == endangle) {
    _ellipse (x, y, xradius, yradius);
    return;
  }

  // really needed?
  bgi_last_arc.x = x;
  bgi_last_arc.y = y;

  for (angle = stangle; angle < endangle; angle++)
    line_fast (x + (xradius * cos (angle * PI_CONV)),
               y - (yradius * sin (angle * PI_CONV)),
               x + (xradius * cos ((angle + 1) * PI_CONV)),
               y - (yradius * sin ((angle + 1) * PI_CONV)));

  update ();

} // ellipse ()

// -----

// Yeah, duplicated code. The thing is, I can't catch the bug.

void _ellipse (int cx, int cy, int xradius, int yradius)
{
  // from "A Fast Bresenham Type Algorithm For Drawing Ellipses"
  // by John Kennedy

  int
    x, y,
    xchange, ychange,
    ellipseerror,
    TwoASquare, TwoBSquare,
    StoppingX, StoppingY;

  if (0 == xradius && 0 == yradius)
    return;

  TwoASquare = 2*xradius*xradius;
  TwoBSquare = 2*yradius*yradius;
  x = xradius;
  y = 0;
  xchange = yradius*yradius*(1 - 2*xradius);
  ychange = xradius*xradius;
  ellipseerror = 0;
  StoppingX = TwoBSquare*xradius;
  StoppingY = 0;

  while (StoppingX >= StoppingY) {

    // 1st set of points, y' > -1

    // normally, I'd put the line_fill () code here; but
    // the outline gets overdrawn, can't find out why.
    fputpixel (cx + x, cy - y);
    fputpixel (cx - x, cy - y);
    fputpixel (cx - x, cy + y);
    fputpixel (cx + x, cy + y);
    y++;
    StoppingY += TwoASquare;
    ellipseerror += ychange;
    ychange +=TwoASquare;

    if ((2*ellipseerror + xchange) > 0 ) {
      x--;
      StoppingX -= TwoBSquare;
      ellipseerror +=xchange;
      xchange += TwoBSquare;
    }
  } // while

  // 1st point set is done; start the 2nd set of points

  x = 0;
  y = yradius;
  xchange = yradius*yradius;
  ychange = xradius*xradius*(1 - 2*yradius);
  ellipseerror = 0;
  StoppingX = 0;
  StoppingY = TwoASquare*yradius;

  while (StoppingX <= StoppingY ) {

    // 2nd set of points, y' < -1

    fputpixel (cx + x, cy - y);
    fputpixel (cx - x, cy - y);
    fputpixel (cx - x, cy + y);
    fputpixel (cx + x, cy + y);
    x++;
    StoppingX += TwoBSquare;
    ellipseerror += xchange;
    xchange +=TwoBSquare;
    if ((2*ellipseerror + ychange) > 0) {
      y--,
        StoppingY -= TwoASquare;
      ellipseerror += ychange;
      ychange +=TwoASquare;
    }
  }

  update ();

} // _ellipse ()

// -----

void fillellipse (int cx, int cy, int xradius, int yradius)
{
  // Draws an ellipse centered at (x, y), with axes given by
  // xradius and yradius, and fills it using the current fill color
  // and fill pattern.

  // from "A Fast Bresenham Type Algorithm For Drawing Ellipses"
  // by John Kennedy

  check_initgraph ();

  int
    x, y,
    xchange, ychange,
    ellipseerror,
    TwoASquare, TwoBSquare,
    StoppingX, StoppingY;

  if (0 == xradius && 0 == yradius)
    return;

  TwoASquare = 2*xradius*xradius;
  TwoBSquare = 2*yradius*yradius;
  x = xradius;
  y = 0;
  xchange = yradius*yradius*(1 - 2*xradius);
  ychange = xradius*xradius;
  ellipseerror = 0;
  StoppingX = TwoBSquare*xradius;
  StoppingY = 0;

  while (StoppingX >= StoppingY) {

    // 1st set of points, y' > -1

    line_fill (cx + x, cy - y, cx - x, cy - y);
    line_fill (cx - x, cy + y, cx + x, cy + y);
    y++;
    StoppingY += TwoASquare;
    ellipseerror += ychange;
    ychange +=TwoASquare;

    if ((2*ellipseerror + xchange) > 0 ) {
      x--;
      StoppingX -= TwoBSquare;
      ellipseerror +=xchange;
      xchange += TwoBSquare;
    }
  } // while

  // 1st point set is done; start the 2nd set of points

  x = 0;
  y = yradius;
  xchange = yradius*yradius;
  ychange = xradius*xradius*(1 - 2*yradius);
  ellipseerror = 0;
  StoppingX = 0;
  StoppingY = TwoASquare*yradius;

  while (StoppingX <= StoppingY ) {

    // 2nd set of points, y' < -1

    line_fill (cx + x, cy - y, cx - x, cy - y);
    line_fill (cx - x, cy + y, cx + x, cy + y);
    x++;
    StoppingX += TwoBSquare;
    ellipseerror += xchange;
    xchange +=TwoBSquare;
    if ((2*ellipseerror + ychange) > 0) {
      y--,
        StoppingY -= TwoASquare;
      ellipseerror += ychange;
      ychange +=TwoASquare;
    }
  }

  // outline

  _ellipse (cx, cy, xradius, yradius);

  update ();

} // fillellipse ()

// -----

static int intcmp (const void *n1, const void *n2)
{
  // helper function for fillpoly ()

  return (*(const int *) n1) - (*(const int *) n2);

} // intcmp ()

// -----

// the following function was adapted from the public domain
// code by Darel Rex Finley,
// http://alienryderflex.com/polygon_fill/

void fillpoly (int numpoints, int *polypoints)
{
  // Draws a polygon of numpoints vertices and fills it using the
  // current fill color.

  check_initgraph ();

  int
    nodes,      // number of nodes
    *nodeX,     // array of nodes
    ymin, ymax,
    pixelY,
    i, j,
    tmp, tmpcolor;

  if (NULL == (nodeX = calloc (sizeof (int), numpoints))) {
    char *str = "Can't allocate memory for fillpoly().\n";
    fprintf (stderr, "%s", str);
    showerrorbox (str);
    return;
  }

  tmp = bgi_fg_color;
  if (EMPTY_FILL == bgi_fill_style.pattern)
    tmpcolor = bgi_bg_color;
  else // all other styles
    tmpcolor = bgi_fill_style.color;

  _setcolor (tmpcolor);

  // find Y maxima
  ymin = ymax = polypoints[1];

  for (i = 0; i < 2 * numpoints; i += 2) {
    if (polypoints[i + 1] < ymin)
      ymin = polypoints[i + 1];
    if (polypoints[i + 1] > ymax)
      ymax = polypoints[i + 1];
  }

  //  Loop through the rows of the image.
  for (pixelY = ymin; pixelY < ymax; pixelY++) {

    //  Build a list of nodes.
    nodes = 0;
    j = 2 * numpoints - 2;

    for (i = 0; i < 2 * numpoints; i += 2) {

      if (
          ((float) polypoints[i + 1] < (float)  pixelY &&
           (float) polypoints[j + 1] >= (float) pixelY) ||
          ((float) polypoints[j + 1] < (float)  pixelY &&
           (float) polypoints[i + 1] >= (float) pixelY))
        nodeX[nodes++] =
        (int) (polypoints[i] + (pixelY - (float) polypoints[i + 1]) /
               ((float) polypoints[j + 1] - (float) polypoints[i + 1]) *
               (polypoints[j] - polypoints[i]));
      j = i;
    }

    // sort the nodes
    qsort (nodeX, nodes, sizeof (int), intcmp);

    // fill the pixels between node pairs.
    for (i = 0; i < nodes; i += 2) {
      if (SOLID_FILL == bgi_fill_style.pattern)
        line_fast (nodeX[i], pixelY, nodeX[i + 1], pixelY);
      else
        line_fill (nodeX[i], pixelY, nodeX[i + 1], pixelY);
    }

  } //   for pixelY

  _setcolor (tmp);
  drawpoly (numpoints, polypoints);
  
  // close the polygon, since diawpoly() will not
  i = numpoints - 1;
  line_fast (polypoints[2*i], polypoints[2*i + 1],
             polypoints[0], polypoints[1]);

  update ();

} // fillpoly ()

// -----

// These are setfillpattern-compatible arrays for the tiling patterns.

static Uint8 fill_patterns[1 + USER_FILL][8] = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // EMPTY_FILL
  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // SOLID_FILL
  {0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00}, // LINE_FILL
  {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}, // LTSLASH_FILL
  {0xe0, 0xc1, 0x83, 0x07, 0x0e, 0x1c, 0x38, 0x70}, // SLASH_FILL
  {0xf0, 0x78, 0x3c, 0x1e, 0x0f, 0x87, 0xc3, 0xe1}, // BKSLASH_FILL
  {0xd2, 0x69, 0xb4, 0x5a, 0x2d, 0x96, 0x4b, 0xa5}, // LTBKSLASH_FILL
  {0xff, 0x88, 0x88, 0x88, 0xff, 0x88, 0x88, 0x88}, // HATCH_FILL
  {0x81, 0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42}, // XHATCH_FILL
  {0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33}, // INTERLEAVE_FILL
  {0x01, 0x00, 0x10, 0x00, 0x01, 0x00, 0x10, 0x00}, // WIDE_DOT_FILL
  {0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00}, // CLOSE_DOT_FILL
  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}  // USER_FILL
};

static Uint8 mirror_bits (Uint8 n)
{
  // Used by setfillpattern()

  Uint8
    i, ret = 0;

  for (i = 0; i < 8; i++)
    if ((n & (1 << i)) != 0)
      ret += (1 << (7 - i));

  return ret;

} // mirror_bits ()

// -----

static void ff_putpixel (int x, int y)
{
  // similar to putpixel (), but uses fill patterns

  Uint8
    ptn;

  x += vp.left;
  y += vp.top;

  ptn = mirror_bits (fill_patterns[bgi_fill_style.pattern][y % 8]);

  // if the corresponding bit in the pattern is 1
  if ( (ptn >> x % 8) & 1)
    putpixel_copy (x, y, bgi_argb_palette[bgi_fill_style.color]);
  else
    putpixel_copy (x, y, bgi_argb_palette[bgi_bg_color]);

} // ff_putpixel ()

// -----

// the following code is adapted from
// https://lodev.org/cgtutor/floodfill.html

void _floodfill (int x, int y, int border)
{
  int
    x1,
    sl_x1, sl_x2, // scanline X coordinates
    oldcol = getpixel (x, y);

  // draw current scanline from start position to the right
  x1 = x;
  while (x1 < getmaxx () && getpixel (x1, y) != border)
    ff_putpixel (x1++, y);
  sl_x2 = x1 - 1; // scanline right X coordinate

  // draw current scanline from start position to the left
  x1 = x - 1;
  while (x1 >= 0 && getpixel (x1, y) != border)
    ff_putpixel (x1--, y);
  sl_x1 = x1 + 1; // scanline left X coordinate

  // test for new scanlines above
  x1 = x;
  while (x1 < getmaxx () && x1 <= sl_x2) {
    if (y > 0 && getpixel (x1, y - 1) == oldcol)
      _floodfill (x1, y - 1, border);
    x1++;
  }
  x1 = x - 1;
  while (x1 >= 0 && x1 >= sl_x1) {
    if (y > 0 && getpixel (x1, y - 1) == oldcol)
      _floodfill (x1, y - 1, border);
    x1--;
  }

  // test for new scanlines below
  x1 = x;
  while (x1 < getmaxx () && x1 <= sl_x2) {
    if (y < getmaxy () - 1 && getpixel (x1, y + 1) == oldcol)
      _floodfill (x1, y + 1, border);
    x1++;
  }

  x1 = x - 1;
  while (x1 >= 0 && x1 >= sl_x1) {
    if (y < getmaxy () - 1 && getpixel (x1, y + 1) == oldcol)
      _floodfill (x1, y + 1, border);
    x1--;
  }

} // _floodfill ()

// -----

void floodfill (int x, int y, int border)
{
  // Fills an enclosed area with colour.

  check_initgraph ();

  unsigned int
    oldcol;
  int
    tmp_pattern,
    tmp_color;

  oldcol = getpixel (x, y);

  // the way the above implementation of floodfill works,
  // the fill colour must be different from the background color

  if (oldcol == border || oldcol == bgi_fill_style.color ||
      x < 0 || x > vp.right - vp.left || // out of viewport/window?
      y < 0 || y > vp.bottom - vp.top)
    return;

  // special case for fill patterns. The background colour can't be
  // the same in the area to be filled and in the fill pattern.

  if (SOLID_FILL == bgi_fill_style.pattern) {
    _floodfill (x, y, border);
    return;
  }

  else { // fill patterns

    if (bgi_bg_color == oldcol) {

      // solid fill first, then pattern fill on top of it
      tmp_pattern = bgi_fill_style.pattern;
      bgi_fill_style.pattern = SOLID_FILL;
      tmp_color = bgi_fill_style.color;

      // find a suitable temporary fill colour; it must be
      // different from the border, the background,
      // and the former fill colour
      for (int i = BLACK; i < MAXCOLORS + 1; i++) {
	bgi_fill_style.color = i;
	if (oldcol != bgi_fill_style.color &&
	    border != bgi_fill_style.color &&
	    tmp_color != bgi_fill_style.color)
	  break;
      }

      // solid fill...
      _floodfill (x, y, border);

      // ...then pattern fill
      bgi_fill_style.pattern = tmp_pattern;
      bgi_fill_style.color = tmp_color;
      _floodfill (x, y, border);
    } // if
    else
      _floodfill (x, y, border);

  } // else

  update ();

} // floodfill ()

// -----

int getactivepage (void)
{
  // Returns the active page number.

  check_initgraph ();
  return (bgi_ap);

} // getactivepage ()

// -----

void getarccoords (struct arccoordstype *arccoords)
{
  // Gets the coordinates of the last call to arc(), filling the
  // arccoords structure.

  check_initgraph ();

  arccoords->x = bgi_last_arc.x;
  arccoords->y = bgi_last_arc.y;
  arccoords->xstart = bgi_last_arc.xstart;
  arccoords->ystart = bgi_last_arc.ystart;
  arccoords->xend = bgi_last_arc.xend;
  arccoords->yend = bgi_last_arc.yend;

} // getarccoords ()

// -----

void getaspectratio (int *xasp, int *yasp)
{
  // Retrieves the current graphics mode's aspect ratio.
  // Irrelevant on modern hardware.

  check_initgraph ();

  *xasp = 10000;
  *yasp = 10000;

} // getaspectratio ()

// -----

int getbkcolor (void)
{
  // Returns the current background color.

  check_initgraph ();

  return bgi_bg_color;

} // getbkcolor ()

// -----

// **Note**: this function belongs to "conio.h"; it is here
// for convenience and WinBGIm compatibility.
// The name is not 'getch' to avoid a bug with Mingw64.

int bgi_getch (void)
{
  // Waits for a key and returns its ASCII value or code,
  // or QUIT if the user asked to close the window

  int
    key, type;

  SDL_Keymod
    keymod;

  if (bgi_window_is_hidden
      || grNoInitGraph == bgi_error_code
      || 0 == bgi_num_windows)
    return (getchar ());

  refresh ();
  
  // any pending keypresses by kbhit ()?
  if (SDL_TRUE == bgi_key_pressed) {
    bgi_key_pressed = SDL_FALSE;
    return bgi_last_key_pressed;
  }

  do {

    key = getevent ();
    type = bgi_last_event;
    keymod = SDL_GetModState ();

    if (QUIT == type)
      return QUIT;

    if (SDL_KEYDOWN == type &&
        key != KEY_LEFT_CTRL &&
        key != KEY_RIGHT_CTRL &&
        key != KEY_LEFT_SHIFT &&
        key != KEY_RIGHT_SHIFT &&
        key != KEY_LEFT_ALT &&
        key != KEY_RIGHT_ALT &&
        key != KEY_CAPSLOCK &&
        key != KEY_LGUI &&
        key != KEY_RGUI &&
        key != KEY_MENU &&
        key != KEY_ALT_GR) { // can't catch AltGr!
      if (KMOD_LSHIFT == keymod ||
	  KMOD_RSHIFT == keymod ||
	  KMOD_CAPS   == keymod)
	key -= ('a' - 'A');
      return (int) key;
    }

  } while (1);

  // we should never get here...
  return 0;

} // bgi_getch ()

// -----

int getcolor (void)
{
  // Returns the current drawing (foreground) color.

  check_initgraph ();

  return bgi_fg_color;

} // getcolor ()

// -----

struct palettetype *getdefaultpalette (void)
{
  // Returns the default palette

  check_initgraph ();

  return &bgi_pal;

} // getdefaultpalette ()

// -----

char *getdrivername (void)
{
  // Returns a pointer to a string containing the name
  // of the current graphics driver.

  check_initgraph ();

  return ("SDL_bgi");

} // getdrivername ()

// -----

void getfillpattern (char *pattern)
{
  // Copies the user-defined fill pattern, as set by setfillpattern,
  // into the 8-byte area pointed to by pattern.

  check_initgraph ();

  for (int i = 0; i < 8; i++)
    pattern[i] = (char) fill_patterns[USER_FILL][i];

} // getfillpattern ()

// -----

void getfillsettings (struct fillsettingstype *fillinfo)
{
  // Fills the fillsettingstype structure pointed to by fillinfo
  // with information about the current fill pattern and fill color.

  check_initgraph ();

  fillinfo->pattern = bgi_fill_style.pattern;
  fillinfo->color = bgi_fill_style.color;

} // getfillsettings ()

// -----

int getgraphmode (void)
{
  // Returns the current graphics mode.

  check_initgraph ();

  return bgi_gm;

} // getgraphmode ()

// -----

void getimage (int left, int top, int right, int bottom, void *bitmap)
{
  // Copies a bit image of the specified region into the memory
  // pointed by bitmap.

  check_initgraph ();

  Uint32
    bitmap_w, bitmap_h, *tmp;
  int
    i = 2, x, y;

  // bitmap has already been malloc()'ed by the user.
  tmp = bitmap;
  bitmap_w = right - left + 1;
  bitmap_h = bottom - top + 1;

  // copy width and height to the beginning of bitmap
  memcpy (tmp, &bitmap_w, sizeof (Uint32));
  memcpy (tmp + 1, &bitmap_h, sizeof (Uint32));

  // copy image to bitmap
  for (y = top + vp.top; y <= bottom + vp.top; y++)
    for (x = left + vp.left; x <= right + vp.left; x++)
      tmp [i++] = getpixel_raw (x, y);

} // getimage ()

// -----

void getlinesettings (struct linesettingstype *lineinfo)
{
  // Fills the linesettingstype structure pointed by lineinfo with
  // information about the current line style, pattern, and thickness.

  check_initgraph ();

  lineinfo->linestyle = bgi_line_style.linestyle;
  lineinfo->upattern = bgi_line_style.upattern;
  lineinfo->thickness = bgi_line_style.thickness;

} // getlinesettings ();

// -----

int getmaxcolor (void)
{
  // Returns the maximum color value available (MAXCOLORS).

  check_initgraph ();

  if (! bgi_argb_mode)
    return MAXCOLORS;
  else
    return PALETTE_SIZE;

} // getmaxcolor ()

// -----

int getmaxheight (void)
{
  // Returns the maximum height available for a new window.

  int
    x, y;

  getscreensize (&x, &y);

  return (y);

} // getmaxheight ()

// -----

int getmaxmode (void)
{
  // Returns the maximum mode number for the current driver.

  return SDL_FULLSCREEN;

} // getmaxmode ()

// -----

int getmaxwidth (void)
{
  // Returns the maximum width available for a new window.

  int
    x, y;

  getscreensize (&x, &y);

  return (x);

} // getmaxwidth ()

// -----


int getmaxx ()
{
  // Returns the maximum x screen coordinate.

  check_initgraph ();

  return bgi_maxx;

} // getmaxx ()

// -----

int getmaxy ()
{
  // Returns the maximum y screen coordinate.

  check_initgraph ();

  return bgi_maxy;

} // getmaxy ()

// -----

char *getmodename (int mode_number)
{
  // Returns a pointer to a string containing
  // the name of the specified graphics mode.

  check_initgraph ();

  switch (mode_number) {

  case SDL_CGALO:
    return "SDL_CGALO";
    break;

  case SDL_CGAHI:
    return "SDL_CGAHI";
    break;

  case SDL_EGA:
    return "SDL_EGA";
    break;

  case SDL_VGA:
    return "SDL_VGA";
    break;

  case SDL_HERC:
    return "SDL_HERC";
    break;

  case SDL_PC3270:
    return "SDL_PC3270";
    break;

  case SDL_1024x768:
    return "SDL_1024x768";
    break;

  case SDL_1152x900:
    return "SDL_1152x900";
    break;

  case SDL_1280x1024:
    return "SDL_1280x1024";
    break;

  case SDL_1366x768:
    return "SDL_1366x768";
    break;

  case SDL_USER:
    return "SDL_USER";
    break;

  case SDL_FULLSCREEN:
    return "SDL_FULLSCREEN";
    break;

  default:
  case SDL_800x600:
    return "SDL_800x600";
    break;

  } // switch

} // getmodename ()

// -----

void getmoderange (int graphdriver, int *lomode, int *himode)
{
  // Gets the range of valid graphics modes.

  // CHECK
  // check_initgraph ();

  // return dummy values
  *lomode = SDL;
  *himode = SDL_FULLSCREEN;

} // getmoderange ()

// -----

void getpalette (struct palettetype *palette)
{
  // Fills the palettetype structure pointed by palette with
  // information about the current palette's size and colors.

  check_initgraph ();
  
  if (NULL == palette) {
    bgi_error_code = grError;
    return;
  }

  for (int col = BLACK; col < MAXCOLORS; col++)
    palette->colors[col] = bgi_argb_palette[col];

  palette->size = MAXCOLORS + 1;

} // getpalette ()

// -----

int getpalettesize ()
{
  // Returns the size of the palette, depending of bgi_argb_mode.

  check_initgraph ();

  if (! bgi_argb_mode)
    return BGI_COLORS;
  else
    return PALETTE_SIZE;

} // getpalettesize ()

// -----

static Uint32 getpixel_raw (int x, int y)
{
  // Returns a pixel as Uint32 value

  return PIXEL (x, y);

} // getpixel_raw ()

// -----

static int is_in_range (int x, int x1, int x2)
{
  // Utility function for getpixel ()

  return (x >= x1 && x <= x2);

} // is_in_range ()

// -----

unsigned int getpixel (int x, int y)
{
  // Returns the color of the pixel located at (x, y).

  check_initgraph ();

  int
    col;
  Uint32
    tmp;

  x += vp.left;
  y += vp.top;

  // out of screen?
  if (! is_in_range (x, 0, bgi_maxx) &&
      ! is_in_range (y, 0, bgi_maxy))
    return bgi_bg_color;

  tmp = getpixel_raw (x, y);

  // now find the colour

  for (col = BLACK; col < MAXCOLORS + 1; col++)
    if (tmp == bgi_argb_palette[col])
      return col;

  // if it's not a BGI color, just return the 0xAARRGGBB value
  return tmp;

} // getpixel ()

// -----

void gettextsettings (struct textsettingstype *texttypeinfo)
{
  // Fills the textsettingstype structure pointed to by texttypeinfo
  // with information about the current text font, direction, size,
  // and justification.

  check_initgraph ();

  texttypeinfo->font      = bgi_txt_style.font;
  texttypeinfo->direction = bgi_txt_style.direction;
  texttypeinfo->charsize  = bgi_txt_style.charsize;
  texttypeinfo->horiz     = bgi_txt_style.horiz;
  texttypeinfo->vert      = bgi_txt_style.vert;

} // gettextsettings ()

// -----

void getviewsettings (struct viewporttype *viewport)
{
  // Fills the viewporttype structure pointed to by viewport
  // with information about the current viewport.

  check_initgraph ();

  viewport->left   = vp.left;
  viewport->top    = vp.top;
  viewport->right  = vp.right;
  viewport->bottom = vp.bottom;
  viewport->clip   = vp.clip;

} // getviewsettings ()

// -----

int getvisualpage (void)
{
  // Returns the visual page number.

  return (bgi_vp);

} // getvisualpage ()

// -----

int getx (void)
{
  // Returns the current x coordinate, relative to the viewport.

  check_initgraph ();

  return bgi_cp_x;

} // getx ()

// -----

int gety (void)
{
  // Returns the current y coordinate, relative to the viewport.

  check_initgraph ();

  return bgi_cp_y;

} // gety ()

// -----

char *grapherrormsg (int errorcode)
{
  // Returns a pointer to the error message string associated with
  // errorcode, returned by graphresult().

  // CHECK
  // check_initgraph ();

  char
    *error_msg[] = {
      "No error",
      "(BGI) graphics not installed (use initgraph)",
      "Graphics hardware not detected",
      "Device driver file not found",
      "Invalid device driver file",
      "Not enough memory to load driver",
      "Out of memory in scan fill",
      "Out of memory in flood fill",
      "Font file not found",
      "Not enough memory to load font",
      "Invalid graphics mode for selected driver",
      "Graphics error",
      "Graphics I/O error",
      "Invalid font file",
      "Invalid font number",
      "Invalid device number",
      "", // -16, unspecified
      "", // -17, unspecified
      "Invalid version number"
    };

  return error_msg [-errorcode];

} // grapherrormsg ()

// -----

void graphdefaults (void)
{
  // Resets all graphics settings to their defaults.

  check_initgraph ();
  initpalette ();

  // initialise the graphics writing mode
  bgi_writemode = COPY_PUT;

  // initialise the viewport
  vp.left = 0;
  vp.top = 0;

  vp.right = bgi_maxx;
  vp.bottom = bgi_maxy;
  vp.clip = SDL_TRUE;

  // initialise the CP
  bgi_cp_x = 0;
  bgi_cp_y = 0;

  // initialise the text settings
  bgi_txt_style.font = DEFAULT_FONT;
  bgi_txt_style.direction = HORIZ_DIR;
  bgi_txt_style.charsize = 1;
  bgi_txt_style.horiz = LEFT_TEXT;
  bgi_txt_style.vert = TOP_TEXT;

  // initialise the fill settings
  bgi_fill_style.pattern =  SOLID_FILL;
  bgi_fill_style.color = WHITE;

  // initialise the line settings
  bgi_line_style.linestyle = SOLID_LINE;
  bgi_line_style.upattern = EMPTY_FILL;
  bgi_line_style.thickness = NORM_WIDTH;

  // initialise the palette
  bgi_pal.size = BGI_COLORS;
  if (SDL_FALSE == bgi_use_newpalette)
    // use the old (ugly) palette
    for (int i = BLACK; i < MAXCOLORS + 1; i++)
      bgi_pal.colors[i] = bgi_orig_palette[i];
  else
    // use the new palette
    for (int i = BLACK; i < MAXCOLORS + 1; i++)
      bgi_pal.colors[i] = bgi_std_palette[i];

  // the rgb palette is not initialised

} // graphdefaults ()

// -----

int graphresult (void)
{
  // Returns the error code for the last unsuccessful graphics
  // operation and resets the error level to grOk.

  int
    tmp = bgi_error_code;
  bgi_error_code = grOk;

  return tmp;

} // graphresult ()

// -----

unsigned imagesize (int left, int top, int right, int bottom)
{
  // Returns the size in bytes of the memory area required to store
  // a bit image.

  check_initgraph ();

  return 2 * sizeof(Uint32) + // witdth, height
    (right - left + 1) * (bottom - top + 1) * sizeof (Uint32);

} // imagesize ()

// -----

void initgraph (int *graphdriver, int *graphmode, char *pathtodriver)
{
  // Initializes the graphics system.

  bgi_fast_mode = SDL_FALSE;   // BGI compatibility

  // the graphics driver parameter is always set to SDL; graphics
  // modes may vary; the path parameter is ignored.

  if (NULL != graphmode)
    bgi_gm = *graphmode;
  else
    bgi_gm = SDL_800x600; // default

  // if graphdriver == NULL, no problem

  if (NULL != graphdriver && DETECT == *graphdriver) {
    // graphics mode can be forced to VGA for compatibility
    char *res = getenv ("SDL_BGI_RES");
    if (NULL != res)
      if (0 == strcmp ("VGA", res)) {
	bgi_gm = SDL_VGA;
	if (NULL != graphmode)
	  *graphmode = SDL_VGA;
      }
  }

#ifdef __EMSCRIPTEN__
  if (NULL != graphdriver && DETECT == *graphdriver) {
    // graphics mode can be forced to VGA for compatibility
    char res[4];
    FILE *file = fopen ("SDL_BGI_RES", "r");
    if (NULL != file) {
      fgets (res, 4, file);
      fclose (file);
      if (0 == strcmp ("VGA", res))
	bgi_gm = SDL_VGA;
      if (NULL != graphmode)
	*graphmode = SDL_VGA;
    }
  }
#endif

  switch (bgi_gm) {

  case SDL_320x200:
    initwindow (320, 200);
    break;

  case SDL_640x200:
    initwindow (640, 200);
    break;

  case SDL_640x350:
    initwindow (640, 350);
    break;

  case SDL_640x480:
    initwindow (640, 480);
    break;

  case SDL_720x348:
    initwindow (720, 348);
    break;

  case SDL_720x350:
    initwindow (720, 350);
    break;

  case SDL_1024x768:
    initwindow (1024, 768);
    break;

  case SDL_1152x900:
    initwindow (1152, 900);
    break;

  case SDL_1280x1024:
    initwindow (1280, 1024);
    break;

  case SDL_1366x768:
    initwindow (1366, 768);
    break;

  case SDL_FULLSCREEN:
    initwindow (0, 0);
    break;

  default:
  case SDL_800x600:
    initwindow (800, 600);
    break;

  } // switch

  cleardevice ();
  refresh_window ();

  bgi_error_code = grOk;

} // initgraph ()

// -----

void initpalette (void)
{
  // Initialises the default palette

  check_initgraph ();

#ifndef __EMSCRIPTEN__
  char
    *res = getenv ("SDL_BGI_PALETTE");

  if (NULL != res &&
      (0 == strcmp ("BGI", res)) )
    bgi_use_newpalette = SDL_FALSE;
#else
  char res[4];
  FILE *file = fopen ("SDL_BGI_PALETTE", "r");
  if (NULL != file) {
    fgets (res, 4, file);
    fclose (file);
    if (0 == strcmp ("BGI", res))
      bgi_use_newpalette = SDL_FALSE;
  }
#endif

  bgi_argb_palette = calloc (BGI_COLORS + TMP_COLORS + PALETTE_SIZE,
			     sizeof (Uint32));
  if (NULL == bgi_argb_palette) {
    char *str = "Could not allocate global palette, aborting.\n";
    fprintf (stderr, "%s", str);
    showerrorbox (str);
    exit (1);
  }

  if (SDL_FALSE == bgi_use_newpalette) // use the old (ugly) palette
    for (int i = BLACK; i < MAXCOLORS + 1; i++)
      bgi_argb_palette[i] = bgi_orig_palette[i];
  else // use the new palette
    for (int i = BLACK; i < MAXCOLORS + 1; i++)
      bgi_argb_palette[i] = bgi_std_palette[i];

} // initpalette ()

// -----

// **Note**: this function belongs to "conio.h"; it is here for
// convenience and WinBGIm compatibility.

// k_bhit() instead of kbhit() to fix Mingw bug

int k_bhit (void)
{
  // Returns 1 when a key is pressed, or QUIT
  // if the user asked to close the window

  SDL_Event
    event;
  SDL_Keycode
    key;
  SDL_Keymod
    keymod;

  update ();
  
  if (SDL_TRUE == bgi_key_pressed) { // a key was pressed during delay ()
    bgi_key_pressed = SDL_FALSE;
    return SDL_TRUE;
  }

  if (SDL_PollEvent (&event)) {

    if (SDL_KEYDOWN == event.type) {
      key = event.key.keysym.sym;
      bgi_last_event = event.type;
      keymod = SDL_GetModState ();
      if (key != SDLK_LCTRL &&
          key != SDLK_RCTRL &&
          key != SDLK_LSHIFT &&
          key != SDLK_RSHIFT &&
          key != SDLK_LGUI &&
          key != SDLK_RGUI &&
          key != SDLK_LALT &&
          key != SDLK_RALT &&
          key != SDLK_PAGEUP &&
          key != SDLK_PAGEDOWN &&
          key != SDLK_CAPSLOCK &&
          key != SDLK_MENU &&
          key != SDLK_APPLICATION) {
	bgi_key_pressed = SDL_TRUE;
	bgi_last_key_pressed = (int) key;
	if (KMOD_LSHIFT == keymod ||
	    KMOD_RSHIFT == keymod ||
	    KMOD_CAPS   == keymod)
	  bgi_last_key_pressed -= ('a' - 'A');
	return SDL_TRUE;
      }
      else
        return SDL_FALSE;
    } // if (SDL_KEYDOWN == event.type)
    else
      if (SDL_WINDOWEVENT == event.type) {
        if (SDL_WINDOWEVENT_CLOSE == event.window.event)
          return QUIT;
      }
    else
      SDL_PushEvent (&event); // don't disrupt the mouse

  } // if (SDL_PollEvent (&event))

  return SDL_FALSE;

} // k_bhit ()

// -----

int lastkey (void)
{
  // Returns the last key pressed

  return bgi_last_key_pressed;
}

// -----

// Bresenham's line algorithm routines that implement logical
// operations: copy, xor, and, or, not.

void line_copy (int x1, int y1, int x2, int y2)
{
  int
    counter = 0, // # of pixel plotted
    dx = abs (x2 - x1),
    sx = x1 < x2 ? 1 : -1,
    dy = abs (y2 - y1),
    sy = y1 < y2 ? 1 : -1,
    err = (dx > dy ? dx : -dy) / 2,
    e2;

  for (;;) {

    // plot the pixel only if the corresponding bit
    // in the current pattern is set to 1

    if (SOLID_LINE == bgi_line_style.linestyle)
      putpixel_copy (x1, y1, bgi_argb_palette[bgi_fg_color]);
    else
      if ((bgi_line_patterns[bgi_line_style.linestyle] >> counter % 16) & 1)
        putpixel_copy (x1, y1, bgi_argb_palette[bgi_fg_color]);

    counter++;

    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for

} // line_copy ()

// -----

void line_xor (int x1, int y1, int x2, int y2)
{
  int
    counter = 0, // # of pixel plotted
    dx = abs (x2 - x1),
    sx = x1 < x2 ? 1 : -1,
    dy = abs (y2 - y1),
    sy = y1 < y2 ? 1 : -1,
    err = (dx > dy ? dx : -dy) / 2,
    e2;

  for (;;) {

    if (SOLID_LINE == bgi_line_style.linestyle)
      putpixel_xor (x1, y1, bgi_argb_palette[bgi_fg_color]);
    else
      if ((bgi_line_patterns[bgi_line_style.linestyle] >> counter % 16) & 1)
        putpixel_xor (x1, y1, bgi_argb_palette[bgi_fg_color]);

    counter++;

    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for

} // line_xor ()

// -----

void line_and (int x1, int y1, int x2, int y2)
{
  int
    counter = 0, // # of pixel plotted
    dx = abs (x2 - x1),
    sx = x1 < x2 ? 1 : -1,
    dy = abs (y2 - y1),
    sy = y1 < y2 ? 1 : -1,
    err = (dx > dy ? dx : -dy) / 2,
    e2;

  for (;;) {

    if (SOLID_LINE == bgi_line_style.linestyle)
      putpixel_and (x1, y1, bgi_argb_palette[bgi_fg_color]);
    else
      if ((bgi_line_patterns[bgi_line_style.linestyle] >> counter % 16) & 1)
        putpixel_and (x1, y1, bgi_argb_palette[bgi_fg_color]);

    counter++;

    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for

} // line_and ()

// -----

void line_or (int x1, int y1, int x2, int y2)
{
  int
    counter = 0, // # of pixel plotted
    dx = abs (x2 - x1),
    sx = x1 < x2 ? 1 : -1,
    dy = abs (y2 - y1),
    sy = y1 < y2 ? 1 : -1,
    err = (dx > dy ? dx : -dy) / 2,
    e2;

  for (;;) {

    if (SOLID_LINE == bgi_line_style.linestyle)
      putpixel_or (x1, y1, bgi_argb_palette[bgi_fg_color]);
    else
      if ((bgi_line_patterns[bgi_line_style.linestyle] >> counter % 16) & 1)
        putpixel_or (x1, y1, bgi_argb_palette[bgi_fg_color]);

    counter++;

    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for

} // line_or ()

// -----

void line_not (int x1, int y1, int x2, int y2)
{
  int
    counter = 0, // # of pixel plotted
    dx = abs (x2 - x1),
    sx = x1 < x2 ? 1 : -1,
    dy = abs (y2 - y1),
    sy = y1 < y2 ? 1 : -1,
    err = (dx > dy ? dx : -dy) / 2,
    e2;

  for (;;) {

    if (SOLID_LINE == bgi_line_style.linestyle)
      putpixel_not (x1, y1, bgi_argb_palette[bgi_fg_color]);
    else
      if ((bgi_line_patterns[bgi_line_style.linestyle] >> counter % 16) & 1)
        putpixel_not (x1, y1, bgi_argb_palette[bgi_fg_color]);

    counter++;

    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for

} // line_not ()

// -----

void line_fill (int x1, int y1, int x2, int y2)
{
  // line function used for filling

  int
    dx = abs (x2 - x1),
    sx = x1 < x2 ? 1 : -1,
    dy = abs (y2 - y1),
    sy = y1 < y2 ? 1 : -1,
    err = (dx > dy ? dx : -dy) / 2,
    e2;

  for (;;) {

    ff_putpixel (x1, y1);

    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  } // for

} // line_fill ()

// -----

static int octant (int x, int y)
{
  // Returns the octant where x, y lies; used by line().

  if (x >= 0) { // octants 1, 2, 7, 8

    if (y >= 0)
      return (x > y) ? 1 : 2;
    else
      return (x > -y) ? 8 : 7;

  } // if (x > 0)

  else { // x < 0; 3, 4, 5, 6

    if (y >= 0)
      return (-x > y) ? 4 : 3;
    else
      return (-x > -y) ? 5 : 6;

  } // else

} // octant()

// -----

void line (int x1, int y1, int x2, int y2)
{
  // Draws a line between two specified points.

  check_initgraph ();

  int
    oct;

  check_initgraph ();

  // viewport
  x1 += vp.left;
  y1 += vp.top;
  x2 += vp.left;
  y2 += vp.top;

  switch (bgi_writemode) {

  case COPY_PUT:
    line_copy (x1, y1, x2, y2);
    break;

  case AND_PUT:
    line_and (x1, y1, x2, y2);
    break;

  case XOR_PUT:
    line_xor (x1, y1, x2, y2);
    break;

  case OR_PUT:
    line_or (x1, y1, x2, y2);
    break;

  case NOT_PUT:
    line_not (x1, y1, x2, y2);
    break;

  } // switch

  if (THICK_WIDTH == bgi_line_style.thickness) {

    oct = octant (x2 - x1, y1 - y2);

    switch (oct) { // draw thick line

    case 1:
    case 4:
    case 5:
    case 8:
      switch (bgi_writemode) {
      case COPY_PUT:
        line_copy (x1, y1 - 1, x2, y2 - 1);
        line_copy (x1, y1 + 1, x2, y2 + 1);
        break;
      case AND_PUT:
        line_and (x1, y1 - 1, x2, y2 - 1);
        line_and (x1, y1 + 1, x2, y2 + 1);
        break;
      case XOR_PUT:
        line_xor (x1, y1 - 1, x2, y2 - 1);
        line_xor (x1, y1 + 1, x2, y2 + 1);
        break;
      case OR_PUT:
        line_or (x1, y1 - 1, x2, y2 - 1);
        line_or (x1, y1 + 1, x2, y2 + 1);
        break;
      case NOT_PUT:
        line_not (x1, y1 - 1, x2, y2 - 1);
        line_not (x1, y1 + 1, x2, y2 + 1);
        break;
      } // switch
      break;

    case 2:
    case 3:
    case 6:
    case 7:
      switch (bgi_writemode) {
      case COPY_PUT:
        line_copy (x1 - 1, y1, x2 - 1, y2);
        line_copy (x1 + 1, y1, x2 + 1, y2);
        break;
      case AND_PUT:
        line_and (x1 - 1, y1, x2 - 1, y2);
        line_and (x1 + 1, y1, x2 + 1, y2);
        break;
      case XOR_PUT:
        line_xor (x1 - 1, y1, x2 - 1, y2);
        line_xor (x1 + 1, y1, x2 + 1, y2);
        break;
      case OR_PUT:
        line_or (x1 - 1, y1, x2 - 1, y2);
        line_or (x1 + 1, y1, x2 + 1, y2);
        break;
      case NOT_PUT:
        line_not (x1 - 1, y1, x2 - 1, y2);
        line_not (x1 + 1, y1, x2 + 1, y2);
        break;
      } // switch
      break;

    } // switch

  } // if (THICK_WIDTH...)

  update ();

} // line ()

// -----

static void line_fast (int x1, int y1, int x2, int y2)
{
  // Draws a line in fast mode

  int
    fastmode = bgi_fast_mode;

  bgi_fast_mode = SDL_TRUE; // draw in fast mode
  line (x1, y1, x2, y2);
  bgi_fast_mode = fastmode;

} // line_fast ()

// -----

void linerel (int dx, int dy)
{
  // Draws a line from the CP to a point that is (dx,dy)
  // pixels from the CP.

  check_initgraph ();

  line (bgi_cp_x, bgi_cp_y, bgi_cp_x + dx, bgi_cp_y + dy);
  bgi_cp_x += dx;
  bgi_cp_y += dy;

} // linerel ()

// -----

static void linerel_fast (int dx, int dy)
{
  // Fast version of linerel().

  int
    fastmode = bgi_fast_mode;

  bgi_fast_mode = SDL_TRUE; // draw in fast mode
  linerel (dx, dy);
  bgi_fast_mode = fastmode;

} // linerel ()

// -----

void lineto (int x, int y)
{
  // Draws a line from the CP to (x, y), then moves the CP to (dx, dy).

  check_initgraph ();

  line (bgi_cp_x, bgi_cp_y, x, y);
  bgi_cp_x = x;
  bgi_cp_y = y;

} // lineto ()

// -----

static void lineto_fast  (int x, int y)
{
  // Fast version of lineto().

  int
    fastmode = bgi_fast_mode;

  bgi_fast_mode = SDL_TRUE; // draw in fast mode
  line (bgi_cp_x, bgi_cp_y, x, y);
  bgi_cp_x = x;
  bgi_cp_y = y;
  bgi_fast_mode = fastmode;

} // lineto ()

// -----

void moveto (int x, int y)
{
  // Moves the CP to the position (x, y), relative to the viewport.

  check_initgraph ();

  bgi_cp_x = x;
  bgi_cp_y = y;

} // moveto ()

// -----

void moverel (int dx, int dy)
{
  // Moves the CP by (dx, dy) pixels.

  check_initgraph ();

  bgi_cp_x += dx;
  bgi_cp_y += dy;

} // moverel ()

// -----

static void _bar (int left, int top, int right, int bottom)
{
  // Used by drawchar_bitmap()

  int
    tmp, y;

  // like bar (), but uses bgi_fg_color

  tmp = bgi_fg_color;
  // setcolor (bgi_fg_color);
  for (y = top; y <= bottom; y++)
    line_fast (left, y, right, y);

  _setcolor (tmp);

} // _bar ()

// -----

static void drawchar_bitmap (unsigned char ch)
{
  // Draws an 8x8 bitmap character;
  // used by outtextxy ()

  unsigned char
    i, j, k;
  int
    x, y, tmp;

  tmp = bgi_bg_color;
  bgi_bg_color = bgi_fg_color; // for bar ()
  _setcolor (bgi_bg_color);

  // for each of the 8 bytes that make up the font

  for (i = 0; i < 8; i++) {

    k = bgi_bitmap_font[8*ch + i];

    // scan horizontal line

    for (j = 0; j < 8; j++)

      if ( (k << j) & 0x80) { // bit set to 1
        if (HORIZ_DIR == bgi_txt_style.direction) {
          x = bgi_cp_x + j * bgi_font_mag_x;
          y = bgi_cp_y + i * bgi_font_mag_y;
          // putpixel (x, y, bgi_fg_color);
          _bar (x, y, x + bgi_font_mag_x - 1, y + bgi_font_mag_y - 1);
        }
        else {
          x = bgi_cp_x + i * bgi_font_mag_y;
          y = bgi_cp_y - j * bgi_font_mag_x;
          // putpixel (bgi_cp_x + i, bgi_cp_y - j, bgi_fg_color);
          _bar (x, y, x + bgi_font_mag_x - 1, y + bgi_font_mag_y - 1);
        }
      } // if

  } // for i

  if (HORIZ_DIR == bgi_txt_style.direction)
    bgi_cp_x += 8*bgi_font_mag_x;
  else
    bgi_cp_y -= 8*bgi_font_mag_y;

  bgi_bg_color = tmp;

} // drawchar_bitmap ()

// -----

static void drawchar_internal (int ch)
{
  // Draws an character in internal font;
  // used by outtextxy ()

  int
    x, y,
    tmpx, tmpy,
    num;

  // TODO: char -> short int?

  const char
    *glyph;

  // fix signed chars
  if (ch < 0)
    ch += 256;
  ch -= 32; // glyph 0 = ASCII 32

  glyph = bgi_internal_font[ch];
  num = bgi_internal_font_size[ch];
  x = tmpx = bgi_cp_x;
  y = tmpy = bgi_cp_y;

  // internal fonts have 223 glyphs
  if (ch > 0 && ch < 224) {

    for (int i = 0; i < num - 2; i += 4) {

      // compatibility with Turbo C requires a vertical offset
      moveto (x, y + bgi_internal_offset);

      if (HORIZ_DIR == bgi_txt_style.direction) {
	moverel (bgi_font_mag_x * glyph[i],
		 bgi_font_mag_y * glyph[i + 1]);
	linerel_fast (bgi_font_mag_x *
		      (glyph[i + 2] - glyph[i]),
		      bgi_font_mag_y *
		      (glyph[i + 3] - glyph[i + 1]));
      }
      else { // VERT_DIR
	moverel (bgi_font_mag_y * glyph[i + 1],
		 - bgi_font_mag_x * glyph[i]);
	linerel_fast (bgi_font_mag_y *
		      (glyph[i + 3] - glyph[i + 1]),
		      - bgi_font_mag_x *
		      (glyph[i + 2] - glyph[i]));
      }

    } // for

  } // if

  if (HORIZ_DIR == bgi_txt_style.direction)
    tmpx += bgi_font_mag_x * bgi_internal_font_width[ch];
  else
    tmpy -= bgi_font_mag_x * bgi_internal_font_width[ch];

  moveto (tmpx, tmpy);

} // drawchar_internal ()

// -----

static void drawchar_chr (int ch)
{
  // Draws a character in CHR font;
  // used by outtextxy ()

  /*
  printf("drawchar_chr called for %d\n", ch);
  if (ch > 127)
    printf("char after 127.\n");
   */

  if (ch < 0)
    // We have to fix some signed chars...
    ch = 256 + ch;

  // printf("width: %d\n", chr_font->widths[ch]);

  int
    x, y,
    tmpx, tmpy;

  x = tmpx = bgi_cp_x;
  y = tmpy = bgi_cp_y;

  int height = bgi_font_mag_y*chr_font->capheight;

  display_glyph (chr_font, ch, bgi_font_mag_x, bgi_font_mag_y,
		 x, y + height, bgi_txt_style.direction);

  if (HORIZ_DIR == bgi_txt_style.direction)
    tmpx += bgi_font_mag_x * chr_font->widths[ch];
  else
    tmpy -= bgi_font_mag_y * chr_font->widths[ch];

  moveto (tmpx, tmpy);

} // drawchar_chr ()

// -----

int installuserfont (char *name)
{
  // Loads a CHR font from disk.
  // Undocumented feature: if the font file is not found, this
  // function returns LAST_SPEC_FONT.

  if (chr_next_font == MAX_FONTS)
    return grError;

  chr_fonts[chr_next_font] = load_font (name);

  if (!chr_fonts[chr_next_font])
    return LAST_SPEC_FONT;
  else {
    chr_next_font++;
    return chr_next_font - 1;
  }

} // installuserfont ()

// -----

void outtext (char *textstring)
{
  // Outputs textstring at the CP.

  check_initgraph ();

  int
    tmp_x = bgi_cp_x;

  outtextxy (bgi_cp_x, bgi_cp_y, textstring);

  // the CP has been advanced by drawchar_*()

  if ( (HORIZ_DIR == bgi_txt_style.direction) &&
       (LEFT_TEXT == bgi_txt_style.horiz))
    bgi_cp_x = tmp_x + textwidth (textstring);

} // outtext ()

// -----

void outtextxy (int x, int y, char *textstring)
{
  // Outputs textstring at (x, y).
  // What happens to the CP afterwards? Undocumented feature!

  check_initgraph ();

  int
    tmp_tck, // line thickness
    tmp_stl, // line style
    tmp_x = bgi_cp_x,
    tmp_y = bgi_cp_y,
    x1 = 0,
    y1 = 0,
    tw,
    th;

  tw = textwidth (textstring);
  if (0 == tw)
    return;

  th = textheight (textstring);

  if (HORIZ_DIR == bgi_txt_style.direction) {

    if (LEFT_TEXT == bgi_txt_style.horiz)
      x1 = x;

    if (CENTER_TEXT == bgi_txt_style.horiz)
      x1 = x - round (tw / 2.0) + 1;

    if (RIGHT_TEXT == bgi_txt_style.horiz)
      x1 = x - tw;

    if (CENTER_TEXT == bgi_txt_style.vert)
      y1 = y - th / 2;

    if (TOP_TEXT == bgi_txt_style.vert)
      y1 = y;

    if (BOTTOM_TEXT == bgi_txt_style.vert)
      y1 = y - th;

  }
  else { // VERT_DIR

    if (LEFT_TEXT == bgi_txt_style.horiz)
      y1 = y;

    if (CENTER_TEXT == bgi_txt_style.horiz)
      y1 = y + round (tw / 2.0) + 1;

    if (RIGHT_TEXT == bgi_txt_style.horiz)
      y1 = y + tw;

    if (CENTER_TEXT == bgi_txt_style.vert)
      x1 = x - th / 2;

    if (TOP_TEXT == bgi_txt_style.vert)
      x1 = x;

    if (BOTTOM_TEXT == bgi_txt_style.vert)
      x1 = x - th;

  } // VERT_DIR

  moveto (x1, y1);

  // if THICK_WIDTH, fallback to NORM_WIDTH
  tmp_tck = bgi_line_style.thickness;
  tmp_stl = bgi_line_style.linestyle;
  bgi_line_style.thickness = NORM_WIDTH;
  bgi_line_style.linestyle = SOLID_LINE;

  for (int i = 0; i < strlen (textstring); i++) {
    if (chr_font)
      drawchar_chr (textstring[i]);
    else if (DEFAULT_FONT == bgi_font)
      drawchar_bitmap (textstring[i]);
    else
      drawchar_internal (textstring[i]);
  }

  // restore status
  bgi_line_style.thickness = tmp_tck;
  bgi_line_style.linestyle = tmp_stl;
  bgi_cp_x = tmp_x;
  bgi_cp_y = tmp_y;

  update ();

} // outtextxy ()

// -----

void pieslice (int x, int y, int stangle, int endangle, int radius)
{
  // Draws and fills a pie slice centered at (x, y), with a radius
  // given by radius, traveling from stangle to endangle.

  // quick and dirty for now, Bresenham-based later (maybe)

  check_initgraph ();

  int
    angle;

  if (0 == radius || stangle == endangle)
    return;

  if (endangle < stangle)
    endangle += 360;

  if (0 == radius)
    return;

  bgi_last_arc.x = x;
  bgi_last_arc.y = y;
  bgi_last_arc.xstart = x + (radius * cos (stangle * PI_CONV));
  bgi_last_arc.ystart = y - (radius * sin (stangle * PI_CONV));
  bgi_last_arc.xend = x + (radius * cos (endangle * PI_CONV));
  bgi_last_arc.yend = y - (radius * sin (endangle * PI_CONV));

  for (angle = stangle; angle < endangle; angle++)
    line_fast (x + (radius * cos (angle * PI_CONV)),
               y - (radius * sin (angle * PI_CONV)),
               x + (radius * cos ((angle+1) * PI_CONV)),
               y - (radius * sin ((angle+1) * PI_CONV)));
  line_fast (x, y, bgi_last_arc.xstart, bgi_last_arc.ystart);
  line_fast (x, y, bgi_last_arc.xend, bgi_last_arc.yend);

  angle = (stangle + endangle) / 2;

  // !!! FIXME: what if we're trying to fill an already filled pieslice?
  floodfill (x + (radius * cos (angle * PI_CONV)) / 2,
             y - (radius * sin (angle * PI_CONV)) / 2,
             bgi_fg_color);

  update ();

} // pieslice ()

// -----

void putimage (int left, int top, void *bitmap, int op)
{
  // Puts the bit image pointed to by bitmap onto the screen.

  check_initgraph ();

  Uint32
    bitmap_w, bitmap_h, *tmp;
  int
    i = 2, x, y;

  tmp = bitmap;

  // get width and height info from bitmap
  memcpy (&bitmap_w, tmp, sizeof (Uint32));
  memcpy (&bitmap_h, tmp + 1, sizeof (Uint32));

  // put bitmap to the screen
  for (int yy = 0; yy < bitmap_h; yy++)
    for (int xx = 0; xx < bitmap_w; xx++) {

      x = left + vp.left + xx;
      y = top + vp.top + yy;

      switch (op) {

      case COPY_PUT:
        putpixel_copy (x, y, tmp[i++]);
        break;

      case AND_PUT:
        putpixel_and (x, y, tmp[i++]);
        break;

      case XOR_PUT:
        putpixel_xor (x, y, tmp[i++]);
        break;

      case OR_PUT:
        putpixel_or (x, y, tmp[i++]);
        break;

      case NOT_PUT:
        putpixel_not (x, y, tmp[i++]);
        break;

      } // switch

    } // for x

  update ();

} // putimage ()

// -----

static void putpixel_copy (int x, int y, Uint32 pixel)
{
  // plain putpixel - no logical operations

  // out of range?
  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;

  if (SDL_TRUE == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;

  PIXEL (x, y) = pixel;

  // we could use the native function:
  // SDL_RenderDrawPoint (bgi_renderer, x, y);
  // but strangely it's slower

} // putpixel_copy ()

// -----

static void putpixel_xor (int x, int y, Uint32 pixel)
{
  // XOR'ed putpixel

  // out of range?
  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;

  if (SDL_TRUE == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;

  PIXEL (x, y) ^= (pixel & 0x00ffffff);

} // putpixel_xor ()

// -----

static void putpixel_and (int x, int y, Uint32 pixel)
{
  // AND-ed putpixel

  // out of range?
  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;

  if (SDL_TRUE == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;

  PIXEL (x, y) &= pixel;

} // putpixel_and ()

// -----

static void putpixel_or (int x, int y, Uint32 pixel)
{
  // OR-ed putpixel

  // out of range?
  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;

  if (SDL_TRUE == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;

  PIXEL (x, y) |= (pixel & 0x00ffffff);

} // putpixel_or ()

// -----

static void putpixel_not (int x, int y, Uint32 pixel)
{
  // NOT-ed putpixel

  // out of range?
  if (x < 0 || x > bgi_maxx || y < 0 || y > bgi_maxy)
    return;

  if (SDL_TRUE == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;

  PIXEL (x, y) = ~ (pixel & 0x00ffffff);

} // putpixel_not ()

// -----

void putpixel (int x, int y, int color)
{
  // Plots a point at (x,y) in the color defined by 'color'.

  int
    tmpcolor;

  check_initgraph ();

  if (color < -1)
    color += 32768;

  x += vp.left;
  y += vp.top;

  // clip
  if (SDL_TRUE == vp.clip)
    if (x < vp.left || x > vp.right || y < vp.top || y > vp.bottom)
      return;

  // COLOR () or COLOR32 () or RGBPALETTE () set ARGB_FG_COL
  if (-1 == color) {
    tmpcolor = ARGB_FG_COL;
    bgi_argb_palette[tmpcolor] = bgi_argb_palette[ARGB_TMP_COL];
  }
  else {
    if (color > MAXCOLORS && ! bgi_use_tmp_color) {
      color %= MAXCOLORS;
      bgi_use_tmp_color = SDL_FALSE;
    }
    bgi_argb_mode = SDL_FALSE;
    tmpcolor = color;
  }

  switch (bgi_writemode) {

  case XOR_PUT:
    putpixel_xor  (x, y, bgi_argb_palette[tmpcolor]);
    break;

  case AND_PUT:
    putpixel_and  (x, y, bgi_argb_palette[tmpcolor]);
    break;

  case OR_PUT:
    putpixel_or   (x, y, bgi_argb_palette[tmpcolor]);
    break;

  case NOT_PUT:
    putpixel_not  (x, y, bgi_argb_palette[tmpcolor]);
    break;

  default:
  case COPY_PUT:
    putpixel_copy (x, y, bgi_argb_palette[tmpcolor]);

  } // switch

  update_pixel (x, y);

} // putpixel ()

// -----

void rectangle (int x1, int y1, int x2, int y2)
{
  // Draws a rectangle delimited by (left,top) and (right,bottom).

  check_initgraph ();

  line_fast (x1, y1, x2, y1);
  line_fast (x2, y1, x2, y2);
  line_fast (x2, y2, x1, y2);
  line_fast (x1, y2, x1, y1);

  update ();

} // rectangle ()

// -----

static void refresh_window (void)
{
  // Updates the screen.

  updaterect (0, 0, bgi_maxx, bgi_maxy);
  SDL_RaiseWindow (bgi_window);

} // refresh_window ()

// -----

void restorecrtmode (void)
{
  // Hides the graphics window.

  check_initgraph ();

  SDL_HideWindow (bgi_win[bgi_current_window]);
  bgi_window_is_hidden = SDL_TRUE;

} // restorecrtmode ()

// -----

void sector (int x, int y, int stangle, int endangle,
             int xradius, int yradius)
{
  // Draws and fills an elliptical pie slice centered at (x, y),
  // horizontal and vertical radii given by xradius and yradius,
  // traveling from stangle to endangle.

  // quick and dirty for now, Bresenham-based later (maybe)

  check_initgraph ();

  int
    angle, tmpcolor;

  if (0 == xradius && 0 == yradius)
    return;

  if (endangle < stangle)
    endangle += 360;

  // really needed?
  bgi_last_arc.x = x;
  bgi_last_arc.y = y;
  bgi_last_arc.xstart = x + (xradius * cos (stangle * PI_CONV));
  bgi_last_arc.ystart = y - (yradius * sin (stangle * PI_CONV));
  bgi_last_arc.xend = x + (xradius * cos (endangle * PI_CONV));
  bgi_last_arc.yend = y - (yradius * sin (endangle * PI_CONV));

  for (angle = stangle; angle < endangle; angle++)
    line_fast (x + (xradius * cos (angle * PI_CONV)),
               y - (yradius * sin (angle * PI_CONV)),
               x + (xradius * cos ((angle+1) * PI_CONV)),
               y - (yradius * sin ((angle+1) * PI_CONV)));
  line_fast (x, y, bgi_last_arc.xstart, bgi_last_arc.ystart);
  line_fast (x, y, bgi_last_arc.xend, bgi_last_arc.yend);

  tmpcolor = bgi_fg_color;
  _setcolor (bgi_fill_style.color);
  angle = (stangle + endangle) / 2;
  // find a point within the sector
  // !!! FIXME: what if we're trying to fill an already filled sector?
  floodfill (x + (xradius * cos (angle * PI_CONV)) / 2,
             y - (yradius * sin (angle * PI_CONV)) / 2,
             tmpcolor);

  update ();

} // sector ()

// -----

void setactivepage (int page)
{
  // Makes 'page' the active page for all subsequent graphics output.

  check_initgraph ();

  if (! bgi_fast_mode)
    bgi_blendmode = SDL_BLENDMODE_NONE; // like in Turbo C

  if (page > -1 && page < VPAGES) {
    bgi_ap = page;
    bgi_activepage[bgi_current_window] = bgi_vpage[bgi_ap]->pixels;
  }

} // setactivepage ()

// -----

void setallpalette (struct palettetype *palette)
{
  // Sets the current palette to the values given in palette.

  check_initgraph ();

  if (NULL == palette) {
    bgi_error_code = grError;
    return;
  }

  for (int i = 0; i < palette->size; i++)
    bgi_argb_palette[i] = palette->colors[i];
  
  // !!! The original setallpalette restored pixels onscreen too;
  // this is not possible with the current implementation.

} // setallpalette ()

// -----

void setaspectratio (int xasp, int yasp)
{
  // Changes the default aspect ratio of the graphics.
  // Actually, it does nothing; it makes no sense on modern hardware.

  return;

} // setaspectratio ()

// -----

void setbkcolor (int col)
{
  // Sets the current background color using the default palette.

  check_initgraph ();

  if (col < -1)
    col += 32768;

  // COLOR () or COLOR32 () or RGBPALETTE () set ARGB_BG_COL
  if (-1 == col) {
    bgi_bg_color = ARGB_BG_COL;
    bgi_argb_palette[ARGB_BG_COL] = bgi_argb_palette[ARGB_TMP_COL];
  }
  else {
    if (col > MAXCOLORS)
      col %= MAXCOLORS;
    bgi_argb_mode = SDL_FALSE;
    bgi_bg_color = col;
  }

  // this was undocumented!
  clearviewport ();

} // setbkcolor ()

// -----

void _setcolor (int col)
{
  // Sets the current drawing color using the default palette.

  check_initgraph ();

  if (col < -1)
    col += 32768;

  // COLOR () or COLOR32 () or RGBPALETTE () set ARGB_FG_COL
  if (-1 == col) {
    bgi_fg_color = ARGB_FG_COL;
    bgi_argb_palette[ARGB_FG_COL] = bgi_argb_palette[ARGB_TMP_COL];
  }
  else {
    if (col > MAXCOLORS && ! bgi_use_tmp_color) {
      col %= MAXCOLORS;
      bgi_use_tmp_color = SDL_FALSE;
    }
    bgi_argb_mode = SDL_FALSE;
    bgi_fg_color = col;
  }

} // _setcolor ()

// -----

void setcolor (int col)
{
  // Sets the current drawing color using the default palette.

  check_initgraph ();

  if (col < -1)
    col += 32768;

  // COLOR () or COLOR32 () or RGBPALETTE () set ARGB_FG_COL
  if (-1 == col) {
    bgi_fg_color = ARGB_FG_COL;
    bgi_argb_palette[ARGB_FG_COL] = bgi_argb_palette[ARGB_TMP_COL];
  }
  else {
    if (col > MAXCOLORS && ! bgi_use_tmp_color) {
      col %= MAXCOLORS;
      bgi_use_tmp_color = SDL_FALSE;
    }
    bgi_argb_mode = SDL_FALSE;
    bgi_fg_color = col;
  }

} // setcolor ()

// -----

void setfillpattern (char *upattern, int color)
{
  // Sets a user-defined fill pattern.

  check_initgraph ();

  if (color < -1)
    color += 32768;

  for (int i = 0; i < 8; i++)
    fill_patterns[USER_FILL][i] = (Uint8) *upattern++;

  // COLOR () or COLOR32 () or RGBPALETTE () set ARGB_FILL_COL
  if (-1 == color) {
    bgi_fill_color = ARGB_FILL_COL;
    bgi_argb_palette[bgi_fill_color] = bgi_argb_palette[ARGB_TMP_COL];
    bgi_fill_style.color = bgi_fill_color;
  }
  else {
    if (color > MAXCOLORS)
      color %= MAXCOLORS;
    bgi_argb_mode = SDL_FALSE;
    bgi_fill_style.color = color;
  }

  bgi_fill_style.pattern = USER_FILL;

} // setfillpattern ()

// -----

void setfillstyle (int pattern, int color)
{
  // Sets the fill pattern and fill color.

  check_initgraph ();

  if (pattern < 0 && pattern > USER_FILL) {
    bgi_error_code = grError;
    return;
  }

  // if (color < -1)
    // color += 32768;

  bgi_fill_style.pattern = pattern;

  // COLOR () or COLOR32 () or RGBPALETTE () set ARGB_FILL_COL
  if (-1 == color) {
    bgi_fill_color = ARGB_FILL_COL;
    bgi_argb_palette[bgi_fill_color] = bgi_argb_palette[ARGB_TMP_COL];
    bgi_fill_style.color = bgi_fill_color;
  }
  else {
    // if (color > MAXCOLORS)
      // color %= MAXCOLORS;
    bgi_argb_mode = SDL_FALSE;
    bgi_fill_style.color = color;
  }

} // setfillstyle ()

// -----

void setgraphmode (int mode)
{
  // Shows the window that was hidden by restorecrtmode ().

  check_initgraph ();

  // get rid og key presses, etc.
  SDL_PumpEvents ();
  SDL_FlushEvents (SDL_FIRSTEVENT, SDL_LASTEVENT);
  SDL_ShowWindow (bgi_win[bgi_current_window]);
  SDL_RestoreWindow (bgi_win[bgi_current_window]);
  bgi_window_is_hidden = SDL_FALSE;
  cleardevice ();

} // setgraphmode ()

// -----

void setlinestyle (int linestyle, unsigned upattern, int thickness)
{
  // Sets the line width and style for all lines drawn by line(),
  // lineto(), rectangle(), drawpoly(), etc.

  check_initgraph ();

  bgi_line_style.linestyle = linestyle;
  bgi_line_patterns[USERBIT_LINE] = bgi_line_style.upattern = upattern;
  bgi_line_style.thickness = thickness;

} // setlinestyle ()

// -----

void setpalette (int colornum, int color)
{
  // Changes the standard palette entry 'colornum' to 'color';
  // it also changes pixels onscreen.

  int
    x, y;

  Uint32
    oldcol, newcol;

  check_initgraph ();

  // handle negative colours?
  if (colornum == color
      || color < -1
      || colornum < -1
      || colornum > MAXCOLORS)
    return;

  oldcol = bgi_argb_palette[colornum];

  if (-1 == color) // user called COLOR()
    bgi_argb_palette[colornum] = bgi_argb_palette[ARGB_TMP_COL];
  else {
    if (SDL_FALSE == bgi_use_newpalette)
      // use the old (ugly) palette
      bgi_argb_palette[colornum] = bgi_orig_palette[color];
    else
      // use the new palette
      bgi_argb_palette[colornum] = bgi_std_palette[color];
  }

  newcol = bgi_argb_palette[colornum];

  for (x = 0; x < getmaxx (); x++)
    for (y = 0; y < getmaxy (); y++)
      if (oldcol == PIXEL (x, y))
	PIXEL (x, y) = newcol;

} // setpalette ()

// -----

void settextjustify (int horiz, int vert)
{
  // Sets text justification.

  check_initgraph ();

  if (HORIZ_DIR == bgi_txt_style.direction) {
    bgi_txt_style.horiz = horiz;
    bgi_txt_style.vert = vert;
  }
  else {
    bgi_txt_style.horiz = vert;
    bgi_txt_style.vert = horiz;
  }

} // settextjustify ()

// -----

void settextstyle (int font, int direction, int charsize)
{
  // Sets the text font, the direction of the text
  // (HORIZ DIR, VERT DIR), and the size of characters.

  check_initgraph ();

  bgi_font = font;
  chr_font = chr_fonts[font];

  bgi_txt_style.font = font;

  switch (font) {

  case TRIPLEX_FONT:
    bgi_internal_font = &trip[0];
    bgi_internal_font_size = &trip_size[0];
    bgi_internal_font_width = &trip_width[0];
    bgi_internal_font_height = trip_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -trip_desc_height; // was negative
    break;

  case SMALL_FONT:
    bgi_internal_font = &litt[0];
    bgi_internal_font_size = &litt_size[0];
    bgi_internal_font_width = &litt_width[0];
    bgi_internal_font_height = litt_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -litt_desc_height;
    break;

  case SANS_SERIF_FONT:
    bgi_internal_font = &sans[0];
    bgi_internal_font_size = &sans_size[0];
    bgi_internal_font_width = &sans_width[0];
    bgi_internal_font_height = sans_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -sans_desc_height;
    break;

  case GOTHIC_FONT:
    bgi_font = GOTHIC_FONT;
    bgi_internal_font = &goth[0];
    bgi_internal_font_size = &goth_size[0];
    bgi_internal_font_width = &goth_width[0];
    bgi_internal_font_height = goth_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -goth_desc_height;
    break;

  case SCRIPT_FONT:
    bgi_font = SCRIPT_FONT;
    bgi_internal_font = &scri[0];
    bgi_internal_font_size = &scri_size[0];
    bgi_internal_font_width = &scri_width[0];
    bgi_internal_font_height = scri_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -goth_desc_height;
    break;

  case SIMPLEX_FONT:
    bgi_font = SIMPLEX_FONT;
    bgi_internal_font = &simp[0];
    bgi_internal_font_size = &simp_size[0];
    bgi_internal_font_width = &simp_width[0];
    bgi_internal_font_height = simp_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -simp_desc_height;
    break;

  case TRIPLEX_SCR_FONT:
    bgi_font = TRIPLEX_SCR_FONT;
    bgi_internal_font = &tscr[0];
    bgi_internal_font_size = &tscr_size[0];
    bgi_internal_font_width = &tscr_width[0];
    bgi_internal_font_height = tscr_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -tscr_desc_height;
    break;

  case COMPLEX_FONT:
    bgi_font = COMPLEX_FONT;
    bgi_internal_font = &lcom[0];
    bgi_internal_font_size = &lcom_size[0];
    bgi_internal_font_width = &lcom_width[0];
    bgi_internal_font_height = lcom_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -lcom_desc_height;
    break;

  case EUROPEAN_FONT:
    bgi_internal_font = &euro[0];
    bgi_internal_font_size = &euro_size[0];
    bgi_internal_font_width = &euro_width[0];
    bgi_internal_font_height = euro_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -euro_desc_height;
    break;

  case BOLD_FONT:
    bgi_internal_font = &bold[0];
    bgi_internal_font_size = &bold_size[0];
    bgi_internal_font_width = &bold_width[0];
    bgi_internal_font_height = bold_height;
    bgi_internal_offset = 0;
    bgi_internal_font_desc = -bold_desc_height;
    break;

  case DEFAULT_FONT:
  default:
      bgi_internal_offset = 0;
    break;

  } // switch

  // undocumented feature; please see installuserfont()
  if (LAST_SPEC_FONT == font && NULL == chr_font)
    bgi_font = DEFAULT_FONT;

  if (font > LAST_SPEC_FONT) { // use a loaded CHR font
    chr_font = chr_fonts[font];
    if (NULL == chr_font) {
      char str[100];
      bgi_error_code = grInvalidFontNum;
      sprintf (str, "BGI error: %s\n", grapherrormsg (bgi_error_code));
      fprintf (stderr, "%s", str);
      showerrorbox (str);
      exit (1);
    }
  }

  if (charsize > 10)
    charsize = 10; // as per specs

  if (USER_CHAR_SIZE == charsize) {
    bgi_font_mag_x = bgi_user_size_x;
    bgi_font_mag_y = bgi_user_size_y;
  }
  else
    if (font >= LAST_SPEC_FONT) {
      // just an estimate: precise glyph metric to be implemented
      bgi_font_mag_x = bgi_font_mag_y =
	(float)(charsize)*((float)(charsize)/27.0 - 1/54.0) + 26.0/54.0;
    }
  else
    bgi_font_mag_x = bgi_font_mag_y =
    bgi_font_magnification[bgi_font][charsize];

  if (VERT_DIR == direction)
    bgi_txt_style.direction = VERT_DIR;
  else
    bgi_txt_style.direction = HORIZ_DIR;

  bgi_txt_style.charsize = charsize;
  bgi_internal_offset *= charsize;

} // settextstyle ()

// -----

void setusercharsize (int multx, int divx, int multy, int divy)
{
  // Changes the font width and height.

  check_initgraph ();

  bgi_user_size_x = (float)multx / (float)divx;
  bgi_user_size_y = (float)multy / (float)divy;

  // According to the docs:
  // "When charsize equals 0, the output functions outtext and
  // outtextxy magnify the stroked font text using either the default
  // character magnification factor (4) or the user-defined character
  // size given by setusercharsize."
  // But Turbo C++ 1.01 always uses magnification.
  //
  // if (USER_CHAR_SIZE == bgi_txt_style.charsize) {
    bgi_font_mag_x = bgi_user_size_x;
    bgi_font_mag_y = bgi_user_size_y;
  // }

} // setusercharsize ()

// -----

void setviewport (int left, int top, int right, int bottom, int clip)
{
  // Sets the current viewport for graphics output.

  check_initgraph ();

  if (left < 0 || right > bgi_maxx || top < 0 || bottom > bgi_maxy)
    return;

  vp.left = left;
  vp.top = top;
  vp.right = right;
  vp.bottom = bottom;
  vp.clip = clip;
  // relative to viewport (left, top) corner
  bgi_cp_x = 0;
  bgi_cp_y = 0;

} // setviewport ()

// -----

void setvisualpage (int page)
{
  // Sets the visual graphics page number.

  check_initgraph ();

  if (page > -1 && page < bgi_np + 1) {
    bgi_vp = page;
    bgi_visualpage[bgi_current_window] = bgi_vpage[bgi_vp]->pixels;
  }

  // always!
  refresh ();

} // setvisualpage ()

// -----

void setwritemode (int mode)
{
  // Sets the writing mode for line drawing. 'mode' can be COPY PUT,
  // XOR PUT, OR PUT, AND PUT, and NOT PUT.

  check_initgraph ();

  bgi_writemode = mode;

} // setwritemode ()

// -----

int textheight (char *textstring)
{
  // Returns the height in pixels of a string.

  check_initgraph ();

  int
    height;

  // FIXME: take care of loaded fonts.

  if (DEFAULT_FONT == bgi_font)
    height = bgi_font_mag_y * bgi_bitmap_font_height;
  else
    height = bgi_font_mag_y * bgi_internal_font_height;

  return height;

} // textheight ()

// -----

int textwidth (char *textstring)
{
  // Returns the width in pixels of a string.

  check_initgraph ();

  int
    ch;
  float
    width = 0.0;

  if (DEFAULT_FONT == bgi_font) // 8x8 bitmap font
    width = (strlen (textstring) *
	     bgi_font_mag_x * bgi_bitmap_font_width);
  else
    if (chr_font) {
      for (int i = 0; textstring[i] != '\0'; i++) {
	ch = textstring[i];
	if (ch < 0)
	  ch = 256 + ch;
	//printf("Calculating width for %d: %c\n", ch, ch);
	width += bgi_font_mag_x * (float)chr_font->widths[ch];
	/*
	 printf("mag_x: %f width: %d, Total: %f\n",
	 bgi_font_mag_x, chr_font->widths[ch], width);
	 */
      } // for
    } // if
  else // internal font
    for (int i = 0; textstring[i] != '\0'; i++) {
      ch = textstring[i] - 32;
      if (ch < 0)
	ch += 256;
      width += bgi_font_mag_x * bgi_internal_font_width[ch];
    }

  return (int)width;

} // textwidth ()

// -----

// ===== SDL_BGI EXTENSIONS =====

int ALPHA_VALUE (int color)
{
  // Returns the alpha (transparency) component of 'color' in the
  // ARGB palette.

  return ((bgi_argb_palette[BGI_COLORS + TMP_COLORS + color] >> 24) & 0xFF);

} // ALPHA_VALUE ()

// -----

int BLUE_VALUE (int color)
{
  // Returns the blue component of 'color' in the ARGB palette

  return (bgi_argb_palette[BGI_COLORS + TMP_COLORS + color] & 0xFF);

} // BLUE_VALUE ()

// -----

int COLOR (int r, int g, int b)
{
  // Can be used as an argument for setcolor() and setbkcolor()
  // to set an ARGB color.

  bgi_use_tmp_color = SDL_TRUE;
  bgi_argb_palette[ARGB_TMP_COL] = 0xff000000 | r << 16 | g << 8 | b;
  return -1;

} // COLOR ()

// -----

int COLOR32 (Uint32 color)
{
  // Can be used as an argument for setcolor() and setbkcolor()
  // to set an ARGB color.

  bgi_use_tmp_color = SDL_TRUE;
  bgi_argb_palette[ARGB_TMP_COL] = color;
  return -1;

} // COLOR32 ()

// -----

int GREEN_VALUE (int color)
{
  // Returns the green component of 'color' in the ARGB palette

  return ((bgi_argb_palette[BGI_COLORS + TMP_COLORS + color] >> 8) & 0xFF);

} // GREEN_VALUE ()

// -----

int IS_BGI_COLOR (int color)
{
  // Returns 1 if 'color' belongs to the default palette

  return (! bgi_argb_mode && color < ARGB_TMP_COL + 1);

} // IS_BGI_COLOR ()

// -----

int IS_RGB_COLOR (int color)
{
  // Returns 1 if 'color' belongs to the ARGB palette. The 'color'
  // argument is actually redundant

  return bgi_argb_mode;

} // IS_RGB_COLOR ()

// -----

int RED_VALUE (int color)
{
  // Returns the red component of 'color' in the ARGB palette

  return ((bgi_argb_palette[BGI_COLORS + TMP_COLORS + color] >> 16) & 0xFF);

} // RED_VALUE ()

// -----

int RGBPALETTE (int color)
{
  // Can be used as an argument for setcolor() and setbkcolor()
  // to set an ARGB color.

  bgi_use_tmp_color = SDL_TRUE;
  bgi_argb_palette[ARGB_TMP_COL] =
    bgi_argb_palette[BGI_COLORS + TMP_COLORS + color];
  return -1;

} // RGBPALETTE ()

// -----

void fputpixel (int x, int y)
{
  // like putpixel (), but not immediately displayed

  check_initgraph ();

  // viewport range is taken care of by this function only,
  // since all others use it to draw.

  x += vp.left;
  y += vp.top;

  switch (bgi_writemode) {

  case XOR_PUT:
    putpixel_xor  (x, y, bgi_argb_palette[bgi_fg_color]);
    break;

  case AND_PUT:
    putpixel_and  (x, y, bgi_argb_palette[bgi_fg_color]);
    break;

  case OR_PUT:
    putpixel_or   (x, y, bgi_argb_palette[bgi_fg_color]);
    break;

  case NOT_PUT:
    putpixel_not  (x, y, bgi_argb_palette[bgi_fg_color]);
    break;

  default:
  case COPY_PUT:
    putpixel_copy (x, y, bgi_argb_palette[bgi_fg_color]);

  } // switch

} // putpixel ()

// -----

void closewindow (int id)
{
  // Closes a window.

  check_initgraph ();

  if (SDL_FALSE == bgi_active_windows[id]) {
    char str[40];
    sprintf (str, "Window %d does not exist.\n", id);
    fprintf (stderr, "%s", str);
    showerrorbox (str);
    return;
  }

  SDL_DestroyTexture (bgi_txt[id]);
  SDL_DestroyRenderer (bgi_rnd[id]);
  SDL_DestroyWindow (bgi_win[id]);
  bgi_active_windows[id] = SDL_FALSE;
  bgi_num_windows--;

} // closewindow ()

// -----

char *colorname (int color)
{
  switch (color) {
    
  case BLACK:
    return "BLACK";
    break;
    
  case BLUE:
    return "BLUE";
    break;
    
  case GREEN:
    return "GREEN";
    break;
    
  case CYAN:
    return "CYAN";
    break;
    
  case RED:
    return "RED";
    break;
    
  case MAGENTA:
    return "MAGENTA";
    break;

  case BROWN:
    return "BROWN";
    break;
    
  case LIGHTGRAY:
    return "LIGHTGRAY";
    break;
    
  case DARKGRAY:
    return "DARKGRAY";
    break;
    
  case LIGHTBLUE:
    return "LIGHTBLUE";
    break;
    
  case LIGHTGREEN:
    return "LIGHTGREEN";
    break;
    
  case LIGHTCYAN:
    return "LIGHTCYAN";
    break;
    
  case LIGHTRED:
    return "LIGHTRED";
    break;
    
  case LIGHTMAGENTA:
    return "LIGHTMAGENTA";
    break;
    
  case YELLOW:
    return "YELLOW";
    break;

  case WHITE:
    return "WHITE";
    break;

  default:
    return "(none)";
    
  }

} // colorname ()

// -----

int doubleclick (void)
{

  if (2 == bgi_mouse.clicks) {
    bgi_mouse.clicks = 0;
    return SDL_TRUE;
  }
  else
    return SDL_FALSE;

} // doubleclick ()

// -----

int edelay (int msec)
{
  // Waits for 'msec' milliseconds and returns SDL_FALSE if no
  // event occurred in the meantime, SDL_TRUE otherwise

  Uint32
    stop;
  int
    ev = SDL_FALSE;

  update ();
  stop = SDL_GetTicks () + msec;

  do {

    if (event ())
      ev = SDL_TRUE;

  } while (SDL_GetTicks () < stop);

  return ev;

} // edelay ()

// -----

int event (void)
{
  // Returns SDL_TRUE if an event has occurred, SDL_FALSE otherwise.

  SDL_Event
    event;

  update ();
  
  if (SDL_PollEvent (&event)) {

    bgi_last_event = event.type;

    switch (bgi_last_event) {

      // no SDL_WINDOWEVENT here

      case SDL_KEYDOWN:
      bgi_last_key_pressed = (int) event.key.keysym.sym;
      SDL_PushEvent (&event); // don't disrupt the keyboard
      bgi_mouse.x = bgi_mouse.y = -1;
      return SDL_TRUE;
      break;

      case SDL_MOUSEMOTION:
      bgi_mouse.x = event.motion.x;
      bgi_mouse.y = event.motion.y;
      bgi_last_key_pressed = -1;
      return SDL_TRUE;

      case SDL_MOUSEBUTTONDOWN:
      bgi_mouse.btn = event.button.button;
      bgi_mouse.clicks = event.button.clicks;
      SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y);
      return SDL_TRUE;
      break;

      case SDL_MOUSEBUTTONUP:
      bgi_mouse.btn = event.button.button;
      bgi_mouse.clicks = 0;
      SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y);
      return SDL_TRUE;
      break;

      case SDL_MOUSEWHEEL:
      bgi_mouse.wheel = (event.wheel.y > 0) ?
	WM_WHEELUP : WM_WHEELDOWN;
      SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y);
      return SDL_TRUE;
      break;
      
      case SDL_QUIT:
      return SDL_TRUE;
      break;
      
      default:
        ;

    } // switch

  } // if

  return SDL_FALSE;

} // event ()

// -----

int eventtype (void)
{
  // Returns the type of the last event:
  // SDL_KEYDOWN, SDL_MOUSEMOTION, SDL_MOUSEBUTTONDOWN,
  // SDL_MOUSEBUTTONUP, SDL_MOUSEWHEEL, SDL_QUIT

  return (bgi_last_event);

} // eventtype ()

// -----

void getbuffer (Uint32 *buffer)
{
  // Copies the current window contents to 'buffer', which must have
  // the same width and height as the current window.

  memcpy ((void *) buffer,
	  bgi_activepage[bgi_current_window],
	  (bgi_maxy + 1) * (bgi_maxx + 1) * sizeof (Uint32));

} // getbuffer ()

// -----

int getclick ()
{
  // Waits for a button click.

  int
    click;

  while (1) {

    click = mouseclick ();
    if (WM_LBUTTONDOWN == click ||
	WM_MBUTTONDOWN == click ||
	WM_RBUTTONDOWN == click)
    return bgi_mouse.btn;
  }

} // getclick ()

// -----

int getcurrentwindow (void)
{
  // Returns the ID of current window

  return bgi_current_window;

} // getcurrentwindow ()

// -----

int getevent (void)
{
  // Waits for a keypress or mouse click, and returns
  // the code of the mouse event or key that was pressed.

  SDL_Event
    event;

  while (SDL_WaitEvent (&event)) {

    bgi_last_event = event.type;

    switch (bgi_last_event) {

    case SDL_WINDOWEVENT:

      switch (event.window.event) {

      case SDL_WINDOWEVENT_SHOWN:
      case SDL_WINDOWEVENT_EXPOSED:
	refresh_window ();
	break;

      case SDL_WINDOWEVENT_CLOSE:
	bgi_last_event = QUIT;
	return SDL_QUIT;
	break;

      default:
	;

      } // switch (event.window.event)
      break;

#if 0
    case SDL_MOUSEMOTION:
      SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y);
      return SDL_MOUSEMOTION;
      break;
#endif

    case SDL_KEYDOWN:
      bgi_last_key_pressed = (int) event.key.keysym.sym;
      bgi_mouse.x = bgi_mouse.y = -1;
      // logically, it should be SDL_KEYDOWN
      return bgi_last_key_pressed;
      break;

    case SDL_MOUSEBUTTONDOWN:
      bgi_mouse.btn = event.button.button;
      bgi_mouse.clicks = event.button.clicks;
      SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y);
      // logically, it should be SDL_MOUSEBUTTONDOWN
      return bgi_mouse.btn;
      break;

    case SDL_MOUSEWHEEL:
      SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y);
      return (event.wheel.y > 0) ?
	WM_WHEELUP : WM_WHEELDOWN;
      break;

    default:
      ;

    } // switch (bgi_last_event)

  } // while

  return 0;

} // getevent ()

// -----

static void _getclick (int btn)
{
  // Waits for the specified 'btn' click.

  // temporary slow mode - I don't know why
  // Emscripten wants this in order to work

  int
    tmp = bgi_fast_mode;
  bgi_fast_mode = SDL_FALSE;
  
  SDL_PumpEvents ();

  // click
  while (1) {
    event ();
    if (SDL_MOUSEBUTTONDOWN == bgi_last_event &&
        bgi_mouse.btn == btn)
      break;
  }
  
  // release
  while (1) {
    event ();
    if (SDL_MOUSEBUTTONUP == bgi_last_event)
      break;
  }
  
  bgi_fast_mode = tmp;
  
} // _getclick ()

// -----

void getleftclick (void)
{

  _getclick (WM_LBUTTONDOWN);
  
} // getleftclick ()

// -----

void getlinebuffer (int y, Uint32 *linebuffer)
{
  // Copies the screen line at 'y' coordinate to 'linebuffer';
  // faster than getpixel ()

  memcpy ((void *) linebuffer,
	  (void *) bgi_activepage[bgi_current_window] +
	  y * (bgi_maxx + 1) * sizeof (Uint32),
	  (bgi_maxx + 1) * sizeof (Uint32));

} // getbuffer ()

// -----

void getmiddleclick (void)
{
  // Waits for a middle button click + release.
  
  _getclick (WM_MBUTTONDOWN);
  
} // getleftclick ()

// -----

void getmouseclick (int kind, int *x, int *y)
{
  // Sets the x, y coordinates of the last 'kind' button click
  // expected by ismouseclick().

  if (kind == bgi_mouse.btn) {
    *x = bgi_mouse.x;
    *y = bgi_mouse.y;
  }
  else
    *x = *y = -1;

} // getmouseclick ()

// -----

void getrgbpalette (struct rgbpalettetype *palette, int size)
{
  // Fills the rgbpalettetype structure pointed by palette with
  // information about the current RGB palette's size and colors.

  check_initgraph ();

  if (NULL == palette) {
    bgi_error_code = grError;
    return;
  }

  palette->size = size;

  for (int col = 0; col < size; col++)
    palette->colors[col] =
    bgi_argb_palette[BGI_COLORS + TMP_COLORS + col];

} // getrgbpalette ()

// -----

int getrgbpalettesize (void)
{
  // Returns the current size of the ARGB palette.

  return PALETTE_SIZE;
}

// -----


void getrightclick (void)
{
  // Waits for a right button click + release.
  
  _getclick (WM_RBUTTONDOWN);
  
} // getleftclick ()

// -----

void getscreensize (int *x, int *y)
{
  // Reports the screen size, regardless of the window dimensions.

  if (SDL_Init (SDL_INIT_VIDEO) != 0) {
    SDL_Log ("SDL_Init() failed: %s", SDL_GetError ());
    showerrorbox ("SDL_Init() failed");
    exit (1);
  }

  SDL_DisplayMode mode =
  { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

  if (SDL_GetDisplayMode (0, 0, &mode) != 0) {
    SDL_Log ("SDL_GetDisplayMode() failed: %s", SDL_GetError ());
    showerrorbox ("SDL_GetDisplayMode() failed");
    exit (1);
  }

  *x = mode.w;
  *y = mode.h;

} // getscreensize ()

// -----

int initwindow (int width, int height)
{
  // Initializes the graphics system, opening a 'width' x 'height' window.

  int
    display_count = 0,
    page;

  static int
    first_run = SDL_TRUE,  // first run of initwindow ()
    fullscreen = -1;       // fullscreen window already created?

  char
    str[100];

  // let's be proactive
  bgi_error_code = grOk;

  // fix wrong values
  if (width < 0)
    width = 0;
  if (height < 0)
    height = 0;

  SDL_DisplayMode mode =
  { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

  if (SDL_TRUE == first_run) {
    first_run = SDL_FALSE;
     // initialise SDL2
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
      SDL_Log ("SDL_Init() failed: %s", SDL_GetError ());
      showerrorbox ("SDL_Init() failed");
      exit (1);
    }
    // initialise bgi_active_windows[]
    for (int i = 0; i < NUM_BGI_WIN; i++)
      bgi_active_windows[i] = SDL_FALSE;
  }

  // any display available?
  if ((display_count = SDL_GetNumVideoDisplays ()) < 1) {
    SDL_Log ("SDL_GetNumVideoDisplays() returned: %i\n", display_count);
    showerrorbox ("SDL_GetNumVideoDisplays() failed");
    exit (1);
  }

  // get display mode
  if (SDL_GetDisplayMode (0, 0, &mode) != 0) {
    SDL_Log ("SDL_GetDisplayMode() failed: %s", SDL_GetError ());
    showerrorbox ("SDL_GetDisplayMode() failed");
    exit (1);
  }

  // find a free ID for the window
  do {
    bgi_current_window++;
    if (SDL_FALSE == bgi_active_windows[bgi_current_window])
      break;
  } while (bgi_current_window < NUM_BGI_WIN);

  if (bgi_current_window < NUM_BGI_WIN) {
    bgi_active_windows[bgi_current_window] = SDL_TRUE;
    bgi_num_windows++;
  }
  else {
    sprintf (str, "Could not create new window.\n");
    fprintf (stderr, "%s", str);
    showerrorbox (str);
    return -1;
  }

  // check if a fullscreen window has been created
  if (SDL_TRUE == fullscreen) {
    sprintf (str, "Fullscreen window already exists.\n");
    fprintf (stderr, "%s", str);
    showerrorbox (str);
    return -1;
  }

  // take note of window size
  bgi_maxx = width - 1;
  bgi_maxy = height - 1;

  if (SDL_FALSE == bgi_fast_mode) {   // called by initgraph ()
    if (0 == width || 0 == height) {  // fullscreen
      bgi_maxx = mode.w - 1;
      bgi_maxy = mode.h - 1;
      bgi_window_flags = bgi_window_flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
      fullscreen = SDL_TRUE;
      bgi_gm = SDL_FULLSCREEN;
    }
    else {
      bgi_maxx = width - 1;
      bgi_maxy = height - 1;
      fullscreen = SDL_FALSE;
    }
  } // if (SDL_FALSE == bgi_fast_mode)
  else { // initwindow () called directly
    if (width > mode.w || height > mode.h) {
      // window too large - force fullscreen
      width = 0;
      height = 0;
    }
    if ( (0 != width) && (0 != height) ) {
      bgi_maxx = width - 1;
      bgi_maxy = height - 1;
      fullscreen = SDL_FALSE;
    }
    else { // 0, 0: fullscreen
      bgi_maxx = mode.w - 1;
      bgi_maxy = mode.h - 1;
      bgi_window_flags = bgi_window_flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
      fullscreen = SDL_TRUE;
    }
  }

  bgi_win[bgi_current_window] =
    SDL_CreateWindow (bgi_win_title,
                      bgi_window_x,
                      bgi_window_y,
                      bgi_maxx + 1,
                      bgi_maxy + 1,
                      bgi_window_flags);

  // is the window OK?
  if (NULL == bgi_win[bgi_current_window]) {
    SDL_Log ("Could not create window: %s\n", SDL_GetError ());
    exit (1);
  }

  // window ok; create renderer
  bgi_rnd[bgi_current_window] =
    SDL_CreateRenderer (bgi_win[bgi_current_window], -1,
			// slow but guaranteed to exist. OR'ing
			// other options or setting this flag to
			// 0 causes segfault on my NVIDIA GeForce
			// 920M and in Virtualbox.
                        SDL_RENDERER_SOFTWARE);
  if (NULL == bgi_rnd[bgi_current_window]) {
    SDL_Log ("Could not create renderer: %s\n", SDL_GetError ());
    exit (1);
  }

  // finally, create the texture
  bgi_txt[bgi_current_window] =
    SDL_CreateTexture (bgi_rnd[bgi_current_window],
                       SDL_PIXELFORMAT_ARGB8888,
		       SDL_TEXTUREACCESS_STREAMING,
                       bgi_maxx + 1,
                       bgi_maxy + 1);
  if (NULL == bgi_txt[bgi_current_window]) {
    SDL_Log ("Could not create texture: %s\n", SDL_GetError ());
    exit (1);
  }

  // visual pages
  for (page = 0; page < VPAGES; page++) {
    bgi_vpage[page] =
      SDL_CreateRGBSurface (0, mode.w, mode.h, 32, 0, 0, 0, 0);
    if (NULL == bgi_vpage[page]) {
      SDL_Log ("Could not create surface for visual page %d.", page);
      showerrorbox ("Could not create surface for visual page");
      break;
    }
    else
      bgi_np++;
  }

  bgi_window = bgi_win[bgi_current_window];
  bgi_renderer = bgi_rnd[bgi_current_window];
  bgi_texture = bgi_txt[bgi_current_window];

  bgi_activepage[bgi_current_window] =
    bgi_visualpage[bgi_current_window] =
    bgi_vpage[0]->pixels;
  bgi_ap = bgi_vp = 0;

  graphdefaults ();

  // check for the environment variable 'SDL_BGI_RATE'
  // and act accordingly

  char *speed = getenv ("SDL_BGI_RATE");

  if (NULL == speed) // variable does not exist
    speed = "compatible";
  else {
    if (0 == strcmp ("auto", speed))
      sdlbgiauto ();

    bgi_refresh_rate = atoi (speed);
    if (0 != bgi_refresh_rate) // implies auto mode
      sdlbgiauto ();
  }

  // any other value of SDL_BGI_RATE triggers
  // "compatible" mode by default

  // fix fonts magnification

  short int default_h[] =
  {8, 8, 16, 24, 32, 40, 40, 48, 56, 64, 72, 80};
  short int trip_h[] =
  {31, 18, 20, 23, 31, 41, 51, 62, 77, 93, 124};
  short int litt_h[] =
  {9, 5, 6, 6, 9, 12, 15, 18, 22, 27, 36};
  short int sans_h[] =
  {32, 19, 21, 24, 32, 42, 53, 64, 80, 96, 128};
  short int goth_h[] =
  {32, 19, 21, 24, 32, 42, 53, 64, 80, 96, 128};
  short int scri_h[] =
  {37, 22, 24, 27, 37, 49, 61, 74, 92, 111, 148};
  short int simp_h[] =
  {35, 21, 23, 26, 35, 46, 58, 70, 87, 105, 140};
  short int tscr_h[] =
  {31, 18, 20, 23, 31, 41, 51, 62, 77, 93, 124};
  short int lcom_h[] =
  {35, 21, 23, 26, 35, 46, 58, 70, 87, 105, 140};
  short int euro_h[] =
  {55, 33, 36, 41, 55, 73, 91, 110, 137, 165, 220};
  short int bold_h[] =
  {60, 36, 40, 45, 60, 80, 100, 120, 150, 180, 240};

  // set font magnification
  for (int i = 0; i < FONT_SIZES; i++) {

    bgi_font_magnification[DEFAULT_FONT][i] =
    default_h[i] / 8; // bitmap font height

    bgi_font_magnification[TRIPLEX_FONT][i] =
    (float) trip_h[i] / (float) trip_height;

    bgi_font_magnification[SMALL_FONT][i] =
    (float) litt_h[i] / (float) litt_height;

    bgi_font_magnification[SANS_SERIF_FONT][i] =
    (float) sans_h[i] / (float) sans_height;

    bgi_font_magnification[GOTHIC_FONT][i] =
    (float) goth_h[i] / (float) goth_height;

    bgi_font_magnification[SCRIPT_FONT][i] =
    (float) scri_h[i] / (float) scri_height;

    bgi_font_magnification[SIMPLEX_FONT][i] =
    (float) simp_h[i] / (float) simp_height;

    bgi_font_magnification[TRIPLEX_SCR_FONT][i] =
    (float) tscr_h[i] / (float) tscr_height;

    bgi_font_magnification[COMPLEX_FONT][i] =
    (float) lcom_h[i] / (float) lcom_height;

    bgi_font_magnification[EUROPEAN_FONT][i] =
    (float) euro_h[i] / (float) euro_height;

    bgi_font_magnification[BOLD_FONT][i] =
    (float) bold_h[i] / (float) bold_height;

  } // for i
  
  return bgi_current_window;
  
} // initwindow ()

// -----

int ismouseclick (int btn)
{
  // Returns 1 if the 'btn' mouse button is being clicked.

  SDL_PumpEvents ();

  switch (btn) {

  case SDL_BUTTON_LEFT:
    bgi_mouse.btn = SDL_BUTTON_LEFT;
    return (SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y) &
	    SDL_BUTTON(1));
    break;

  case SDL_BUTTON_MIDDLE:
    bgi_mouse.btn = SDL_BUTTON_MIDDLE;
    return (SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y) &
	    SDL_BUTTON(2));
    break;

  case SDL_BUTTON_RIGHT:
    bgi_mouse.btn = SDL_BUTTON_RIGHT;
    return (SDL_GetMouseState (&bgi_mouse.x, &bgi_mouse.y) &
	    SDL_BUTTON(3));
    break;

  } // switch

  return SDL_FALSE;

} // ismouseclick ()

// -----

int kdelay (int msec)
{
  // Waits for 'msec' milliseconds and returns SDL_FALSE if no
  // keyboard event occurred in the meantime, SDL_TRUE otherwise

  Uint32
    stop;
  int
    ev = SDL_FALSE;

  update ();
  stop = SDL_GetTicks () + msec;

  do {

    if (xkbhit ())
      ev = SDL_TRUE;

  } while (SDL_GetTicks () < stop);

  return ev;

} // kdelay ()

// -----

int mouseclick (void)
{
  // Returns the code of the current mouse event
  // (WM_MOUSEMOVE, WM_LBUTTONDOWN etc.),
  // or SDL_FALSE if no mouse event is occurring.

  int
    type;
  
  int
    tmp = bgi_fast_mode;
  bgi_fast_mode = SDL_FALSE;
  
  // mouse coords and buttons are set by event ()
  
  if (event ()) {
  
    type = bgi_last_event;
  
    switch (type) {
    
    case SDL_MOUSEBUTTONDOWN:
      bgi_fast_mode = tmp;
      if (2 == bgi_mouse.clicks)
        switch (bgi_mouse.btn) {
	case SDL_BUTTON_LEFT:
          return WM_LBUTTONDBLCLK;
          break;
	case SDL_BUTTON_MIDDLE:
          return WM_MBUTTONDBLCLK;
          break;
	case SDL_BUTTON_RIGHT:
          return WM_RBUTTONDBLCLK;
          break;
        } // switch 
      else // single click
        switch (bgi_mouse.btn) {
	case SDL_BUTTON_LEFT:
          return WM_LBUTTONDOWN;
          break;
	case SDL_BUTTON_MIDDLE:
          return WM_MBUTTONDOWN;
          break;
	case SDL_BUTTON_RIGHT:
          return WM_RBUTTONDOWN;
          break;
        }
      break;
      
    case SDL_MOUSEBUTTONUP:
      bgi_fast_mode = tmp;
      switch (bgi_mouse.btn) {
      case SDL_BUTTON_LEFT:
        return WM_LBUTTONUP;
        break;
      case SDL_BUTTON_MIDDLE:
        return WM_MBUTTONUP;
        break;
      case SDL_BUTTON_RIGHT:
        return WM_RBUTTONUP;
        break;
      } // switch 
      break;
      
    case SDL_MOUSEMOTION:
      bgi_fast_mode = tmp;
      return SDL_MOUSEMOTION;
      break;

    case SDL_MOUSEWHEEL:
      bgi_fast_mode = tmp;
      return bgi_mouse.wheel;
      break;
      
    default:
      ;
      
    } // switch (type)
    
  } // if event

  bgi_fast_mode = tmp;
  return SDL_FALSE;

} // mouseclick ()

// -----

int mousex (void)
{
  // Returns the X coordinate of the last mouse click.

  return bgi_mouse.x - vp.left;

} // mousex ()

// -----

int mousey (void)
{
  // Returns the Y coordinate of the last mouse click.

  return bgi_mouse.y - vp.top;

} // mousey ()

// -----

void putbuffer (Uint32 *buffer)
{
  // Copies 'buffer' to the screen; 'buffer' must have the same
  // width and height as the current window.
  // Inspired by:
  // https://lodev.org/cgtutor/examples.html#Drawbuffer

  check_initgraph ();

  memcpy ((void *) bgi_activepage[bgi_current_window],
	  buffer,
	  (bgi_maxy + 1) * (bgi_maxx + 1) * sizeof (Uint32));
  bgi_refresh_needed = SDL_TRUE;

} // putbuffer ()

// -----

void putlinebuffer (int y, Uint32 *linebuffer)
{
  // Copies 'linebuffer' to the 'y' coordinate;
  // faster than putpixel ()

  check_initgraph ();

  memcpy ((void *) bgi_activepage[bgi_current_window] +
	  y * (bgi_maxx + 1) * sizeof (Uint32),
	  linebuffer,
	  (bgi_maxx + 1) * sizeof (Uint32));
  bgi_refresh_needed = SDL_TRUE;

} // putlinebuffer ()

// -----

void copysurface (SDL_Surface *surface, int x1, int y1, int x2, int y2)
{
  // Copies a surface to the active page

  Uint32
    *pixels = surface->pixels;

  for (int y = y1; y < y2; y++)
    for (int x = x1; x < x2; x++)
      PIXEL (x, y) = pixels[y * (bgi_maxx + 1) + x] | 0xff000000;

} // copysurface ()

// -----

void readimagefile (char *bitmapname, int x1, int y1, int x2, int y2)
{
  // Reads a .bmp file and displays it immediately at (x1, y1).

  check_initgraph ();

  SDL_Surface
    *bmp_surface,
    *tmp_surface;

  SDL_Rect
    src_rect,
    dest_rect;

  // load bitmap

  bmp_surface = SDL_LoadBMP (bitmapname);

  if (NULL == bmp_surface) {
    SDL_Log ("SDL_LoadBMP() error: %s\n", SDL_GetError ());
    showerrorbox ("SDL_LoadBMP() error");
    return;
  }

  // source rect, position and size
  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.w = bmp_surface->w;
  src_rect.h = bmp_surface->h;

  // destination rect, position
  dest_rect.x = x1 + vp.left;
  dest_rect.y = y1 + vp.top;

  if (0 == x2 || 0 == y2) { // keep original size
    dest_rect.w = src_rect.w;
    dest_rect.h = src_rect.h;
  }
  else { // change size
    dest_rect.w = x2 - x1 + 1;
    dest_rect.h = y2 - y1 + 1;
  }

  // clip it if necessary
  if (x1 + vp.left + src_rect.w > vp.right && vp.clip)
    dest_rect.w = vp.right - x1 - vp.left + 1;
  if (y1 + vp.top + src_rect.h > vp.bottom && vp.clip)
    dest_rect.h = vp.bottom - y1 - vp.top + 1;

  // get SDL surface from current window
  tmp_surface = SDL_GetWindowSurface (bgi_win[bgi_current_window]);

  // blit bitmap surface to current surface
  SDL_BlitScaled (bmp_surface, &src_rect,
                  tmp_surface, &dest_rect);

  // copy pixel data from the new surface to the active page
  copysurface (tmp_surface, dest_rect.x, dest_rect.y,
               dest_rect.x + dest_rect.w, dest_rect.y + dest_rect.h);

  refresh_window ();
  SDL_FreeSurface (bmp_surface);
  SDL_FreeSurface (tmp_surface);

} // readimagefile ()

// -----

void refresh (void)
{
  // Updates the screen.

  refresh_window ();

} // refresh ()

// -----

void resetwinoptions (int id, char *title, int x, int y)
{
  // Resets title and window position.

  check_initgraph ();

  if (SDL_TRUE == bgi_active_windows[id]) {

    if (strlen (title) > BGI_WINTITLE_LEN) {
      char *str = "BGI window title name too long.\n";
      fprintf (stderr, "%s", str);
      showerrorbox (str);
    }
    else
      // if (0 != strlen (title))
      SDL_SetWindowTitle (bgi_win[id], title);

    if (x != -1 && y != -1)
      SDL_SetWindowPosition (bgi_win[id], x, y);

  } // if

} // resetwinoptions ()

// -----

int resizepalette (Uint32 newsize)
{
  // Resizes the ARGB palette to 'newsize'

  void
    *ptr;

  check_initgraph ();

  ptr = realloc (bgi_argb_palette,
		 (BGI_COLORS + TMP_COLORS + PALETTE_SIZE) *
		 sizeof (Uint32));
  if (NULL == ptr) {
    char *str =
      "Could not resize the global palette; leaving it unchanged.\n";
    fprintf (stderr, "%s", str);
    showinfobox (str);
    return 1;
  }

  bgi_argb_palette = ptr;
  PALETTE_SIZE = newsize;
  return 0;

} // resizepalette ()

// -----

// callback for sdlbgiauto ()

static Uint32 updatecallback (Uint32 interval, void *param)
{
  if (bgi_refresh_needed)
    refresh_window ();

  bgi_refresh_needed = SDL_FALSE;

  return interval;

} // updatecallback ()

// -----

static void update (void)
{
  // Conditionally refreshes the screen or schedule it

  if (! bgi_fast_mode)
    refresh_window ();
  else
    bgi_refresh_needed = SDL_TRUE;

} // update ()

// -----

static void update_pixel (int x, int y)
{
  // Updates a single pixel

  if (! bgi_fast_mode)
    updaterect (x, y, x, y);
  else
    bgi_refresh_needed = SDL_TRUE;

} // update_pixel ()

// -----

void sdlbgiauto (void)
{
  // Triggers "auto refresh mode", i.e. refresh() is performed
  // automatically on a separate thread. 
  // Auto mode can't work in Emscripten.

  check_initgraph ();

  Uint32
    interval;

  if (0 == bgi_refresh_rate) {
    // refresh rate not specified by the user;
    // then, let's use the display refresh rate
    SDL_DisplayMode
      display_mode;
    SDL_GetDisplayMode (0, 0, &display_mode);
    // milliseconds between screen refresh
    bgi_refresh_rate = display_mode.refresh_rate;

    // fallback to 30hz if everything else fails
    if (0 == bgi_refresh_rate)
      bgi_refresh_rate = 30;
  }

  interval = (Uint32) 1000.0 / bgi_refresh_rate;

  // install a timer to periodically update the screen
  SDL_AddTimer (interval, updatecallback, NULL);

  bgi_fast_mode = SDL_TRUE;
  bgi_auto_mode = SDL_TRUE;

} // sdlbgiauto ()

// -----

void sdlbgifast (void)
{
  // Triggers "fast mode", i.e. refresh() is needed to
  // display graphics.

  check_initgraph ();

  bgi_fast_mode = SDL_TRUE;

} // sdlbgifast ()

// -----

void sdlbgislow (void)
{
  // Triggers "slow mode", i.e. refresh() is not needed to
  // display graphics.

  check_initgraph ();

  bgi_fast_mode = SDL_FALSE;

} // sdlbgislow ()

// -----

void setallrgbpalette (struct rgbpalettetype *palette)
{
  // Sets the current RGB palette to the values given in palette.

  check_initgraph ();

  if (NULL == palette) {
    bgi_error_code = grError;
    return;
  }

  palette->size = PALETTE_SIZE;

  for (int i = 0; i < PALETTE_SIZE; i++)
    bgi_argb_palette[BGI_COLORS + TMP_COLORS + i] = palette->colors[i];

} // setallrgbpalette ()

// -----

void setalpha (int col, Uint8 alpha)
{
  // Sets alpha transparency for 'col' to 'alpha' (0-255).

  Uint32
    tmp;

  if (col < -1)
    return;

  // COLOR () or COLOR32 () or RGBPALETTE() set the ARGB_FG_COL colour
  if (-1 == col)
    bgi_fg_color = ARGB_FG_COL;
  else {
    bgi_argb_mode = SDL_FALSE;
    bgi_fg_color = col;
  }
  tmp = bgi_argb_palette[bgi_fg_color] << 8; // get rid of alpha
  tmp = tmp >> 8;
  bgi_argb_palette[bgi_fg_color] = ((Uint32)alpha << 24) | tmp;

} // setalpha ()

// -----

void setbkrgbcolor (int colornum)
{
  // Sets the current background color to the 'colornum' colour
  // in the ARGB palette.

  if (colornum < PALETTE_SIZE + 1 && colornum >= 0) {
    bgi_bg_color = BGI_COLORS + TMP_COLORS + colornum;
    bgi_argb_mode = SDL_TRUE;
  }

} // setbkrgbcolor ()

// -----

void setblendmode (int blendmode)
{
  // Sets the blending mode; it can be either
  // SDL_BLENDMODE_NONE or SDL_BLENDMODE_BLEND

  bgi_blendmode = blendmode;

} // setblendmode ()

// -----

void setcurrentwindow (int id)
{
  // Sets the current window.

  check_initgraph ();

  if (SDL_FALSE == bgi_active_windows[id]) {
    char str[40];
    sprintf (str, "Window %d does not exist.\n", id);
    fprintf (stderr, "%s", str);
    showerrorbox (str);
    return;
  }

  bgi_current_window = id;
  bgi_window = bgi_win[bgi_current_window];
  bgi_renderer = bgi_rnd[bgi_current_window];
  bgi_texture = bgi_txt[bgi_current_window];

  // get current window size
  SDL_GetWindowSize (bgi_window, &bgi_maxx, &bgi_maxy);

  bgi_maxx--;
  bgi_maxy--;

} // setcurrentwindow ()

// -----

void setrgbcolor (int colornum)
{
  // Sets the current foreground color to the 'colornum' colour
  // in the ARGB palette.

  if (colornum < PALETTE_SIZE + 1 && colornum >= 0) {
    bgi_fg_color = BGI_COLORS + TMP_COLORS + colornum;
    bgi_argb_mode = SDL_TRUE;
  }

} // setrgbcolor ()

// -----

void setrgbpalette (int colornum, int red, int green, int blue)
{
  // Sets the 'colornum' entry in the ARGB palette specifying its
  // r, g, and b components.

  check_initgraph ();

  if (colornum < PALETTE_SIZE + 1 && colornum >= 0) {
    bgi_argb_palette[BGI_COLORS + TMP_COLORS + colornum] =
    0xff000000 | red << 16 | green << 8 | blue;
    bgi_argb_mode = SDL_TRUE;
  }

} // setrgbpalette ()

// -----

void setwinoptions (char *title, int x, int y, Uint32 flags)
{
  // Sets window options.

  // reset flags for new windows
  bgi_window_flags = 0;

  if (strlen (title) > BGI_WINTITLE_LEN) {
    char *str = "BGI window title name too long.\n";
    fprintf (stderr, "%s", str);
    showerrorbox (str);
  }
  else
    if (0 != strlen (title))
      strcpy (bgi_win_title, title);

  if (x != -1 && y != -1) {
    bgi_window_x = x;
    bgi_window_y = y;
  }

  if (-1 != flags)
    // only a subset of flag is supported for now
    if (flags & SDL_WINDOW_FULLSCREEN         ||
        flags & SDL_WINDOW_FULLSCREEN_DESKTOP ||
        flags & SDL_WINDOW_SHOWN              ||
        flags & SDL_WINDOW_HIDDEN             ||
        flags & SDL_WINDOW_BORDERLESS         ||
	flags & SDL_WINDOW_MAXIMIZED          ||
        flags & SDL_WINDOW_MINIMIZED)
      bgi_window_flags = flags;

} // setwinoptions ()

// -----

void setwintitle (int id, char *title)
{
  // Sets the window title.

  if (strlen (title) > BGI_WINTITLE_LEN) {
    char *str = "BGI window title name too long.\n";
    fprintf (stderr, "%s", str);
    showerrorbox (str);
  }
  else
    SDL_SetWindowTitle (bgi_win[id], title);

} // setwintitle ()

// -----

void showerrorbox (const char *message)
{
  // Opens an error box

  SDL_ShowSimpleMessageBox (SDL_MESSAGEBOX_ERROR,
			    "Error", message, NULL);

} // showerrorbox ()

// -----

void showinfobox (const char *message)
{
  // Opens an info box

  SDL_ShowSimpleMessageBox (SDL_MESSAGEBOX_INFORMATION,
			    "Message", message, NULL);

} // showinfobox ()

// -----

void swapbuffers (void)
{
  // Swaps current visual and active pages.

  check_initgraph ();

  int oldv = getvisualpage ();
  int olda = getactivepage ();
  setvisualpage (olda);
  setactivepage (oldv);

} // swapbuffers ()

// -----

static void updaterect (int x1, int y1, int x2, int y2)
{
  // Updates a rectangle on the screen. This version uses
  // texture streaming.

  int
    x, y,
    pitch = (bgi_maxx + 1) * sizeof (Uint32),
    semipitch;
  void
    *pixels;
  SDL_Rect
    src_rect, dest_rect;

  swap_if_greater (&x1, &x2);
  swap_if_greater (&y1, &y2);

  src_rect.x = x1;
  src_rect.y = y1;
  src_rect.w = x2 - x1 + 1;
  src_rect.h = y2 - y1 + 1;
  dest_rect.x = x1;
  dest_rect.y = y1;
  dest_rect.w = x2 - x1 + 1;
  dest_rect.h = y2 - y1 + 1;

  if (SDL_LockTexture (bgi_txt[bgi_current_window],
		       NULL, &pixels, &pitch) != 0) {
    SDL_Log ("SDL_LockTexture() failed: %s", SDL_GetError ());
    showerrorbox ("SDL_LockTexture() failed");
    exit (1);
  }

  // whole texture:
  /* memcpy (pixels, bgi_visualpage[bgi_current_window], */
  /* 	  pitch * (bgi_maxy + 1)); */

  x = x1;
  semipitch = x * sizeof (Uint32);

  // copy pixel data from bgi_visualpage
  for (y = y1; y < y2 + 1; y++)
    memcpy (pixels + y * pitch + semipitch,
	    (void *) bgi_visualpage[bgi_current_window] +
	    pitch * y + semipitch,
	    (x2 - x1 + 1) * sizeof (Uint32));

  SDL_UnlockTexture (bgi_txt[bgi_current_window]);
  if (0 != SDL_SetTextureBlendMode
      (bgi_txt[bgi_current_window], bgi_blendmode)) {
    SDL_Log ("SDL_SetTextureBlendMode() failed: %s", SDL_GetError ());
    showerrorbox ("SDL_SetTextureBlendMode() failed");
  }

  if (0 != SDL_RenderCopy (bgi_rnd[bgi_current_window],
			   bgi_txt[bgi_current_window],
			   &src_rect, &dest_rect)) {
    SDL_Log ("SDL_RenderCopy() failed: %s", SDL_GetError ());
    showerrorbox ("SDL_RenderCopy() failed");
  }
  SDL_RenderPresent (bgi_rnd[bgi_current_window]);

} // updaterect ()

// -----

void writeimagefile (char *filename,
                     int left, int top, int right, int bottom)
{
  // Writes a .bmp file from the screen rectangle defined by
  // 'left', 'top', 'right', 'bottom'.

  check_initgraph ();

  SDL_Surface
    *dest;
  SDL_Rect
    rect;

  rect.x = left;
  rect.y = top;
  rect.w = right - left + 1;
  rect.h = bottom - top + 1;

  // the user specified a range larger than the viewport
  if (rect.w > (vp.right - vp.left + 1))
    rect.w = vp.right - vp.left + 1;
  if (rect.h > (vp.bottom - vp.top + 1))
    rect.h = vp.bottom - vp.top + 1;

  dest = SDL_CreateRGBSurface (0, rect.w, rect.h, 32, 0, 0, 0, 0);
  if (NULL == dest) {
    SDL_Log ("SDL_CreateRGBSurface() failed: %s", SDL_GetError ());
    showerrorbox ("SDL_CreateRGBSurface() failed");
    return;
  }

  SDL_RenderReadPixels (bgi_rnd[bgi_current_window],
                        NULL,
                        SDL_GetWindowPixelFormat
			(bgi_win[bgi_current_window]),
                        bgi_vpage[bgi_vp]->pixels,
                        bgi_vpage[bgi_vp]->pitch);
  // blit and save
  SDL_BlitSurface (bgi_vpage[bgi_vp], &rect, dest, NULL);
  SDL_SaveBMP (dest, filename);

  // free the stuff
  SDL_FreeSurface (dest);

} // writeimagefile ()

// -----

int xkb_hit (void)
{
  // Returns 1 when a key is pressed, or QUIT
  // if the user asked to close the window

  SDL_Event
    event;
  SDL_Keycode
    key;

  update ();

  if (SDL_TRUE == bgi_xkey_pressed) { // a key was pressed during delay ()
    bgi_xkey_pressed = SDL_FALSE;
    return SDL_TRUE;
  }

  if (SDL_PollEvent (&event)) {

    if (SDL_KEYDOWN == event.type) {
      key = event.key.keysym.sym;
      bgi_last_event = event.type;
      bgi_xkey_pressed = SDL_TRUE;
      bgi_last_key_pressed = (int) key;
      return SDL_TRUE;
    } // if (SDL_KEYDOWN == event.type)
    else
      if (SDL_WINDOWEVENT == event.type) {
        if (SDL_WINDOWEVENT_CLOSE == event.window.event)
          return QUIT;
      }
    else
      SDL_PushEvent (&event); // don't disrupt the mouse

  } // if (SDL_PollEvent (&event))

  return SDL_FALSE;

} // xkb_hit ()

// -----

// --- End of file SDL_bgi.c
