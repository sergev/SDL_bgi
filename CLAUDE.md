# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

SDL_bgi 3.0.0: a reimplementation of Borland's BGI (`GRAPHICS.H`) on top of SDL2, plus extensions (ARGB colours, mouse, vector/CHR fonts, multiple windows). It aims to compile unmodified Turbo C 2.01 / Borland C++ 1.0 programs (e.g. the original `bgidemo.c`) and to be nearly WinBGIm-compatible. Imported from https://sourceforge.net/projects/sdl-bgi/ (upstream author Guido Gonzato). Zlib license.

## Building

The quickest way is the top-level `Makefile`:
```
make            # builds the library in src/ and every test program in test/
make install    # same as `make -C src install`
make clean
```
It copies the headers into `build/include/` (keeping the installed `SDL2/` layout) and builds the tests with `BGI_CFLAGS`/`BGI_LIBS` pointing at the in-tree library, with an rpath to `src/`. The tests therefore run without installing anything. It does not build `demo/`.

Underneath, two independent build systems produce the same shared library from `src/SDL_bgi.c`:

- **CMake** (output goes in `build/`, which is gitignored):
  ```
  mkdir -p build && cd build && cmake .. && make
  ```
  `mkpkg.sh` wraps this plus `cpack` to build .deb/.rpm (Linux only).
- **Plain Makefile** in `src/` (detects Linux / Darwin / MSYS2 through `uname -s`):
  ```
  cd src && make            # builds libSDL_bgi.so (SDL_bgi.dll on MSYS2)
  make install              # lib -> /usr/local/lib, SDL_bgi.h -> include/SDL2/, graphics.h -> include/
  make python               # installs src/sdl_bgi.py into the user site-packages
  make wasm                 # Emscripten static lib; only if $EMSDK is set
  ```
  `src/Makefile.CodeBlocks` and `src/Makefile.DevCpp` are for the Windows IDEs.

On macOS the `src/`, `test/`, and `demo/` Makefiles get the SDL2 location from `sdl2-config --prefix` (`/usr/local` or `/opt/homebrew`). The library is built as `libSDL_bgi.dylib` with install name `@rpath/libSDL_bgi.dylib` and installed under `/usr/local`.

## Tests and demos

There is no automated test suite. `test/` contains one interactive program per BGI function (`arc.c`, `setviewport.c`, ...), adapted from the Borland C 3.1 Library Reference. They also compile under Turbo C in DOSBox, so keep them TC-compatible. `demo/` contains bigger example programs in C, with Python equivalents (`*.py`).

When run directly, both Makefiles compile against the **installed** headers and library (`/usr/local/include`, `-lSDL_bgi`), not the ones in your tree. To build against the tree, use the top-level `make`, install first, or (in `test/` only) set `BGI_CFLAGS`/`BGI_LIBS` to point at `src/`. `graphics.h` includes `<SDL2/SDL_bgi.h>`, so the include directory must have an `SDL2/` subdirectory:
```
cd test && make            # all test programs
cd test && make circle     # a single test program
cd demo && make mandelbrot
```
`demo/Makefile` downloads `bgidemo.c` with wget when you build the `bgidemo` target. Every program opens an SDL window and waits for a keypress.

## Architecture

- **`src/SDL_bgi.c`** (~6200 lines) is the whole implementation in one file. All state lives in file-`static` globals, on purpose. Only `bgi_window`, `bgi_renderer`, and `bgi_texture` are public, so that users can mix in native SDL2 calls.
- **`src/SDL_bgi.h`** is the real public header (it gets installed as `<SDL2/SDL_bgi.h>`). **`src/graphics.h`** is just a shim that includes it. Both headers use the `_SDL_BGI_H` / `__GRAPHICS_H` include guards.
- **Drawing model:** pixels are written into software ARGB buffers (`bgi_activepage[win]` / `bgi_visualpage[win]`, through the `PIXEL(X,Y)` macro). The buffers are copied to an SDL texture and presented. Write modes (COPY/XOR/AND/OR/NOT) are dispatched to separate `putpixel_*` / `line_*` functions.
- **Refresh modes:** this is the main performance/compatibility trade-off.
  - `initgraph()` sets "slow mode" (`bgi_fast_mode = SDL_FALSE`): the screen refreshes after every primitive, as in BGI.
  - `initwindow()` sets "fast mode": the user calls `refresh()` to update the screen.
  - "Auto mode" is enabled by `SDL_BGI_RATE=auto` or by a numeric rate.
  - `getch()`, `kbhit()`, and `delay()` also refresh the screen.
  - `sdlbgifast()`, `sdlbgislow()`, and `sdlbgiauto()` switch between modes at runtime.
  - Some primitives temporarily force fast mode internally and restore it afterwards.
- **Multiple windows:** up to `NUM_BGI_WIN` windows, held in per-window arrays (`bgi_win[]`, `bgi_rnd[]`, `bgi_txt[]`, page buffers). The current window is selected with `setcurrentwindow()`.
- **Fonts:** the 8×8 bitmap font is embedded as `bgi_bitmap_font`. The ten Borland vector fonts are the generated headers `src/{trip,litt,sans,goth,scri,simp,tscr,lcom,euro,bold}.h`, decoded from the original `.CHR` files (see `tmp/chr_decoder.c`). Don't hand-edit them. Users' `.CHR` files are parsed at runtime by `decode()` and loaded with `installuserfont()`.
- **`SDL_BGI_RES=VGA`** forces 640×480 when `initgraph()` is called with `DETECT`. Under Emscripten it is read from a file named `SDL_BGI_RES`, since there's no environment.
- **Python binding:** `src/sdl_bgi.py` is a pure-`ctypes` wrapper around the shared library (no PySDL2). Each C API function has a hand-written Python counterpart. **`pypi/src/sdl_bgi/sdl_bgi.py` is a separate copy** used for the PyPI package (`pypi/pyproject.toml`). When the C API changes, update `SDL_bgi.h`, `sdl_bgi.py`, the pypi copy, and the docs together.
- **`tmp/`** contains helper programs used during development (font and palette extraction), not part of the library.

## Versioning and docs

The version string appears in several places that must be kept in sync: `VERSION`, `CMakeLists.txt` (`SDL_BGI_VERSION`), `SDL_bgi.h` (`#define SDL_BGI_VERSION`), `sdl_bgi.spec`, `pypi/pyproject.toml`, and the `INSTALL_*.md` files.

`doc/*.md` are the source files. The `.html`/`.pdf` files next to them are generated with pandoc (the command is in each file's header comment). The man page `doc/graphics.3.gz` is generated from `graphics.3.md`. `doc/functions.md` and `doc/compatibility.md` document every API function and its deviations from Turbo C / WinBGIm, so update them when you change behaviour.
