# Installing `SDL_bgi` on Windows

The easiest way to install `SDL_bgi` is by using the binary packages
provided at <http://sdl-bgi.sourceforge.io/#download>, for MSYS2 +
mingw-w64, CodeBlocks, and Dev-C++. Alternatively, you can easily
compile `SDL_bgi` yourself, as shown below.

The following sections were tested with:

- msys2-x86_64-20220704
- codeblocks-20.03mingw
- Dev-Cpp 5.11 + TDM-GCC 4.9.2
- SDL2-2.0.24

**Note 1:** please upgrade MSYS2 before installing `SDL_bgi`; previous
released suffered from bugs in console handling.

**Note 2:** sorry, MSVC is not supported. This compiler is bloated,
incompatible with gcc and clang, is not free, and requires significant
changes to `SDL_bgi` source code.


## Installing the MSYS2 + MINGW64 Binaries

Provided binaries were compiled on MSYS2 and its native package
`mingw-w64-x86_64-SDL2` version 2.0.24-1.

Start the MSYS2 shell, unzip the `SDL_bgi` archive, then copy:

    src/graphics.h           -> /mingw64/include/
    src/SDL_bgi.h            -> /mingw64/include/SDL2/
    bin/Mingw64/SDL_bgi.dll  -> /mingw64/bin

Please note that to run a program compiled with `SDL_bgi` outside of
the MSYS2 environment (e.g. in an ordinary Windows command prompt),
you will need to copy `SDL_bgi.dll` and `SDL2.dll` to the same
directory as the program.


## Installing the Code::Blocks Binaries

Let's assume that you installed CodeBlocks in `C:\CodeBlocks` and SDL2
development libraries in in `C:\SDL2-2.0.24`. Change the following
instructions as needed for your installation.

Unzip `SDL_bgi` archive, then copy:

    src/graphics.h  -> C:\CodeBlocks\MinGW\include
    src/SDL_bgi.h   -> C:\SDL2-2.0.24\i686-w64-mingw32\include\SDL2
    bin/CodeBlocks/SDL_bgi.dll  -> C:\CodeBlocks\MinGW\bin

To learn how to compile programs, please see `howto_CodeBlocks.md`.


## Installing the Dev-C++ Binaries

Let's assume that you installed Dev-C++ in `C:\DevCpp` and SDL2
in `C:\SDL2-2.0.24`. Change the following instructions as needed for
your installation.

Unzip `SDL_bgi` archive, then copy:

    src/graphics.h  -> C:\SDL2-2.0.24\i686-w64-mingw32\include
    src/SDL_bgi.h   -> C:\SDL2-2.0.24\i686-w64-mingw32\include\SDL2
    bin/DevCpp/SDL_bgi.dll  -> C:\DevCpp\MinGW64\bin

To learn how to compile programs, please see `howto_DevCpp.md`.


# Compiling from sources

Windows support is provided for MSYS2 + Mingw-w64 toolchain,
CodeBlocks, and Dev-C++. The MSYS2 shell is required.


## MSYS2 + Mingw-w64

Please make sure you have SDL2 development packages. You should
install `make`, `mingw-w64-x86_64-SDL2`, and `mingw-w64-x86_64-gcc`.
**Do not** install plain `gcc`. Also, make sure that directory
`/mingw64/bin` in included in the $PATH:

    $ pacman -S make mingw-w64-x86_64-SDL2 mingw-w64-x86_64-gcc
    # ...
    $ # expand the $PATH variable if necessary
    $ echo export PATH=/mingw64/bin:$PATH >> $HOME/.bashrc

The procedure was tested on `msys2` and its native package
`mingw-w64-x86_64-SDL2` version 2.0.24-1.

Start the MSYS2 shell, then run the following commands:

    $ VERSION=3.0.0
    $ tar zxvf SDL_bgi-$VERSION.tar.gz
    $ cd SDL_bgi-$VERSION/src
    $ make
    $ make install

Files will be installed in these directories:

    SDL_bgi.h    ->  /mingw64/include/SDL2/
    graphics.h   ->  /mingw64/include/
    SDL_bgi.dll  ->  /mingw64/bin/
    sdl_bgi.py   ->  last entry in Python sys.path
                     (usually )

To uninstall:

    $ make uninstall


## MSYS2 + Code::Blocks

Start the MSYS2 shell, then run the following commands:

    $ VERSION=3.0.0
    $ tar zxvf SDL_bgi-$VERSION.tar.gz
    $ cd SDL_bgi-$VERSION/src
    $ PATH=/c/CodeBlocks/MinGW/bin/:$PATH && make -f Makefile.CodeBlocks
    $ make install

Assuming that you installed CodeBlocks in `C:\CodeBlocks` and SDL2
development libraries in in `C:\SDL2-2.0.24`, files will be installed
in these directories:

    SDL_bgi_.h   ->  C:\SDL2-2.0.24\i686-w64-mingw32\include\SDL2
    graphics.h   ->  C:\CodeBlocks\MinGW\include
    SDL_bgi.dll  ->  C:\CodeBlocks\MinGW\bin


## MSYS2 + Dev-C++

Start the MSYS2 shell, then run the following commands:

    $ VERSION=3.0.0
    $ tar zxvf SDL_bgi-$VERSION.tar.gz
    $ cd SDL_bgi-$VERSION/src
    $ PATH=/c/CodeBlocks/MinGW/bin/:$PATH && make -f Makefile.DevCpp
    $ make install

Assuming that you installed Dev-C++ in `C:\DevCpp` and SDL2
development libraries in in `C:\SDL2-2.0.24`, files will be installed
in these directories:

    SDL_bgi_.h   ->  C:\SDL2-2.0.24\i686-w64-mingw32\include\SDL2
    graphics.h   ->  C:\SDL2-2.0.24\i686-w64-mingw32\include
    SDL_bgi.dll  ->  C:\DevCpp\MinGW64\bin


## Test Programs

Once `SDL_bgi` is installed, you can compile some test programs you
can find in the `test/` directory. Type:

    $ cd test && make

Please see the `using.md` file in the `doc/` directory.

Enjoy!

Guido =8-)
