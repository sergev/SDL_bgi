#!/usr/bin/env python

"""
fern.py: a famous IFS system.

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

gd = DETECT
gm = SDL_800x600

stop = 0
counter = 0
# midx, midy, scale
# int k = 0, prob
# float x, y, xx, yy
  
a = [0.0, 0.85, 0.2, -0.15]
b = [0.0, 0.04, -0.26, 0.28]
c = [0.0, 0.04, 0.23, 0.26]
d = [0.16, 0.85, 0.22, 0.24]
e = [0.0, 0.0, 0.0, 0.0]
f = [0.0, 1.6, 1.6, 0.44]

setwinoptions ("This is a fractal fern", -1, -1, -1)
initgraph (gd, gm, "")
setbkcolor (COLOR (0, 0, 40))
cleardevice ()
setcolor (YELLOW)
outtextxy (0, 0, "Press a key to exit: ")

midx = getmaxx () / 2
midy = getmaxy () / 2
scale = getmaxx () / 16
x = 0.0
y = 0.0
setcolor (COLOR (random (256), random (256), random (256)))
  
# kbhit (), mouseclick () and event () are slow. Use them sparingly.
  
while (stop != 1):

    prob = 1 + random (100)
    if (prob == 1):
       k = 0
    if ( (prob > 1) and (prob < 87)):
       k = 1
    if ( (prob > 86) and (prob < 94)):
       k = 2
    if (prob > 93):
       k = 3
    # to use equal probability, just use:
    # k = random (5)

    xx = a[k] * x + b[k] * y   # + e[k]
    yy = c[k] * x + d[k] * y + f[k]
    x = xx
    y = yy

    fputpixel ((int)(midx + scale * x), (int)(2 * midy - scale * y))
    counter = counter + 1
    
    if (0 == (counter % 100000)):
        setcolor (COLOR (random (256), random (256), random (256)))
        refresh ()
        if (kbhit ()):
          stop = 1

    if (100000000 == counter): # 100 million pixels
        break
  
setcolor (GREEN)
outtextxy (0, 10, "Ok, leaving in 2 seconds")
delay (2000)
closegraph ()

# -----  end of file fern.py -----
