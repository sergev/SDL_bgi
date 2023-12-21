#!/usr/bin/env python

"""
dla.py: diffusion limited aggregation.

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

# -----

SIZE1 = 800
SIZE2 = 600
ALIVE = 1
DEAD  = 0
iterations = 0

screen = empty ((SIZE1, SIZE2))

# -----

def NORTH (n):
    if n == 0:
        return SIZE2 - 1
    else:
        return n - 1

def SOUTH (n):
    if n == SIZE2 - 1:
        return 0
    else:
        return n + 1

def WEST (n):
    if n == 0:
        return SIZE1 - 1
    else:
        return n - 1

def EAST (n):
    if n == SIZE1 - 1:
        return 0
    else:
        return n + 1

# -----

def random_walk ():
  
    done = False
    global iterations
  
    x = random (SIZE1)
    y = random (SIZE2)
    
    # start random walk until another pixel is hit
  
    while (done == False):
    
        n = random (8)
    
        if n == 0:
          x = EAST(x)
    
        if n == 1:
          x = EAST(x)
          y = NORTH(y)
    
        if n == 2:
          y = NORTH(y)
    
        if n == 3:
          x = WEST(x)
          y = NORTH(y)
    
        if n == 4:
          x = WEST(x)
    
        if n == 5:
          x = WEST(x)
          y = SOUTH(y)
    
        if n == 6:
          y = SOUTH(y)
    
        if n == 7:
          x = EAST(x)
          y = SOUTH(y)
    
        # any neighbours?
    
        if screen[EAST(x)][y] != 0 or \
           screen[EAST(x)][NORTH(y)] != 0 or \
           screen[x][NORTH(y)] != 0 or \
           screen[WEST(x)][NORTH(y)] != 0 or \
           screen[WEST(x)][y] != 0 or \
           screen[WEST(x)][SOUTH(y)] != 0 or \
           screen[x][SOUTH(y)] != 0 or \
           screen[EAST(x)][SOUTH(y)] != 0:
            screen[x][y] = ALIVE
            iterations += 1
            done = True
  
    color = int (iterations / 150)
    if color > 1024:
        color = 1024
    setrgbcolor (color)
    fputpixel (x, y)
    iterations += 1

# -----

def makepalette ():
    # set up a palette

    # initialise it to black
    for x in range (PALETTE_SIZE):
        setrgbpalette (x, 0, 0, 0)
    
    # blue fading to white
    for x in range (128):
        setrgbpalette (x, 2*x, 2*x, 255)
    
    # white fading to yellow
    for x in range (128, 256, 1):
        setrgbpalette (x, 255, 255, 255 - x)
    
    # yellow fading to red
    for x in range (256, 512, 1):
        setrgbpalette (x, 255, 512 - x, 0)
    
    # red fading to purple
    for x in range (512, 768, 1):
        setrgbpalette (x, 768 - x, 0, x)
    
    # purple fading to black
    for x in range (768, 1024, 1):
        setrgbpalette (x, 1024 - x, 0, 1024 - x)
    
    # the window is full after ~ 180k iterations.

# -----

for x in range (int (SIZE1 / 30)):  # initial seeds
    screen[random(SIZE1)][random(SIZE2)] = ALIVE

initwindow (SIZE1, SIZE2)
setbkcolor (BLACK)
setcolor (RED)
cleardevice ()
settextjustify (CENTER_TEXT, CENTER_TEXT)
outtextxy (getmaxx() / 2, getmaxy () / 2,
           "Press a key to exit")

getch ()
cleardevice ()
refresh ()
makepalette ()
stop = False

while (stop != True):

    # lay a particle
    random_walk ()

    if (iterations % 10 == 0):
        refresh ()
        if kbhit ():
            stop = 1

print ("Iterations computed: %d\n" % iterations)
closegraph ()

# ----- end of file dla.py
