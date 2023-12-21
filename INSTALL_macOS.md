# Installing `SDL_bgi` on macOS

The easiest way to install `SDL_bgi` requires the Terminal, the
`clang` compiler, the Homebrew package manager, and the SDL2 library.
Full Xcode is not needed.

- open the Terminal and type `clang`. If it's missing, you'll be
  prompted to install it, as part of Xcode's Command Line Tools.

- go to <https://brew.sh> and install Homebrew using the provided
  command. It will take a few minutes.

- type `brew install SDL2` to install the SDL2 library.

- optional: unless you are prepared to use `vim` to edit your
  programs, install a text editor of your liking. See list of
  available options at <https://formulae.brew.sh/formula/>

You're now ready to compile `SDL_bgi`.


## Compiling from sources

To compile and install `SDL_bgi`, start Terminal and run the following
commands:

    % VERSION=3.0.0
    % tar xvf SDL_bgi-$VERSION.tar
    % cd SDL_bgi-$VERSION/src
    % make
    % make install

Files will be installed in these directories:

    SDL_bgi.h      ->  /usr/local/include/SDL2/
    graphics.h     ->  /usr/local/include/
    libSDL_bgi.so  ->  /usr/local/lib/
    sdl_bgi.py     ->  last entry in Python3 sys.path

To uninstall:

    % make uninstall


## Test Programs

Once `SDL_bgi` is installed, you can compile some test programs you
can find in the `test/` directory. Type:

    % cd test && make

Please file see file `doc/using.pdf`.

Enjoy!

Guido =8-)
