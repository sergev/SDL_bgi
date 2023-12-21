#!/usr/bin/env python3

"""
plasma.py: writes the window contents to a `.bmp` file.
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
from math    import *

PI = 3.141593

# int i, x, y, r, g, b
cols = empty ((3, 256))
  
initwindow (600, 600)
  
for i in range (255):
  cols[0][i] = abs( (128. - 127. * sin (i * PI / 32.0)))
  cols[1][i] = abs( (128. - 127. * sin (i * PI / 64.)))
  cols[2][i] = abs( (128. - 127. * sin (i * PI / 128.)))
  
for y in range (getmaxy ()):
  for x in range (getmaxx()):
    c = (sin(x/35.)*128. + sin(y/28.)*32. + sin((x+y)/16.)*64.)
    if (c > 255):
      c = c - 256
    if (c < 0):
      c = 256 + c
    r = cols[0][int (c)]
    if (r > 255):
      r = r - 256
    if (r < 0):
      r = 256 + c
    g = cols[1][int (c)]
    if (g > 255):
      g = g - 256
    if (g < 0):
      g = 256 + c
    b = cols[2][int (c)]
    if (b > 255):
      b = b - 256
    if (b < 0):
      b = 256 + c
    putpixel (x, y, COLOR(r, g, b))

refresh ()
getch ()
writeimagefile ("plasma.bmp", 0, 0, 599, 599)
closegraph ()

# ----- end of file plasma.py
