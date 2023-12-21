#!/usr/bin/env python

"""
rgbpalette.py: Shows how to save, redefine, and restore colours in the
ARGB palette.

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

def main ():

    MAX = 100
  
    palette = rgbpalettetype ()
    # you must allocate space for colors
    col = create_string_buffer (MAX * sizeofint ())
    palette.colors = addressof (col)
    palette.size = MAX
    
    initwindow (500, 250)
    sdlbgislow ()
    setbkcolor (BLACK)
    cleardevice ()
    
    # set MAX hues of blue in the RGB palette
    for i in range (MAX):
        setrgbpalette (i, 0, 0, 10 + i*2)
    
    # show the colours
    setcolor (WHITE)
    outtextxy (250, 100, "Here are 100 shades of blue")
    for i in range (MAX):
        setrgbcolor (i)
        setfillstyle (SOLID_FILL, RGBPALETTE (i))
        bar (10, 10, 200, 200)
        delay (20)
    
    # save the ARGB palette
    setcolor (WHITE)
    outtextxy (250, 120, "Saving the ARGB palette...")
    delay (2000)
    getrgbpalette (palette, MAX)
    cleardevice ()
    
    # reset ARGB colours to gray
    for i in range (MAX):
        setrgbpalette (i, 128, 128, 128)
  
    setcolor (WHITE)
    outtextxy (250, 100, "Grayed out ARGB colours...")
    for i in range (MAX):
        setrgbcolor (i)
        setfillstyle (SOLID_FILL, RGBPALETTE (i))
        bar (10, 10, 200, 200)
        delay (20)
  
    # restore the ARGB palette
    setcolor (WHITE)
    outtextxy (250, 120, "Restoring the ARGB palette...")
    delay (2000)
    setallrgbpalette (palette)
    cleardevice ()
    
    setcolor (WHITE)
    outtextxy (250, 100, "Restored ARGB colours")
    for i in range (MAX):
        setrgbcolor (i)
        setfillstyle (SOLID_FILL, RGBPALETTE (i))
        bar (10, 10, 200, 200)
        delay (20)
  
    setcolor (WHITE)
    outtextxy (250, 120, "Press a key to exit")
    
    getch ()
    closegraph ()

# -----

main ()
  
# ----- end of file rgbpalette.c
