# About

`SDL_bgi` is a graphics library (`GRAPHICS.H`) for C, C++,
WebAssembly, and Python. It's based on SDL2 and it's portable on many
platforms.

Its name refers to BGI, the Borland Graphics Interface that was the
'de facto' standard in PC graphics back in DOS days; it was made
popular by Borland Turbo C/C++ compilers. I wrote `SDL_bgi` because I
wanted a simple to use but fast graphics library for my experiments
with fractals and cellular automata, using the BGI syntax I'm used to.

`SDL_bgi` is functionally compatible with the BGI implementation in
Turbo C 2.01 and Borland C++ 1.0; for instance, it compiles and runs
the original `bgidemo.c`. `SDL_bgi` also provides nearly full
compatibility with another BGI implementation, WinBGIm (see links
below). One of the aims of `SDL_bgi` is the preservation of old
software written for BGI; but not only that.

`SDL_bgi` provides graphics primitives, and is much easier to use than
plain SDL2; it should be especially useful for beginners, i.e. in
introductory programming courses. `SDL_bgi` is pretty fast, and in
addition to BGI compatibility it provides extensions for ARGB colours,
mouse support, vector fonts, and multiple windows. Native SDL2
functions can be used alongside `SDL_bgi` functions. `SDL_bgi` can
also be used in programs written in C++ or Python.

`SDL_bgi` is written in C, and it should compile on any platform
supported by SDL2. It has been tested on GNU/Linux, MS Windows (MSYS2
and Mingw-w64, CodeBlocks, Dev-C++), macOS (High Sierra and Catalina),
Raspios (ARM, i386), and WebAssembly (Emscripten). A few example
programs in C and Python are provided in the `demo/` directory.


## Requirements

The C version requires the SDL2 library development packages. The
Python module requires the `SDL_bgi` binaries and the SDL2 library
binaries.


## How to Install

Once the dependencies above are met, just do:

```
$ pip install sdl_bgi
```

# How to use `SDL_bgi` in Python programs

Please make sure that you installed the `SDL_bgi` binaries for your
platform before proceeding.


## Implementation Details

`SDL_bgi` Python bindings are implemented via
the [ctypes](https://docs.python.org/3/library/ctypes.html) library.
In general:

- functions in Python have the same name as their C counterparts;

- constants and enums are implemented as variables;

- standard C types (`int`, `float`, `char *`, etc.) are mapped to
  Python types;

- structs are available as `ctypes` classes that have the same name
and field names. For example:

\small

```
C struct                     Python class
--------                     ------------
struct arccoordstype {       class arccoordstype (Structure):
  int x;                         _fields_ = [ ("x", c_int),    
  int y;                                      ("y", c_int),    
  int xstart;                                 ("xstart", c_int)
  int ystart;                                 ("ystart", c_int)
  int xend;                                   ("xend", c_int), 
  int yend;                                   ("yend", c_int) ]
};
```
\normalsize


There are minor differences explained below.


## Syntax differences

`ctypes` implements new types that are mapped to equivalent Python
types; for example, `c_int` is equivalent to `int`. Please refer to
`ctypes`'
[Reference](https://docs.python.org/3/library/ctypes.html#fundamental-data-types).

2D arrays can be implemented via [Numpy](https://numpy.org/); please
see for example `demo/life.py` or `demo/buffers_numpy.py`. Strictly
speaking, Numpy is not required; but working with arrays without it is
a pain.

Memory buffers, used for example by `getimage()` or `getbuffer()`, are
implemented using function `create_string_buffer()`.

The `byref()` function can be used to pass variables by reference, as
in the following functions:

\small

```
# void detectgraph (int *graphdriver, int *graphmode);
graphdriver, graphmode = c_int (), c_int ()
detectgraph (byref (graphdriver), byref (graphmode))
print ("graphdriver, graphmode: ", graphdriver.value, graphmode.value)

# void getarccoords (struct arccoordstype *arccoords);
ac = arccoordstype ()
getarccoords (byref (ac))
print ("x, y, xstart, ystart, xend, yend: ", ac.x, ac.y, 
        ac.xstart, ac.ystart, ac.xend, ac.yend)

# void getaspectratio (int *xasp, int *yasp);
xasp, yasp = c_int (), c_int ()
getaspectratio (byref (xasp), byref (yasp))
print ("xasp, yasp: ", xasp.value, yasp.value)

# void getfillsettings (struct fillsettingstype *fillinfo);
fillinfo = fillsettingstype ()
getfillsettings (byref (fillinfo))
print ("pattern, color: ", fillinfo.pattern, fillinfo.color)

# void getimage ()
isize = imagesize (0, 0, len, 16)
image = create_string_buffer (isize)
getimage (0, 0, len, 16, image)

# void getlinesettings (struct linesettingstype *lineinfo);
lineinfo = linesettingstype ()
getlinesettings (byref (lineinfo))
print ("linestyle, thickness: ", ls.linestyle, ls.thickness)

# void getmoderange (int graphdriver, int *lomode, int *himode);
lomode, himode = c_int (), c_int ()
getmoderange (0, byref (lomode), byref (himode))
print ("lomode, himode: ", lomode.value, lomode.value)

# void getmouseclick (int btn, int *x, int *y);
kind, x, y = c_int (), c_int (), c_int ()
getmouseclick (kind, byref (x), byref (y))
print ("mouse x, mouse y: ", x.value, y.value)

# void getscreensize (int x, int y);
x, y = c_int (), c_int ()
getscreensize (byref (x), byref (y))
print ("size x, size y: ", x, y)
```

\normalsize

## Pythonic Syntax

The following functions provide a more Pytonic syntax that only uses
standard Python types:

\small

```
# void detectgraph (int *graphdriver, int *graphmode);
graphdriver, graphmode = detectgraph ()
print ("graphdriver, graphmode: ", graphdriver, graphmode);

# void getarccoords (struct arccoordstype *arccoords);
ac = arccoordstype ()
ac = getarccoords ()
print ("x, y, xstart, ystart, xend, yend: ", ac.x, ac.y, 
        ac.xstart, ac.ystart, ac.xend, ac.yend)

# void getaspectratio (int *xasp, int *yasp);
xasp, yasp = getaspectratio ()
print ("xasp, yasp: ", xasp, yasp)

# void getfillsettings (struct fillsettingstype *fillinfo);
fs = fillsettingstype ()
fs = getfillsettings ()
print ("pattern, color: ", fs.pattern, fs.color)

# void getlinesettings (struct linesettingstype *lineinfo);
ls = linesettingstype ()
ls = getlinesettings ()
print ("linestyle, thickness: ", ls.linestyle, ls.thickness)

# void getmoderange (int graphdriver, int *lomode, int *himode);
lomode, himode = getmoderange ()
print ("lomode, himode: ", lomode, lomode)

# void getmouseclick (int btn, int *x, int *y);
x, y = getmouseclick (WM_LBUTTONDOWN)
print ("mouse x, mouse y: ", x, y)

# void getscreensize (int x, int y);
x, y = getscreensize ()
print ("size x, size y: ", x, y)

# void initgraph (int *graphdriver, int *graphmode, char *pathtodriver)
initgraph ()
```

\normalsize

## Helper Functions

The following functions can be useful:

`list2vec (list)`: converts a Python list of integers to a vector;
used for example by `drawpoly()`

`vec2buf (vector)`: returns a string buffer that contains the values
stored in `vector`. This is a 1-dimensional array that can be obtained
from a Numpy 2D array 'matrix' with `reshape (matrix, -1)`.

`sizeofint ()`: equivalent to C `sizeof (int)`. Please note that this
is not the same as `sys.getsizeof()`!


## Missing Features

SDL2-based variables `bgi_window`, `bgi_renderer`, `bgi_texture`,
`PALETTE_SIZE`, and function `copysurface()` are not available.


## Speeding Things Up

Python is an interpreted language, and its performance is quite poor
if compared to compiled code. The [PyPy](https://www.pypy.org)
interpreter should make Python code run faster, but `SDL_bgi` programs
run much slower with PyPy than with CPython. Another Python
implementation, [Pyston](https://www.pyston.org), actually runs
`SDL_bgi` programs definitely faster than CPython.

To give your programs a real boost, I strongly suggest that module
[Numba](https://numba.pydata.org/) be used. Numba is a high
performance Python JIT compiler that can translate a large subset of
Python and NumPy code into fast machine code. It uses simple function
decorators; please have a look at `demo/mandelbrot.py` to see how it
works.


## Making Standalone Binaries

To deploy a Python program as a standalone executable file, you may use 
[PyInstaller](https://pyinstaller.org) or [Nuitka](https://nuitka.net/).

### Pyinstaller

Run it as in the following example:

```
test$ pyinstaller -F fern.py 
121 INFO: PyInstaller: 5.4.1
121 INFO: Python: 3.10.4
...
7373 INFO: Building EXE from EXE-00.toc completed successfully.
test$ _
```

The resulting executable will be created in directory `dist/`.

### Nuitka

Run it as in the following example:

```
test$ nuitka3 --onefile --remove-output fern.py
Nuitka-Options:INFO: Used command line options: --onefile \
  --remove-output fern.py
Nuitka:INFO: Starting Python compilation with Nuitka '1.1.3' \
  on Python '3.10' commercial grade 'not installed'.
...
Nuitka:INFO: Successfully created 'fern.bin'.
test$ _
```

When run on Windows, you get `fern.exe` and `fern.cmd`, which is a
batch file that sets up the proper runtime environment for the
executable. Run `fern.cmd` to start the program; on MSYS2, use:

```
test$ start fern.cmd
```

On my GNU/Linux Mint 20.2 box, Nuitka creates a much smaller
executable than Pyinstaller does.


# Links

0. `SDL_bgi` home page:
   <https://sdl-bgi.sourceforge.io/>;
   <https://pypi.org/project/sdl-bgi>

1. The SDL library:
   <https://www.libsdl.org/>

2. BGI on Wikipedia:
   <https://en.wikipedia.org/wiki/Borland_Graphics_Interface>

3. WinBGIm, a BGI port for Windows:
   <https://winbgim.codecutter.org/>

4. Xbgi, a BGI XLib port:
   <http://libxbgi.sourceforge.net/>

5. PyEasyGraphics, a BGI-like Python port:
   <https://github.com/royqh1979/PyEasyGraphics>

6. Borland Turbo C 2.01 online emulator:
   <https://archive.org/details/msdos_borland_turbo_c_2.01>

This library is released under the Zlib license; please see the
enclosed file LICENSE.

Brought to you by Guido Gonzato, PhD
<guido dot gonzato at gmail dot com>
