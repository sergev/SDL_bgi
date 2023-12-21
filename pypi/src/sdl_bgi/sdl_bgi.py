# SDL_bgi.py        -*- Python -*-

"""
SDL_bgi is a graphics library (GRAPHICS.H) for C, C++, WebAssembly,
and Python. It's based on SDL2 and it's portable on many platforms.

By Guido Gonzato, PhD
Automatic refresh patch, CHR font support:
Marco Diego Aurélio Mesquita

Latest update: December 12, 2022

ZLib License

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
"""

from ctypes       import *
from sysconfig    import get_platform
from random       import randint

# try to load the SDL_bgi library

if get_platform () == 'win-amd64':       # Windows, IDLE
    sdlbgilib = ".\SDL_bgi.dll"
elif get_platform () == 'mingw_x86_64':  # Windows, MSYS2/Mingw64
    sdlbgilib = '/msys64/mingw64/bin/SDL_bgi.dll'
else: # GNU/Linux, macOS
    sdlbgilib = 'libSDL_bgi.so'

try:
    sb = CDLL (sdlbgilib)
except:
    print ("The sdl_bgi.py module needs the SDL_bgi library binary;")
    print ("please get it from https://sdl-bgi.sourceforge.io")
    print ("Exiting.")
    quit ()

SDL_BGI_VERSION = "3.0.0"
NOPE = False
YEAH = True
BGI_WINTITLE_LEN = 512

# number of concurrent windows that can be created
NUM_BGI_WIN = 16

VPAGES = 4

# BGI fonts

DEFAULT_FONT     = 0   # 8x8 bitmap
TRIPLEX_FONT     = 1   # trip.h
SMALL_FONT       = 2   # litt.h
SANS_SERIF_FONT  = 3   # sans.h
GOTHIC_FONT      = 4   # goth.h
SCRIPT_FONT      = 5   # scri.h
SIMPLEX_FONT     = 6   # simp.h
TRIPLEX_SCR_FONT = 7   # tscr.h
COMPLEX_FONT     = 8   # lcom.h
EUROPEAN_FONT    = 9   # euro.h
BOLD_FONT        = 10  # bold.h
LAST_SPEC_FONT   = 11

HORIZ_DIR = 0
VERT_DIR  = 1

USER_CHAR_SIZE = 0

LEFT_TEXT   = 0
CENTER_TEXT = 1
RIGHT_TEXT  = 2
BOTTOM_TEXT = 0
TOP_TEXT    = 2

# BGI colours

BLACK        = 0
BLUE         = 1
GREEN        = 2
CYAN         = 3
RED          = 4
MAGENTA      = 5
BROWN        = 6 
LIGHTGRAY    = 7  
DARKGRAY     = 8
LIGHTBLUE    = 9  
LIGHTGREEN   = 10
LIGHTCYAN    = 11
LIGHTRED     = 12
LIGHTMAGENTA = 13 
YELLOW       = 14
WHITE        = 15
MAXCOLORS    = 15

# ARGB colours set by COLOR () COLOR32 () and RGBPALETTE ()

ARGB_FG_COL   = 16
ARGB_BG_COL   = 17
ARGB_FILL_COL = 18
ARGB_TMP_COL  = 19
TMP_COLORS    = 4

# line style thickness and drawing mode

NORM_WIDTH  = 1
THICK_WIDTH = 3

SOLID_LINE   = 0
DOTTED_LINE  = 1
CENTER_LINE  = 2
DASHED_LINE  = 3
USERBIT_LINE = 4

COPY_PUT = 0
XOR_PUT  = 1
OR_PUT   = 2
AND_PUT  = 3
NOT_PUT  = 4

# fill styles

EMPTY_FILL      = 0   # fills area in background color
SOLID_FILL      = 1   # fills area in solid fill color
LINE_FILL       = 2   # --- fill
LTSLASH_FILL    = 3   # /// fill
SLASH_FILL      = 4   # /// fill with thick lines
BKSLASH_FILL    = 5   # \\\ fill with thick lines
LTBKSLASH_FILL  = 6   # \\\ fill
HATCH_FILL      = 7   # light hatch fill
XHATCH_FILL     = 8   # heavy cross hatch fill
INTERLEAVE_FILL = 9   # interleaving line fill
WIDE_DOT_FILL   = 10  # Widely spaced dot fill
CLOSE_DOT_FILL  = 11  # Closely spaced dot fill
USER_FILL       = 12  # user defined fill

# window properties
SDL_WINDOW_FULLSCREEN   = 0x00000001  # from SDL_video.h
SDL_BLENDMODE_NONE      = 0x00000000
SDL_BLENDMODE_BLEND     = 0x00000001
SDL_WINDOWPOS_UNDEFINED = 0x1FFF0000
SDL_WINDOWPOS_CENTERED  = 0x2FFF0000
SDL_WINDOW_SHOWN        = 0x00000004
SDL_WINDOW_HIDDEN       = 0x00000008
SDL_WINDOW_BORDERLESS   = 0x00000010
SDL_WINDOW_MINIMIZED    = 0x00000040
SDL_WINDOW_FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN | 0x00001000

# mouse events - compatible with WinBGIm

SDL_BUTTON_LEFT      = 1
SDL_BUTTON_MIDDLE    = 2
SDL_BUTTON_RIGHT     = 3
SDL_MOUSEMOTION      = 0x400  # from SDL_events.h
SDL_MOUSEBUTTONDOWN  = 0x401
SDL_MOUSEBUTTONUP    = 0x402
SDL_MOUSEWHEEL       = 0x403

WM_MOUSEMOVE       = SDL_MOUSEMOTION
WM_LBUTTONDOWN     = SDL_BUTTON_LEFT # from SDL_mouse.h
WM_LBUTTONUP       = SDL_MOUSEBUTTONUP   + SDL_BUTTON_LEFT
WM_LBUTTONDBLCLK   = SDL_MOUSEBUTTONDOWN + SDL_BUTTON_LEFT + 2

WM_MBUTTONDOWN     = SDL_BUTTON_MIDDLE
WM_MBUTTONUP       = SDL_MOUSEBUTTONUP   + 10*SDL_BUTTON_MIDDLE
WM_MBUTTONDBLCLK   = SDL_MOUSEBUTTONDOWN + 10*SDL_BUTTON_MIDDLE + 2

WM_RBUTTONDOWN     = SDL_BUTTON_RIGHT
WM_RBUTTONUP       = SDL_MOUSEBUTTONUP   + 20*SDL_BUTTON_RIGHT
WM_RBUTTONDBLCLK   = SDL_MOUSEBUTTONDOWN + 20*SDL_BUTTON_RIGHT + 2

WM_WHEEL           = SDL_MOUSEWHEEL
WM_WHEELUP         = SDL_BUTTON_RIGHT + 1
WM_WHEELDOWN       = SDL_BUTTON_RIGHT + 2

# keys

SDL_SCANCODE_TO_KEYCODE = lambda key: key | (1 << 30)
# 30 = SDLK_SCANCODE_MASK

KEY_HOME        = SDL_SCANCODE_TO_KEYCODE (74) # from SDL_scancode.h
KEY_LEFT        = SDL_SCANCODE_TO_KEYCODE (80)
KEY_UP          = SDL_SCANCODE_TO_KEYCODE (82)
KEY_RIGHT       = SDL_SCANCODE_TO_KEYCODE (79)
KEY_DOWN        = SDL_SCANCODE_TO_KEYCODE (81)
KEY_PGUP        = SDL_SCANCODE_TO_KEYCODE (75)
KEY_PGDN        = SDL_SCANCODE_TO_KEYCODE (78)
KEY_END         = SDL_SCANCODE_TO_KEYCODE (77)
KEY_INSERT      = SDL_SCANCODE_TO_KEYCODE (73)
KEY_DELETE      = 0o177
KEY_F1          = SDL_SCANCODE_TO_KEYCODE (58)
KEY_F2          = SDL_SCANCODE_TO_KEYCODE (59)
KEY_F3          = SDL_SCANCODE_TO_KEYCODE (60)
KEY_F4          = SDL_SCANCODE_TO_KEYCODE (61)
KEY_F5          = SDL_SCANCODE_TO_KEYCODE (62)
KEY_F6          = SDL_SCANCODE_TO_KEYCODE (63)
KEY_F7          = SDL_SCANCODE_TO_KEYCODE (64)
KEY_F8          = SDL_SCANCODE_TO_KEYCODE (65)
KEY_F9          = SDL_SCANCODE_TO_KEYCODE (66)
KEY_F10         = SDL_SCANCODE_TO_KEYCODE (67)
KEY_F11         = SDL_SCANCODE_TO_KEYCODE (68)
KEY_F12         = SDL_SCANCODE_TO_KEYCODE (69)
KEY_CAPSLOCK    = SDL_SCANCODE_TO_KEYCODE (57)
KEY_LEFT_CTRL   = SDL_SCANCODE_TO_KEYCODE (224)
KEY_RIGHT_CTRL  = SDL_SCANCODE_TO_KEYCODE (228)
KEY_LEFT_SHIFT  = SDL_SCANCODE_TO_KEYCODE (225)
KEY_RIGHT_SHIFT = SDL_SCANCODE_TO_KEYCODE (229)
KEY_LEFT_ALT    = SDL_SCANCODE_TO_KEYCODE (226)
KEY_RIGHT_ALT   = SDL_SCANCODE_TO_KEYCODE (230)
KEY_ALT_GR      = SDL_SCANCODE_TO_KEYCODE (230)
KEY_LGUI        = SDL_SCANCODE_TO_KEYCODE (227)
KEY_RGUI        = SDL_SCANCODE_TO_KEYCODE (231)
KEY_MENU        = SDL_SCANCODE_TO_KEYCODE (118)
KEY_TAB         = '\t'
KEY_BS          = '\b'
KEY_RET         = '\r'
KEY_PAUSE       = SDL_SCANCODE_TO_KEYCODE (72)
KEY_SCR_LOCK    = SDL_SCANCODE_TO_KEYCODE (71)
KEY_ESC         = 0o33

SDL_KEYDOWN     = 0x300
SDL_QUIT        = 0x100 # from SDL_events.h
QUIT            = SDL_QUIT

# graphics modes. Expanded from the original GRAPHICS.H

DETECT         = -1
SDL            = 0
SDL_320x200    = 1
CGA            = 1 
SDL_640x200    = 2
SDL_640x350    = 3
SDL_EGA        = 3
EGA            = 3
SDL_640x480    = 4
SDL_VGA        = 4
VGA            = 4
SDL_720x348    = 5
SDL_720x350    = 6
SDL_800x600    = 7
SVGA           = 7
SDL_1024x768   = 8
SDL_1152x900   = 9
SDL_1280x1024  = 10
SDL_1366x768   = 11
SDL_USER       = 12
SDL_FULLSCREEN = 13

# error messages

grOk               =   0
grNoInitGraph      =  -1
grNotDetected      =  -2
grFileNotFound     =  -3
grInvalidDriver    =  -4
grNoLoadMem        =  -5
grNoScanMem        =  -6
grNoFloodMem       =  -7
grFontNotFound     =  -8
grNoFontMem        =  -9
grInvalidMode      = -10
grError            = -11
grIOerror          = -12
grInvalidFont      = -13
grInvalidFontNum   = -14
grInvalidVersion   = -18

# C structs, implemented as classes

class arccoordstype (Structure):
    _fields_ = [ ("x", c_int),
                 ("y", c_int),
                 ("xstart", c_int),
                 ("ystart", c_int),
                 ("xend", c_int),
                 ("yend", c_int) ]

class date (Structure):
    _fields_ = [ ("da_year", c_int),
                 ("da_day", c_int),
                 ("da_mon", c_int) ]

class fillsettingstype (Structure):
    _fields_ = [ ("pattern", c_int),
                 ("color", c_int) ]

class linesettingstype (Structure):
    _fields_ = [ ("linestyle", c_int),
                 ("upattern", c_uint),
                 ("thickness", c_uint) ]

class palettetype (Structure):
    _fields_ = [ ("size", c_ubyte),
                 ("colors", c_uint32 * (MAXCOLORS + 1)) ]

# SDL_bgi extension

class rgbpalettetype (Structure):
    _fields_ = [ ("size", c_uint),
                 ("colors", c_char_p) ]

class textsettingstype (Structure):
    _fields_ = [ ("font", c_int),
                 ("direction", c_int),
                 ("charsize", c_int),
                 ("horiz", c_int),
                 ("vert", c_int) ]

class viewporttype (Structure):
    _fields_ = [ ("left", c_int),
                 ("top", c_int),
                 ("right", c_int),
                 ("bottom", c_int),
                 ("clip", c_int) ]

# ----- utility

def list2vec (list):
    """
    Turns a list to a vector.
    """
    return (c_int * len (list)) (*list)

# -----

def sizeofint () -> int:
    """
    Returns C's sizeof (int)
    """
    return sb.sizeofint ()

# -----

def vec2buf (vector):
    """
    Returns a string buffer that contains the values stored
    in 'vector'. A vector can be obtained from 'matrix' with
    reshape (matrix, -1). Numpy required.
    """
    SOI = sizeofint ()
    buf = create_string_buffer (vector.size * SOI)
    for element in range (vector.size):
        value = int (vector[element])
        i = element * SOI
        # store 'value' in consecutive bytes
        for j in range (SOI):
            buf[i + j] = value.to_bytes (SOI, 'little')[j]
    return buf

# -----

### ----- standard BGI functions

def arc (x, y, stangle, endangle, radius):
    """
    Draws a circular arc centered at (x, y), with a radius given
    by 'radius', traveling counterclockwise from 'stangle' to
    'endangle'.
    """
    sb.arc (int (x), int (y), int (stangle), int (endangle),
            int (radius))

# -----

def bar3d (left, top, right, bottom, depth, topflag):
    """
    Draws a filled-in rectangle (bar), using the current fill colour
    and fill pattern.
    """
    sb.bar3d (int (left), int (top), int (right), int (bottom),
              int (depth), int (topflag))
    
# -----

def bar (left, top, right, bottom):
    """
    Draws a three-dimensional, filled-in rectangle (bar), using
    the current fill colour and fill pattern.
    """
    sb.bar (int (left), int (top), int (right), int (bottom))
    
# -----

def circle (x, y, radius):
    """
    Draws a circle of the given radius at (x, y).
    """
    sb.circle (int (x), int (y), int (radius))

# -----

def cleardevice ():
    """
    Clears the graphics screen, filling it with the current
    background colour.
    """
    sb.cleardevice ()

# -----

def clearviewport ():
    """
    Clears the viewport, filling it with the current background
    colour.
    """
    sb.clearviewport ()

# -----

def closegraph ():
    """
    Closes the graphics system.
    """
    sb.closegraph ()

# -----

def delay (msec):
    """
    Waits for millisec milliseconds.
    """
    sb.delay (int (msec))

# -----

def detectgraph (graphdriver = None, graphmode = None):
    """
    Detects the default graphics driver and graphics mode to use;
    SDL and SDL_FULLSCREEN, respectively.
    """
    sb.detectgraph.argtypes = [POINTER (c_int), POINTER (c_int)]
    if graphdriver != None and graphmode != None:
        sb.detectgraph (int (graphdriver), int (graphmode))
    else:
        gd, gm = c_int (), c_int ()
        sb.detectgraph (byref (gd), byref (gm))
        return gd.value, gm.value

# -----

def drawpoly (numpoints, polypoints: list):
    """
    Draws a polygon of 'numpoints' vertices; 'polypoints'
    is a list of coordinates.
    """
    sb.drawpoly.argtypes = [c_int, POINTER (c_int)]
    sb.drawpoly (int (numpoints), list2vec (polypoints))

# -----

def ellipse (x, y, stangle, endangle, xradius, yradius):
    """
    Draws an elliptical arc centered at (x, y), with axes given
    by 'xradius' and 'yradius', traveling from 'stangle' to
    'endangle'.
    """
    sb.ellipse (int (x), int (y), int (stangle), int (endangle),
                int (xradius), int (yradius))
    
# -----

def fillellipse (x, y, xradius, yradius):
    """
    Draws an elliptical arc centered at (x, y), with axes given
    by 'xradius' and 'yradius', and fills it using the current fill
    colour and fill pattern.
    """
    sb.fillellipse (int (x), int (y), int (xradius), int (yradius))
    
# -----

def fillpoly (numpoints: int, polypoints: list):
    """
    Draws a polygon of numpoints vertices and fills it using
    the current fill colour.
    """
    sb.fillpoly.argtypes = [c_int, POINTER (c_int)]
    sb.fillpoly (int (numpoints), list2vec (polypoints))

# -----

def floodfill (x, y, border):
    """
    Fills an enclosed area, starting from (x, y) bounded by the
    'border' colour. The area is filled using the current fill colour.
    """
    sb.floodfill (int (x), int (y), int (border))

# -----

def getactivepage () -> int:
    """
    Returns the active page number.
    """
    return sb.getactivepage ()

# -----

def getarccoords (arccoords: arccoordstype = None):
    """
    Gets the coordinates of the last call to arc(), filling the
    'arccoords' structure.
    """
    sb.getarccoords.argtypes = [POINTER (arccoordstype)]
    if arccoords != None:
        sb.getarccoords (arccoords)
    else:
        tmp = arccoordstype ()
        sb.getarccoords (byref (tmp))
        return (tmp)

# -----

def getaspectratio (xasp = None, yasp = None):
    """
    Retrieves the current graphics mode's aspect ratio. 'xasp'
    and 'yasp' are always 10000 (i.e. pixels are square).
    """
    sb.getaspectratio.argtypes = [POINTER (c_int), POINTER (c_int)]
    if xasp != None and yasp != None:
        sb.getaspectratio (int (xasp), int (yasp))
    else:
        tmp1, tmp2 = c_int(), c_int ()
        sb.getaspectratio (byref (tmp1), byref (tmp2))
        return tmp1.value, tmp2.value

# -----

def getbkcolor () -> int:
    """
    Returns the current background colour.
    """
    return sb.getbkcolor ()

# -----

def getch () -> int:
    """
    Waits for a key and returns its ASCII or key code.
    """
    return sb.bgi_getch ()

# -----

def getcolor () -> int:
    """
    Returns the current drawing (foreground) colour.
    """
    return sb.getcolor ()

# -----

def getdefaultpalette () -> palettetype:
    """
    Returns the default palette definition structure.
    """
    sb.getdefaultpalette.restype = POINTER (palettetype)
    tmp = palettetype ()
    tmp = sb.getdefaultpalette ()
    return tmp.contents

# -----

def getdrivername () -> str:
    """
    Returns a string containing the name of the current
    graphics driver.
    """
    str = sb.getdrivername
    str.restype = c_char_p
    return sb.getdrivername ().decode ('ascii')

# -----

def getfillpattern (pattern: str):
    """
    Copies the user-defined fill pattern, as set by setfillpattern(),
    into the 8-byte area pointed to by 'pattern'.
    """
    sb.getfillpattern.argtypes = [c_char_p]
    sb.getfillpattern (pattern)

# -----

def getfillsettings (fillinfo: fillsettingstype = None):
    """
    Fills the fillsettingstype structure pointed to by 'fillinfo'
    with information about the current fill pattern and fill colour.
    """
    sb.getfillsettings.argtypes = [POINTER (fillsettingstype)]
    if fillinfo != None:
        sb.getfillsettings (fillinfo)
    else:
        tmp = fillsettingstype ()
        sb.getfillsettings (byref (tmp))
        return tmp

# -----

def getgraphmode () -> int:
    """
    Returns the current graphics mode.
    """
    return sb.getgraphmode ()

# -----

def getimage (left, top, right, bottom, bitmap):
    """
    Copies a bit image of the specified region into the memory
    pointed to by 'bitmap'.
    """
    sb.getimage.argtypes = [c_int, c_int, c_int, c_int, c_char_p ]
    sb.getimage (int (left), int (top), int (right), int (bottom),
                 bitmap)

# -----

def getlinesettings (lineinfo: linesettingstype = None):
    """
    Fills the linesettingstype structure pointed by 'lineinfo' with
    information about the current line style, pattern, and thickness.
    """
    sb.getlinesettings.argtypes = [POINTER (linesettingstype)]
    if lineinfo != None:
        sb.getlinesettings (lineinfo)
    else:
        tmp = linesettingstype ()
        sb.getlinesettings (tmp)
        return tmp

# -----

def getmaxcolor () -> int:
    """
    Returns the maximum colour value available (MAXCOLORS).
    If ARGB colours are being used, it returns PALETTE_SIZE
    (default is 4096).
    """
    return sb.getmaxcolor ()

# -----

def getmaxmode () -> int:
    """
    Returns the maximum mode number for the current driver.
    In SDL_bgi, the default is SDL_FULLSCREEN.
    """
    return sb.getmaxmode ()

# -----

def getmaxx () -> int:
    """
    Returns the maximum x screen coordinate.
    """
    return sb.getmaxx ()

# -----

def getmaxy () -> int:
    """
    Returns the maximum y screen coordinate.
    """
    return sb.getmaxy ()

# -----

def getmodename (mode_number) -> str:
    """
    Returns a string containing the name of the specified
    graphics mode.
    """
    str = sb.getmodename
    str.restype = c_char_p
    return sb.getmodename (int (mode_number)).decode ('ascii')

# -----

def getmoderange (graphdriver = None, lomode = None, himode = None):
    """
    Returns the range of valid graphics modes. The 'graphdriver'
    parameter is ignored.
    """
    sb.getmoderange.argtypes = [c_int, POINTER (c_int), POINTER (c_int)]
    if graphdriver != None and lomode != None and himode != None:
        sb.getmoderange (int (driver), int (lomode), int (himode))
    else:
        tmp1, tmp2 = c_int(), c_int ()
        sb.getmoderange (0, byref (tmp1), byref (tmp2))
        return tmp1.value, tmp2.value

# -----

def getpalette ():
    """
    Fills the palettetype structure pointed to by palette with
    information about the current palette size and colours.
    """
    sb.getpalette.argtypes = [POINTER (palettetype)]
    tmp = palettetype ()
    sb.getpalette (byref (tmp))
    return tmp

# -----

def getpalettesize () -> int:
    """
    Returns the size of the palette (MAXCOLORS + 1 or
    MAXRGBCOLORS + 1).
    """
    return sb.getpalettesize ()

# -----

def getpixel (x, y) -> int:
    """
    Returns the colour of the pixel located at (x, y).
    """
    return sb.getpixel (int (x), int (y))

# -----

def gettextsettings (texttypeinfo: textsettingstype):
    """
    Fills the textsettingstype structure pointed to by
    'texttypeinfo' with information about the current text font,
    direction, size, and justification.
    """
    sb.gettextsettings.argtypes = [POINTER (textsettingstype)]
    sb.gettextsettings (texttypeinfo)

# -----

def getviewsettings (viewport: viewporttype):
    """
    Fills the viewporttype structure pointed to by 'viewport'
    with information about the current viewport.
    """
    sb.getviewsettings.argtypes = [POINTER (viewporttype)]
    sb.getviewsettings (viewport)

# -----

def getvisualpage () -> int:
    """
    Returns the visual page number.
    """
    return sb.getvisualpage ()

# -----

def getx () -> int:
    """
    Returns the current CP x coordinate, relative to the viewport.
    """
    return sb.getx ()

# -----

def gety () -> int:
    """
    Returns the current CP y coordinate, relative to the viewport.
    """
    return sb.gety ()

# -----

def graphdefaults ():
    """
    Resets all graphics settings to their default values.
    """
    sb.graphdefaults ()

# -----

def grapherrormsg (errorcode) -> str:
    """
    Returns the error message string associated with 'errorcode',
    returned by graphresult().
    """
    sb.grapherrormsg.restype = c_char_p
    return sb.grapherrormsg (errorcode).decode ('ascii')

# -----

def graphresult () -> int:
    """
    Returns the error code for the last unsuccessful graphics
    operation and resets the error level to grOk.
    """
    return sb.graphresult ()

# -----

def imagesize (left, top, right, bottom) -> int:
    """
    Returns the size in bytes of the memory area required to store
    a bit image.
    """
    return sb.imagesize (int (left), int (top),
                         int (right), int (bottom))

# -----

def initgraph (graphdriver = None, graphmode = None, pathtodriver: str = None):
    """
    Initializes the graphics system. Use initgraph ()
    for default settings.
    """
    if graphdriver != None and graphmode != None:
        sb.initgraph (byref (c_int (graphdriver)),
                      byref (c_int (graphmode)), "")
    else:
        tmp1, tmp2 = c_int(), c_int ()
        sb.initgraph (tmp1, tmp2, "")

# -----

def installuserdriver (foo):
    """
    Unimplemented; not needed in SDL_bgi.
    """
    return sb.installuserdriver (foo)

# -----

def installuserfont (name) -> int: 
    """
    Loads and installs a .CHR font from disk. The function returns
    an integer to be used as first argument in settextstyle().
    """
    sb.installuserfont.argtypes = [c_char_p]
    return sb.installuserfont (name.encode ('ascii'))

# -----

def kbhit () -> bool:
    """
    Returns True when a key is pressed, excluding special keys
    (Ctrl, Shift, etc.)
    """
    return sb.k_bhit ()

# -----

def lastkey () -> int:
    """
    Returns the last key that was detected by kbhit().
    """
    return sb.lastkey ()

# -----

def line (x1, y1, x2, y2):
    """
    Draws a line between two specified points.
    """
    sb.line (int (x1), int (y1), int (x2), int (y2))

# -----

def linerel (dx, dy):
    """
    Draws a line from the CP to a point that is (dx, dy) pixels
    from the CP. The CP is then advanced by (dx, dy).
    """
    sb.linerel (int (dx), int (dy))

# -----

def lineto (x, y):
    """
    Draws a line from the CP to (x, y), then moves the CP to
    (dx, dy).
    """
    sb.lineto (int (x), int (y))

# -----

def moverel (dx, dy):
    """
    Moves the CP by (dx, dy) pixels.
    """
    sb.moverel (int (dx), int (dy))

# -----

def moveto (x, y):
    """
    Moves the CP to the position (x, y), relative to the viewport.
    """
    sb.moveto (int (x), int (y))

# -----

def outtext (textstring):
    """
    Outputs 'textstring' at the CP.
    """
    sb.outtextxy.argtypes = [c_char_p]
    sb.outtext (textstring.encode ('ascii'));

# -----

def outtextxy (x, y, textstring):
    """
    Outputs 'textstring' at (x, y).
    """
    sb.outtextxy.argtypes = [c_int, c_int, c_char_p]
    sb.outtextxy (int(x), int(y), textstring.encode ('ascii'));

# -----

def pieslice (x, y, stangle, endangle, radius):
    """
    Draws and fills a pie slice centered at (x, y), with
    a radius given by 'radius', traveling from 'stangle'
    to 'endangle'.
    """
    sb.pieslice (int (x), int (y), int (stangle), int (endangle),
                 int (radius))

# -----

def putimage (left, top, bitmap, op):
    """
    Puts the bit image pointed to by 'bitmap' onto the screen, with
    the upper left corner of the image placed at (left, top).
    'op' specifies the drawing mode (COPY_PUT, etc).
    """
    sb.putimage.argtypes = [c_int, c_int, c_char_p, c_int]
    sb.putimage (int (left), int (top), bitmap, int (op))

# -----

def putpixel (x, y, color):
    """
    Plots a pixel at (x, y) using 'color'.
    """
    sb.putpixel (int (x), int (y), int (color))

# -----

def random (range) -> int:
    """
    Returns a random number between 0 and 'range' - 1.
    """
    return randint (0, range - 1)

# -----

def rectangle (left, top, right, bottom):
    """
    Draws a rectangle delimited by (left, top) and (right, bottom).
    """
    sb.rectangle (int (left), int (top), int (right), int (bottom))

# -----

def registerbgidriver (foo):
    """
    Unimplemented; not needed in SDL_bgi.
    """
    return sb.registerbgidriver (foo)

# -----

def registerbgifont (foo):
    """
    Unimplemented; not needed in SDL_bgi.
    """
    return sb.registerbgifont (foo)

# -----

def restorecrtmode ():
    """
    Hides the graphics window.
    """
    sb.restorecrtmode ()

# -----

def sector (x, y, stangle, endangle, xradius, yradius):
    """
    Draws and fills an elliptical pie slice centered at (x, y),
    horizontal and vertical radii given by 'xradius' and 'yradius',
    traveling from 'stangle' to 'endangle'.
    """
    sb.sector (int (x), int (y), int (stangle), int (endangle),
               int (xradius), int (yradius))

# -----

def setactivepage (page):
    """
    Makes 'page' the active page for all subsequent graphics output.
    """
    sb.setactivepage (int (page))

# -----

def setallpalette (palette: palettetype):
    """
    Sets the current palette to the values given in 'palette'.
    """
    sb.setallpalette.argtypes = [POINTER (palettetype)]
    sb.setallpalette (palette)

# -----

def setaspectratio (xasp, yasp):
    """
    Changes the default aspect ratio of the graphics.
    """
    sb.setaspectratio (int (xasp), int (yasp))

# -----

def setbkcolor (color):
    """
    Sets the current background colour.
    """
    sb.setbkcolor (int (color))

# -----

def setcolor (color):
    """
    Sets the current drawing colour.
    """
    sb.setcolor (int (color))

# -----

def setfillpattern (pattern: str, color):
    """
    Sets a user-defined fill pattern.
    """
    sb.setfillpattern.argtypes = [c_char_p, c_int]
    sb.setfillpattern (pattern, int (color))

# -----

def setfillstyle (pattern, color):
    """
    Sets the fill pattern and fill colour.
    """
    sb.setfillstyle (pattern, int (color))

# -----

def setgraphbufsize (bufsize):
    """
    Unimplemented; not needed in SDL_bgi.
    """
    sb.setgraphbufsize (int (bufsize))

# -----

def setgraphmode (mode):
    """
    Shows the window that was hidden by restorecrtmode().
    """
    sb.setgraphmode (int (mode))

# -----

def setlinestyle (linestyle, upattern, thickness):
    """
    Sets the line width and style.
    """
    sb.setlinestyle (int (linestyle), upattern, int (thickness))

# -----

def setpalette (colornum, color):
    """
    Changes the standard palette 'colornum' to 'color', which can
    also be specified using the COLOR() function.
    """
    sb.setpalette (int (colornum), int (color))

# -----

def settextjustify (horiz, vert):
    """
    Sets text justification. Text output will be justified
    around the CP horizontally and vertically.
    """
    sb.settextjustify (int (horiz), int (vert))

# -----

def settextstyle (font, direction, charsize):
    """
    Sets the text style.
    """
    sb.settextstyle (int (font), int (direction), int (charsize))

# -----

def setusercharsize (multx, divx, multy, divy):
    """
    Lets the user change the character width and height.
    """
    sb.setusercharsize (int (multx), int (divx), int (multy), int (divy))

# -----

def setviewport (left, top, right, bottom, clip):
    """
    Sets the current viewport for graphics output.
    """
    sb.setviewport (int (left), int (top), int (right), int (bottom),
                    int (clip))

# -----

def setvisualpage (page):
    """
    Sets the visual graphics page number to 'page'.
    """
    sb.setvisualpage (int (page))

# -----

def setwritemode (mode):
    """
    Sets the writing mode for line drawing (COPY_PUT, etc.)
    """
    sb.setwritemode (int (mode))

# -----

def textheight (textstring) -> int:
    """
    Returns the height in pixels of 'textstring'.
    """
    return sb.textheight (textstring.encode ('ascii'))

# -----

def textwidth (textstring) -> int:
    """
    Returns the width in pixels of 'textstring'.
    """
    return sb.textwidth (textstring.encode ('ascii'))

### ----- SDL_bgi extensions

# -----

def ALPHA_VALUE (color) -> int:
    """
    Returns the alpha (transparency) component of 'color' in
    the ARGB palette.
    """
    return sb.ALPHA_VALUE (int (color))

# -----

def BLUE_VALUE (color) -> int:
    """
    Returns the blue component of 'color' in the ARGB palette.
    """
    return sb.BLUE_VALUE (int (color))

# -----

def COLOR (r, g, b) -> int:
    """
    Can be used as an argument for putpixel(), setalpha(),
    setcolor(), setbkcolor(), setfillpattern(), and setfillstyle()
    to set a colour specifying its ARGB components.
    """
    return sb.COLOR (int (r), int (g), int (b))

# -----

def COLOR32 (color: int) -> int:
    """
    Can be used as an argument for putpixel(), setalpha(),
    setcolor(), setbkcolor(), setfillpattern(), and setfillstyle()
    to set a colour as ARGB integer.
    """
    return sb.COLOR32 (int (color))

# -----

def GREEN_VALUE (color) -> int:
    """
    Returns the green component of 'color' in the ARGB palette.
    """
    return sb.GREEN_VALUE (int (color))
    
# -----

def IS_BGI_COLOR (color) -> bool:
    """
    Returns True if the current drawing colour is a standard
    BGI colour (that is, not ARGB). The 'color' argument is
    actually redundant (WinBGIm compatibility).
    """
    return sb.IS_BGI_COLOR (int (color))

# -----

def IS_RGB_COLOR (color) -> bool:
    """
    Returns True if the current drawing colour is ARGB.
    The 'color' argument is actually redundant (WinBGIm
    compatibility).
    """
    return sb.IS_RGB_COLOR (int (color))

# -----

def RED_VALUE (color) -> int:
    """
    Returns the red component of 'color' in the ARGB palette.
    """
    return sb.RED_VALUE (int (color))

# -----

def RGBPALETTE (color):
    """
    Can  be used as an argument for putpixel(), setalpha(),
    setcolor(), setbkcolor(), setfillpattern(), and setfillstyle()
    to set a colour from the ARGB palette color entry.
    """
    return sb.RGBPALETTE (int (color))

# -----

def closewindow (id):
    """
    Closes the window identified by 'id'.
    """
    sb.closewindow (int (id))

# -----

def colorname (color) -> str:
    """
    Returns a string containing the colour name.
    """
    str = sb.colorname
    str.restype = c_char_p
    return sb.colorname (int (color)).decode ('ascii')
    
# -----

def colorRGB(r, g, b) -> int:
    """
    Can be used to compose a 32 bit colour with r g b components.
    """
    return 0xff000000 | int (r) << 16 | int (g) << 8 | int (b)

# this is not implemented - PySDL2 required
# def copysurface (surface, x1, y1, x2, y2):

# -----

def doubleclick () -> bool:
    """
    Returns True if the last mouse click was a double click.
    """
    return sb.doubleclick ()

# -----

def edelay (msec) -> bool:
    """
    Waits for 'msec' milliseconds. This function returns True if
    an event occurs during the delay, otherwise it returns False.
    """
    return sb.edelay (int (msec))

# -----

def event () -> bool:
    """
    Returns True if an event (mouse click, key press, or QUIT)
    has occurred.
    """
    return sb.event ()

# -----

def eventtype () -> int:
    """
    Returns the type of the last event; either SDL_KEYPRESS
    or SDL_MOUSEBUTTONDOWN.
    """
    return sb.eventtype ()

# -----

def fputpixel (x, y):
    """
    Plots a point at (x, y) using the current drawing colour.
    """
    sb.fputpixel (int (x), int (y))

# -----

def getbuffer (buffer):
    """
    Copies the contents of the active window to 'buffer'.
    """
    sb.getbuffer.argtypes = [c_char_p ]
    sb.getbuffer (buffer)

# -----

def getclick () -> int:
    """
    Waits for a mouse button click and returns its code.
    """
    return sb.getclick ()

# -----

def getcurrentwindow () -> int:
    """
    Returns the id of the current window.
    """
    return sb.getcurrentwindow ()

# -----

def getevent () -> int:
    """
    Waits for a keypress, mouse click, or SDL_QUIT event,
    and returns the code of the key, mouse button, or SDL_QUIT.
    """
    return sb.getevent ()

# -----

def getleftclick ():
    """
    Waits for the left mouse button to be clicked and released.
    """
    sb.getleftclick ()
    
# -----

def getlinebuffer (y, linebuffer):
    """
    Copies the y-th screen line to 'linebuffer'.
    """
    sb.getlinebuffer.argtypes = [c_int, c_char_p ]
    sb.getlinebuffer (y, linebuffer)

# -----

def getmaxheight () -> int:
    """
    Returns the maximum possible height for a new window
    (actual screen height in pixels).
    """
    return sb.getmaxheight ()

# -----

def getmaxwidth () -> int:
    """
    Returns the maximum possible width for a new window
    (actual screen width in pixels).
    """
    return sb.getmaxwidth ()

# -----

def getmiddleclick ():
    """
    Waits for the middle mouse button to be clicked and released.
    """
    sb.getmiddleclick ()

# -----

def getmouseclick (btn, x = None, y = None):
    """
    Sets the x, y coordinates of the last 'btn' button click
    expected by ismouseclick().
    """
    sb.getmouseclick.argtypes = [c_int, POINTER (c_int), POINTER (c_int)]
    if y != None and y != None:
        sb.getmouseclick (btn, x, y)
    else:
        tmp1, tmp2 = c_int(), c_int ()
        sb.getmouseclick (btn, byref (tmp1), byref (tmp2))
        return tmp1.value, tmp2.value

# -----

def getrgbpalette (palette: rgbpalettetype, size):
    """
    Fills the rgbpalettetype structure pointed by 'palette' with
    information about the current ARGB palette's size and colours.
    """
    sb.getrgbpalette.argtypes = [POINTER (rgbpalettetype), c_int]
    tmp = rgbpalettetype ()
    sb.getrgbpalette (byref (palette), int (size))

# -----

def getrgbpalettesize () -> int:
    """
    Returns the current size of the ARGB palette.
    """
    return sb.getrgbpalettesize ()

# -----

def getrightclick ():
    """
    Waits for the right mouse button to be clicked and released.
    """
    sb.getrightclick ()

# -----

def getscreensize (width = None, height = None):
    """
    Reports the screen width and height in 'width' and 'height',
    regardless of current window dimensions.
    """
    sb.getscreensize.argtypes = [POINTER (c_int), POINTER (c_int)]
    if width != None and height != None:
        sb.getscreensize (width, height)
    else:
        tmp1, tmp2 = c_int(), c_int ()
        sb.getscreensize (byref (tmp1), byref (tmp2))
        return tmp1.value, tmp2.value

# -----

def getwindowheight () -> int:
    """
    Equivalent to getmaxy() (WinBGIm compatibility).
    """
    return sb.getmaxy ()

# -----

def getwindowwidth () -> int:
    """
    Equivalent to getmaxx() (WinBGIm compatibility).
    """
    return sb.getmaxx ()

# -----

def initpalette ():
    """
    Initialises the BGI palette to the standard 16 colours.
    """
    sb.initpalette ()

# -----

def initwindow (width, height) -> int:
    """
    Initializes the graphics system, opening a width x height
    window. Set width or height equal to 0 for fullscreen.
    """
    return sb.initwindow (int (width), int (height))

# -----

def ismouseclick (btn) -> int:
    """
    Returns True if the btn mouse button was clicked.
    """
    return sb.ismouseclick (int (btn))

# -----

def kdelay (msec) -> bool:
    """
    Waits for 'msec' milliseconds. This function returns True if
    a key is pressed during the delay, otherwise it returns False.
    """
    return sb.kdelay (int (msec))

# -----

def mouseclick () -> int:
    """
    Returns the code of the mouse button that is being
    clicked, or SDL_MOUSEMOTION, or 0.
    """
    return sb.mouseclick ()

# -----

def mousex () -> int:
    """
    Returns the X coordinate of the last mouse click.
    """
    return sb.mousex ()

# -----

def mousey () -> int:
    """
    Returns the Y coordinate of the last mouse click.
    """
    return sb.mousey ()

# -----

def putbuffer (buffer):
    """
    Copies 'buffer' to the current window.
    """
    sb.putbuffer.argtypes = [c_char_p]
    sb.putbuffer (buffer)

# -----

def putlinebuffer (y, linebuffer):
    """
    Copies 'linebuffer' to the y coordinate in the current window.
    """
    sb.putlinebuffer.argtypes = [c_int, c_char_p]
    sb.putlinebuffer (y, linebuffer)

# -----

def readimagefile (filename, x1, y1, x2, y2):
    """
    Reads a .bmp file and displays it immediately at (x1, y1).
    """
    sb.readimagefile.argtypes = [c_char_p, c_int, c_int, c_int, c_int]
    sb.readimagefile (filename.encode ('ascii'),
                      int (x1), int (y1), int (x2), int (y2))

# -----

def refresh ():
    """
    Updates the screen contents, i.e. displays all graphics.
    """
    sb.refresh ()

# -----

def resetwinoptions (id, title, x, y):
    """
    Resets title and position of window 'id' to 'title', (x, y).
    """
    sb.resetwinoptions (id, title.encode ('ascii'), int (x), int (y))

# -----

def resizepalette (newsize) -> int:
    """
    Resizes the ARGB palette to newsize;
    returns 0 if successful, 1 otherwise.
    """
    sb.resizepalette.argtypes = [c_int]
    return sb.resizepalette (newsize)

# -----

def sdlbgiauto ():
    """
    Triggers auto mode, i.e. refresh() is performed automatically.
    """
    sb.sdlbgiauto ()

# -----

def sdlbgifast ():
    """
    Triggers fast mode, i.e. refresh() is needed to display graphics.
    """
    sb.sdlbgifast ()

# -----

def sdlbgislow ():
    """
    Triggers slow mode i.e. refresh() is not needed to display graphics.
    """
    sb.sdlbgislow ()

# -----

def setallrgbpalette (palette: rgbpalettetype):
    """
    Sets the current ARGB palette to the values given in 'palette'.
    """
    sb.setallrgbpalette.argtypes = [POINTER (rgbpalettetype)]
    sb.setallrgbpalette (byref (palette))

# -----

def setalpha (col, alpha):
    """
    Sets alpha transparency for colour 'col' to 'alpha' (0-255);
    0 means full transparecy, 255 full opacity.
    """
    sb.setalpha (int (col), int (alpha))

# -----

def setbkrgbcolor (color):
    """
    Sets the current background colour to the 'color' entry in
    the ARGB palette.
    """
    sb.setbkrgbcolor (int (color))

# -----

def setblendmode (blendmode):
    """
    Sets the blend mode to be used with screen refresh;
    either SDL_BLENDMODE_NONE or SDL_BLENDMODE_BLEND.
    """
    sb.setblendmode (int (blendmode))

# -----

def setcurrentwindow (id):
    """
    Sets the current active window to 'id'.
    """
    sb.setcurrentwindow (int (id))

# -----

def setrgbcolor (color):
    """
    Sets the current drawing colour to the 'color' entry in
    the ARGB palette.
    """
    sb.setrgbcolor (int (color))

# -----

def setrgbpalette (colornum, red, green, blue):
    """
    Sets the colornum entry in the ARGB palette specifying
    the r, g, and b components.
    """
    sb.setrgbpalette (int (colornum),
                      int (red), int (green), int (blue))

# -----

def setwinoptions (title, x, y, flags):
    """
    Sets the window title 'title', the initial position
    to (x, y), and SDL2 'flags' OR'ed together.
    """
    sb.setwinoptions (title.encode ('ascii'),
                      int (x), int (y), int (flags))

# -----

def setwintitle (id, title):
    """
    Sets the title of the window identified by 'id'.
    """
    sb.setwintitle (int (id), title.encode ('ascii'))

# -----

def showerrorbox (message):
    """
    Opens an error message box with the specified 'message'.
    """
    sb.showerrorbox (message.encode ('ascii'))

# -----

def showinfobox (message):
    """
    Opens an information message box with the specified 'message'.
    """
    sb.showinfobox (message.encode ('ascii'))

# -----

def swapbuffers ():
    """
    Swaps the current active and the current visual graphics pages.
    """
    sb.swapbuffers ()


# -----

# strange bug: screen is garbled when this function is called
def writeimagefile (filename, left, top, right, bottom):
    """
    Writes a .bmp file from the screen rectangle defined by
    'left', 'top', 'right', 'bottom'.
    """
    sb.writeimagefile.argtypes = [c_char_p, c_int, c_int, c_int, c_int]
    sb.writeimagefile (filename.encode ('ascii'),
                       int (left), int (top), int (right), int (bottom))
    
# -----

def xkbhit () -> bool:
    """
    Returns True when any key is pressed, including special keys
    (Ctrl, Shift, etc.)
    """
    return sb.xkb_hit ()

# -----

### ----- End of file sdl_bgi.py
