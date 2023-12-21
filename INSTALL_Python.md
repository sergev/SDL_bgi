# Installing `SDL_bgi` for Python

The `sdl_bgi.py` Python module has been tested with Python 3.10.6
(GNU/Linux, Windows, MSYS2) using the standard interpreter, IDLE,
IPython, ptpython, and Pyston. It uses standard modules `ctypes`,
`sysconfig`, and `random`; `PySDL2` is not required.

To use `sdl_bgi.py` in Python programs, first you must install the
`SDL_bgi` binaries; please follow the instructions for your operating
system. When you're done, choose one of the following options. 


## GNU/Linux, macOS

Install `sdl_bgi.py` the usual way:

```
$ pip install sdl_bgi
```

Alternatively, you can install `sdl_bgi.py` with:

```
$ cd src/
src $ make python
```

which installs `sdl_bgi.py` in the user's site package directory (e.g.
`$HOME/.local/lib/python3.10/site-packages/`).

Now `sdl_bgi.py` can be used in scripts, in the default Python
interpreter, in IDLE, and IPython.


## Windows: Native Python and `cmd`

This method employs Python, installed from
<https://python.org/download/windows>, and the standard Windows
command prompt (`cmd`).

Copy `SDL2.dll`, `bin/Mingw64/SDL_bgi.dll`, and `src/sdl_bgi.py` to
your Python directory, i.e. the directory where you save your Python
programs. let's assume its path is `C:\Users\yourname\MyPython`. 
`SDL2.dll` is included in package `SDL2-X.Y.ZZ-win32-x64.zip` from the
SDL2 download page.

To run a program, start `cmd` and type:

```
C:\Documents and Settings\yourname>cd MyPython

C:\Documents and Settings\yourname\MyPython>python program.py
```

Please note that the Python console, IPython, and ptpython won't work;
the graphical window will freeze and won't respond anymore. Go figure.


## Windows: IDLE

As above, copy `SDL_bgi.dll`, `SDL2.dll`, and `sdl_bgi.py` to your
Python directory; let's assume its path is
`C:\Users\yourname\MyPython`. Start IDLE, then run the following
commands:

```
from os import chdir
chdir ("MyPython")
from sdl_bgi import *
# write your Python code...
```

## Windows: MSYS/Mingw64

This method uses one of the native Python packages for MSYS2, i.e.
`mingw-w64-x86_w64-python`.

Install `sdl_bgi.py` the usual way:

```
$ pip install sdl_bgi
```

You can also install `sdl_bgi.py` with:

```
$ cd src/
src $ make python
```

which installs `sdl_bgi.py` in `sys.path[-3]`, that is 
`$HOME/.local/lib/python3*/site-packages/`.

As above, interactive Python shells will not work; scripts and IDLE
work fine. 

Please also see `doc/howto_Python.md`.

Enjoy!

Guido =8-)
