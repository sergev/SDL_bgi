# module sdl_bgi.py

"""
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
3. This notice may not be removed or altered from any source distribution
"""

__doc__ = """
`SDL_bgi` is a graphics library (`GRAPHICS.H`) for C, C++, WebAssembly,
and Python. It's based on SDL2 and it's portable on many platforms.

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
supported by SDL2; it has been tested on GNU/Linux, MS Windows (MSYS2
+ Mingw-w64, CodeBlocks, Dev-C++), macOS (High Sierra and Catalina),
Raspios (ARM, i386), and WebAssembly (Emscripten). A few example
programs are provided in the `test/` directory.
"""

from .sdl_bgi import *

# --- end of file __init__.py
