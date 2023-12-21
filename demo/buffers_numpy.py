#!/usr/bin/env python3

"""
buffers_numpy.py: getbuffer() / putbuffer() demonstration. This
version uses Numpy; please see 'buffers.py' for a Numpy-less version.

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

WIDTH  = 256
HEIGHT = 256
SIZE   = WIDTH * HEIGHT * sizeofint ()

# plain 'int' raises overflow error
red  = empty ((WIDTH, HEIGHT), dtype=int64)
hues = empty ((WIDTH, HEIGHT), dtype=int64)

# initialise the buffers
for y in range (HEIGHT):
    for x in range (WIDTH):
        # fill the red buffer with 0xffff0000
        red [x, y] = 0xffff0000
        # fill the hues buffer with colorRGB (x, y, abs (x - y))
        hues [x, y] = colorRGB (x, y, abs (x - y))

setwinoptions ("Using putbuffer()", -1, -1, -1)
initwindow (WIDTH, HEIGHT)
setcolor (BLACK)
setbkcolor (WHITE)
cleardevice ()
outtextxy (0, 0, "Click to continue:")
refresh ()
getclick ()

# create a buffer for 'red' array
b_red = vec2buf (reshape (red, -1))
putbuffer (b_red)
refresh ()

# create a temporary buffer
b_tmp = vec2buf (reshape (red, -1))
getbuffer (b_tmp)  # b_tmp == red
setcolor (WHITE)
outtextxy (0, 0, "Left click to continue:")
refresh ()
getclick ()
# create a buffer for 'hues' array
b_hues = vec2buf (reshape (hues, -1))

for alpha in range (128):
  
    for n in range (0, SIZE, 4):  
        b_hues[n + 3] = 0x00 | alpha  # set color alpha
      
    putbuffer (b_tmp)   # red screen
    putbuffer (b_hues)  # hues + alpha on red
    refresh ()
    delay (10)
    
outtextxy (0, 0, "Left click to finish:")
refresh ()
getclick ()

closegraph ()
  
# ----- end of file buffers.py
