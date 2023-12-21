#!/usr/bin/env python3

"""
linebuffers.py: getlinebuffer() / putlinebuffer() demonstration.
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

# this program is horribly slow, and Numba will not work.
# memmove() won't work as expected. Duh.

# -----

WIDTH  = 256
HEIGHT = 256
SOI    = sizeofint ()
SIZE   = WIDTH * SOI

hues = empty ((WIDTH, HEIGHT), dtype=int64)
linebuffer = create_string_buffer (SIZE)

# -----

def shiftbuffer (buffer):
    """
    scroll line elements 1 position to the left;
    memmove () not applicable. Buffer must be populated
    with buffer[i] = n.to_bytes (1, 'little')
    """
    
    tmp = create_string_buffer (SOI)
    bl = len (buffer)
    
    # copy the last integer from buffer to tmp
    for i in range (SOI):
        tmp[i] = buffer [bl - SOI + i]
    # shift the buffer four times
    for x in range (SOI):
        for i in range (bl - 2, -1, -1):
            buffer [i + 1] = buffer [i]
    
    # restore first element
    for i in range (SOI):
        buffer [i] = tmp [i]
  
# -----

# initialise the buffer
for y in range (HEIGHT):
    for x in range (WIDTH):
        hues [x, y] = colorRGB (x, y, abs (x - y))
  
setwinoptions ("Using putlinebuffer()", -1, -1, -1)
initwindow (WIDTH, HEIGHT)
refresh ()
showinfobox ("Left click to switch between\n" +
             "putlinebuffer() and putpixel()\n" +
	     "right click to stop.\n" + 
             "Warning: *very* slow!")
  
b_hues = vec2buf (reshape (hues, -1))
putbuffer (b_hues)
refresh ()
mode = 1

while (False == ismouseclick (WM_RBUTTONDOWN)):

    if 1 == mode:
         for y in range (HEIGHT):
             getlinebuffer (y, linebuffer)
             shiftbuffer (linebuffer)
             putlinebuffer (y, linebuffer)
    """
    else:
      for y in range (HEIGHT):
	for x in range (WIDTH):
	  linebuffer[x] = getpixel (x, y)
	shiftbuffer (linebuffer)
	for x in range (WIDTH):
	  putpixel (x, y, COLOR32(linebuffer[x]))
    """
    
    if WM_LBUTTONDOWN == mouseclick ():
        mode ^= 1
        if 1 == mode:
            print ("using getlinebuffer()/putlinebuffer()")
        else:
            print ("using getpixel()/putpixel()")
    
    delay (5)
    refresh ()
  
puts ("Bye!")
closegraph()

# ----- end of file linebuffers.py
