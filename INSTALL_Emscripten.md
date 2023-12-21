# `SDL_bgi` and Emscripten

Unmodified `SDL_bgi` programs can be compiled to WebAssembly using the
[Emscripten](https://emscripten.org/) compiler `emcc`. The following
tools are used to produce standalone `html` files that can be run in
supported browsers:

- `src/Makefile` provides a `wasm` target, only available when the
`EMSDK` environment variable is defined;
- `test/emcc.sh` can be used to compile a program;
- `test/Makefile.emcc` compiles the sample programs.

Emscripten support was tested with `emcc` 3.1.22 (latest git version)
and `clang'' 14.0.0 on GNU/Linux Mint 20.2 and MSYS2 20220906.


## Installing Emscripten Support

Emscripten must be properly installed, and the `EMSDK` environment
variable must be defined; please consult the Emscripten [Download and
install](https://emscripten.org/docs/getting_started/downloads.html)
page.

To compile `SDL_bgi` and install Emscripten support:

```
$ cd src/
src/$ make wasm
*** Building on Linux ***
...
src/$ make clean
```

Files will be installed in appropriate directories:

```
graphics.h    -> $EMSDK/upstream/emscripten/cache/sysroot/include
SDL_bgi.h     -> $EMSDK/upstream/emscripten/cache/sysroot/include/SDL2
libSDL_bgi.a  -> $EMSDK/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten
```

To uninstall:

```
$ cd src/
src/$ make unwasm
```

