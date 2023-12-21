#!/usr/bin/env python

"""
buffers.py: getbuffer() / putbuffer() demonstration.
This program is equivalent to 'buffers_numpy.py', but it does not use 
matrix and vector facilities provided by Numpy.

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

WIDTH  = 256
HEIGHT = 256
SIZE   = WIDTH * HEIGHT * sizeofint ()

"""
These are byte buffers, i.e. one-dimensional 'arrays'.
Function create_string_buffer() is provided by ctypes.
"""

red  = create_string_buffer (SIZE)
tmp  = create_string_buffer (SIZE)
hues = create_string_buffer (SIZE)

# -----

def bufferxy (buf, width, x, y, value):
    """
    Given a byte buffer 'buf 'simulating an array of 'width'
    columns, set (x, y) element as 'value'.
    """
    # bytes, from lowest to highest
    SOI = sizeofint ()
    vtb = value.to_bytes (4, 'little')
    for j in range (SOI):
        buf [y * width * SOI + x + j] = vtb[j]

# -----

# initialise the buffers
for y in range (HEIGHT):
    for x in range (0, WIDTH * sizeofint (), 4):
        """
        each value in 'x' must have room for a C integer, given by 
        sizeof (int): 4 bytes. Therefore, the real 'x' column is
        given by x // 4.
        """
        bufferxy (red, WIDTH, x, y, 0xffff0000)
        bufferxy (hues, WIDTH, x, y, colorRGB (x//4, y, abs ((x//4) - y)))

setwinoptions ("Using putbuffer()", -1, -1, -1)
initwindow (WIDTH, HEIGHT)
setcolor (BLACK)
setbkcolor (WHITE)
cleardevice ()
outtextxy (0, 0, "Click to continue:")
refresh ()
  
getclick ()
putbuffer (red)

getbuffer (tmp)  # tmp == red
setcolor (WHITE)
outtextxy (0, 0, "Left click to continue:")
refresh ()
getclick ()
  
for alpha in range (128):
  
    for n in range (0, SIZE, 4):  
        hues[n + 3] = 0x00 | alpha  # set color alpha
      
    putbuffer (tmp)   # red screen
    putbuffer (hues)  # hues + alpha on red
    refresh ()
    delay (10)
    
outtextxy (0, 0, "Left click to finish:")
refresh ()
getclick ()

closegraph ()
  
# ----- end of file buffers.py
