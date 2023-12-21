#!/usr/bin/env python

"""
life.py: Conway's Game of Life.
By Guido Gonzato, September 2022.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
"""

try:
    from numpy   import *  # matrix support
except ModuleNotFoundError or ImportError:
    print ("Sorry, this program needs Numpy.")
    quit ()
    
from sdl_bgi import *
from sys     import argv

try:
    from numba import jit
    NUMBA_FOUND = True
except ModuleNotFoundError or ImportError:
    NUMBA_FOUND = False

# -----

if NUMBA_FOUND == False:
    print ("Numba not found - program will be slow.")
    def jit (*args, **kwargs):  # define dummy decorator
        def dummy (func):
            return func
        return dummy

# ----- 

SIZE  = 500
ALIVE = 1
DEAD  = 0
max_iter = 100  # max iteration
maxx, maxy = getscreensize ()

a = empty ((SIZE, SIZE))
b = empty ((SIZE, SIZE))

# -----

def initialise (percentage):
    for x in range (SIZE):
        for y in range (SIZE):
            if random (100) < percentage:
                a[x][y] = ALIVE
            else:
                a[x][y] = DEAD

# -----

def main_loop ():
  
    iterations = 0
    stop = 0
    
    while (stop != True):
        evolve_cells (a, b)
        print_cells (b)
        refresh ()
        evolve_cells (b, a)
        print_cells (a)
        refresh ()
        iterations += 1
        if kbhit ():
            if KEY_ESC == lastkey ():
                stop = True
            else:
                initialise (random (100))

# -----

@jit(nopython=True)
def NORTH (n):
    if n == 0:
        return SIZE - 1
    else:
        return n - 1

@jit(nopython=True)
def SOUTH (n):
    if n == SIZE - 1:
        return 0
    else:
        return n + 1

@jit(nopython=True)
def WEST (n):
    if n == 0:
        return SIZE - 1
    else:
        return n - 1

@jit(nopython=True)
def EAST (n):
    if n == SIZE - 1:
      return 0
    else:
      return n + 1

# -----

def print_cells (cells):
  
    for x in range (SIZE):
        for y in range (SIZE):
            if (DEAD == cells[x][y]):
                color = WHITE
            else:
                color = BLUE
            putpixel (x, y, color)

# -----

@jit(nopython=True)
def evolve_cells (old, new):
# evolve cells in array new[][] based on the state of array old[][]

    for x in range (SIZE):
        for y in range (SIZE):
            # count alive neighbours of (x, y) cell, 
            # starting from N, clockwise
            n = 0
            if (ALIVE == old[x][NORTH(y)]):         # N
                n += 1
            if (ALIVE == old[EAST(x)][NORTH(y)]):   # NE
                n += 1
            if (ALIVE == old[EAST(x)][y]):          # E
                n += 1
            if (ALIVE == old[EAST(x)][SOUTH(y)]):   # SE
                n += 1
            if (ALIVE == old[x][SOUTH(y)]):         # S
                n += 1
            if (ALIVE == old[WEST(x)][SOUTH(y)]):   # SW
                n += 1
            if (ALIVE == old[WEST(x)][y]):          # W
                n += 1
            if (ALIVE == old[WEST(x)][NORTH(y)]):   # NW
                n += 1
            if n < 2 or n > 3:
                new[x][y] = DEAD
            else:
                if 3 == n:
                    new[x][y] = ALIVE
                else:
                  new[x][y] = old[x][y]

# -----

def main (args):

    if (len (args) == 1):
        percentage = 10
    else:
      percentage = int (argv[1])
  
    showinfobox ("Press any key to restart,\n"
                 "or ESC to exit.")
    
    initialise (percentage)
    initwindow (SIZE, SIZE)
    main_loop ()
    
    closegraph ()
  
# -----

if __name__ == '__main__':
  main (argv)
  
# ----- end of file life.py
