#!/bin/sh

# This script compiles a .C source using emscripten, which is
# assumed to be installed and working. To run the resulting program,
# open $NAME.html in a web browser.

# Tested on GNU/Linux only!

if [ $# -eq 0 ] ; then
  echo "Usage: $0 [-c] [-f] [-m] [-s] <file.c>"
  exit 1
fi

if ! [ -x "$(command -v emcc)" ]; then
  echo 'Error: emcc is not installed.' >&2
  exit 1
fi

SHELLFILE=""
SHELL=""

while getopts ":cfmsh" opt; do
  case ${opt} in
    c ) SHELLFILE="./shells/canvas_only.html"
      ;;
    f ) SHELLFILE="./shells/fullwindow.html"
      ;;
    m ) SHELLFILE="./shells/shell_minimal.html"
      ;;
    s ) SHELLFILE="./shells/sdl_bgi.html"
      ;;
    h ) echo "Usage: $0 [-c] [-f] [-m] [-s] <file.c>"
        echo "-c: use ./shells/canvas_only.html"
	echo "-f: use ./shells/fullwindow.html"
	echo "-m: use ./shells/shell_minimal.html"
	echo "-s: use ./shells/sdl_bgi.html"
	exit 1
      ;;
  esac
done

if [ -n "$SHELLFILE" ] ; then
  SHELL="--shell-file "$SHELLFILE
fi

# program must be the last argument
for arg in "$@" ; do : ; done

NAME=$(basename $arg .c)

# load files in 'assets/' as if they were in local directory '.'
EMBED="--embed-file ./assets@/."
CFLAGS="-std=gnu99 -O2 -Wall"

emcc $CFLAGS -o $NAME.html \
$SHELL $INCLUDE $EMBED $NAME.c -lSDL_bgi \
-s USE_SDL=2 \
-s ALLOW_MEMORY_GROWTH=1 \
-s ASYNCIFY \
-s SINGLE_FILE

# --- end of file emcc.sh
