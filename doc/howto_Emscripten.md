---
header-includes: |
  \usepackage{libertine}
  \renewcommand{\ttdefault}{cmtt}
...

<!---

  Convert this file with:

  pandoc -V urlcolor=blue howto_Emscripten.md -o howto_Emscripten.pdf

--->

# `SDL_bgi` and Emscripten

Unmodified `SDL_bgi` programs can be compiled to WebAssembly using the
[Emscripten](https://emscripten.org/) compiler `emcc`; please see
`INSTALL_Emscripten.md`.

When Emscripten support is installed, the following tools are used to
make standalone `html` files that can be run in browsers without
the need of a local webserver:

- `demo/emcc.sh` can be used to compile a program;
- `demo/Makefile.emcc` compiles the sample programs.

The `emcc` flag `-s ASYNCIFY` is used to simulate infinite loops
without modifiying the program sources, at the expense of some
performance hit.


## Compiling Programs

Compilation example:

```
demo$ ./emcc.sh life.c
demo$ firefox ./life.html &
```

The script `emcc.sh` uses one of the available shells in directory
`demo/shells`, and embeds files that exist in `demo/assets`. Type
`./emcc.sh -h` for a brief explanation.

To compile nearly all available demo programs:

```
demo$ make -f Makefile.emcc
```

Not all programs can be compiled, though. For example, `loadimage.c`
makes no sense when run in a browser; in fact, it expects to find files
in the local file system, which is not accessible by the browser.

To compile a program directly in the command line:

```
demo$ emcc --emrun -o program.html program.c -lSDL_bgi \
    -std=gnu99 -O2 -Wall -lm \
    -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY -s SINGLE_FILE
```

The `-s SINGLE_FILE` flag can be omitted if you are running a local
web server.


## Available shells

Emscripten shell files are `html` templates that define how a program
will look when run in the browser. Four shells are provided:

`sdl_bgi.html`: canvas + text area, no logo

`shell_minimal.html`: taken from
[Emscripten sources](https://github.com/emscripten-core/emscripten/blob/main/src/shell_minimal.html)

`canvas_only.html`: canvas only, no text area, no logo

`fullwindow.html`: canvas only, full window, resizeable, no border.
This shell is quite slow.

If no shell is specified, the default Emscripten shell will be used.


## Environment Files

Environment variables don't exist in a browser environment. However,
the same result as using variables can be obtained writing
"environment files" in the `assets/` directory. The file
`SDL_BGI_RES`, containing the string `VGA`, will have the same effect
as the environment variable `SDL_BGI_RES`. The same holds for
`SDL_BGI_PALETTE`, value `BGI`. Files must exist at compile time.


## Caveats

SDL2 mutex support is not available in current releases of Emscripten.
Hence, all references to mutex-related functions in `SDL_bgi.c` are
excluded from compilation. For example:

```
#ifndef __EMSCRIPTEN__
static SDL_mutex
  *bgi_update_mutex = NULL;
#endif
```

As a result, automatic refresh is not available.

Programs running in fullscreen mode need keyboard input to toggle
fullscreen mode in the browser.
