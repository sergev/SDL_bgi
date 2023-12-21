#!/usr/bin/env python3

"""
hopalong.py: an implementation of Barry Martin's algorithm.
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

from sdl_bgi import *
from math    import sqrt
from random  import seed
from sys     import argv

def sgn(n):
    if (n < 0):
      return -1
    else:
      return 1

# -----

def main (args):

    if (len (args) == 1):
        s = 1922
    else:
        s = int (argv[1])
  
    seed (s)
    gd = DETECT
    gm = getmaxmode ()
    initgraph(gd, gm, "")
    
    setbkcolor (BLACK)
    cleardevice ()
    setcolor (YELLOW)
    outtextxy (0, 0, "Press a key or click to exit: ")
    
    xoffs = getmaxx () / 2
    yoffs = getmaxy () / 2
    j = random (100)
    k = random (100)
    x = y = xx = xp = yp = r = 0.0
    stop, counter = 0, 0
    setcolor (COLOR (random (256), random (256), random (256)))
    
    while (stop != 1):
        xx = y - sgn (x) * sqrt (abs (k * x - 1))
        y = j - x
        x = xx
        xp = x * 2 + xoffs
        yp = y * 2 + yoffs
        fputpixel (xp, yp)
        counter += 1
        if counter == 50000:
            refresh ()
            counter = 0
            setcolor (COLOR (random (256), random (256), random (256)))
            refresh ()
            if ismouseclick (WM_LBUTTONDOWN) or kbhit ():
                stop = 1
    
    closegraph ()
    if (SDL_KEYDOWN == eventtype ()):
        print ("Key pressed.")
    else:
        print ("Mouse clicked.")

# -----
      
if __name__ == '__main__':
  main (argv)


# ----- end of file hopalong.py
