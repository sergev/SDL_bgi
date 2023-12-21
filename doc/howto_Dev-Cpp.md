---
header-includes: |
  \usepackage{libertine}
  \renewcommand{\ttdefault}{cmtt}
...

<!---

  Convert this file with:

  pandoc -V urlcolor=blue howto_Dev-Cpp.md -o howto_Dev-Cpp.pdf

--->

# How to compile `SDL_bgi` programs with Dev-C++

These instructions show how to compile an `SDL_bgi` program using
Dev-C++; we will use `fern.c`, provided in the `demo/` directory.

Let's assume that Dev-C++ is installed in `C:\Dev-Cpp`, and that the
SDL2 development libraries for MinGW are installed in
`C:\SDL2-*`. If you installed the software in different
directories, change the following instructions accordingly.

The procedure was tested with Orwell Dev-Cpp 5.11, TDM64-GCC 10.3.0,
and SDL2-devel-2.24.2-mingw.tar.gz. Newer releases of these packages
should work.

Links:

<https://sourceforge.net/projects/orwelldevcpp/>

<https://libsdl.org/download-2.0.php>

<https://jmeubank.github.io/tdm-gcc/>


## Upgrading Dev-C++'s Compiler

By default, Dev-C++ ships with TDM-GCC 4.9.2; this is a very old
version of `gcc` that is not compatible with current SDL2 development
libraries. To compile `SDL_bgi` programs, you must upgrade TDM-GCC to
a newer release.

- remove the directory `C:\Dev-Cpp\MinGW64`

- install TDM64-GCC in `C:\Dev-Cpp\MinGW64`

- start Dev-C++. A window will appear, reporting missing compiler 
  directories. Click on `Yes`

- from menu `Tools/Compiler Options.../Compiler set to configure`, 
  click on the `++` icon (`Find and automatically configure compilers`)

- when asked to `Clear current compiler list?`, click on `Yes`

- `TDM-GCC 10.3.0 64-bit release` will be set as the `Compiler set to
  configure`. Click on `Ok`

- test Dev-C++ and make sure that you can compile programs.


## Installing `SDL_bgi`

- copy `bin/Dev-Cpp/SDL_bgi.dll` to\
  `C:\Dev-Cpp\MinGW64\lib`

- copy `src/SDL_bgi.h` to\
  `C:\SDL2-*\x86_64-w64-mingw32\include\SDL2`

- copy `src/graphics.h` to\
  `C:\SDL2-*\x86_64-w64-mingw32\include`


## How to compile

- Run Dev-C++ and start an empty project called `Fern`: menu
  `File/New/Project...`, tick "C Project", select "Empty Project",
  insert the name "Fern", click on Ok, then save the project file

- in the left-hand pane, click on tab `Project`, remove `Untitled`,
  right click on `Fern`, click on `Add to project...`, then pick `fern.c`

- menu `Project/Project options...`, tab `Parameters`, tab `Linker`:
  add the string `-lmingw32 -lSDL_bgi -lSDL2main -lSDL2 -mwindows`

- menu `Project/Project options...`, tab `Parameters`, tab `Linker`:
  click on `Add library or object` and select `SDL_bgi` from\
  `C:\Dev-Cpp\MinGW64\lib`

- tab `Directories`, tab `Include Directories`: add\
  `C:\SDL2-*\x86_64-w64-mingw32\include\SDL2` and\
  `C:\SDL2-*\x86_64-w64-mingw32\include`

- tab `Library Directories`: add\ 
  `C:\SDL2-*\x86_64-w64-mingw32\lib` and\
  `C:\Dev-Cpp\MinGW64\lib` then click on Ok

- compile the program: menu `Execute/Compile`

- copy ``C:\SDL2-*\x86_64-w64-mingw32\bin\SDL2.dll` and\
 `SDL_bgi.dll` to the same directory as `fern.exe`

- run it: menu `Execute/Run`.

**Note 1**: if you release your compiled program, you must
also provide `SDL2.dll` and `SDL_bgi.dll` in the same directory.

**Note 2**: if your code uses functions provided by `stdio.h` (e.g.
`printf()`, `scanf()`, etc.) you must also run a terminal. Remove the
option `-mwindows` from the linker parameters.


[]: # Absurdly complicated. Give me a command line anytime.
