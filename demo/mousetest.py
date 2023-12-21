#!/usr/bin/env python3

"""
mousetest.py: shows how to detect and use mouse clicks.
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

s = " Click around (click here to exit) "
  
initgraph (0, 0, "")
setbkcolor (BLACK)
setcolor (RED)
setviewport (20, 20, getmaxx() - 20, getmaxy() - 20, 1)
clearviewport ()
rectangle (0, 0, getmaxx () - 40, getmaxy () - 40)

len = len(s) * 8  # assumes an 8x8 font
rectangle (0, 0, len, 16)
settextjustify (LEFT_TEXT, TOP_TEXT)
outtextxy (0, 4, s)
isize = imagesize (0, 0, len, 16)
image = create_string_buffer (isize)
getimage (0, 0, len, 16, image)
setlinestyle (SOLID_LINE, 0, THICK_WIDTH)
  
while (True):
  
    # we use mouseclick () because we want to check for
    # all mouse events; ismouseclick () is much faster
    mclick = mouseclick ()
      
    if 0 == mclick:
        continue
      
    # top left box
    if mousex () < len and mousey () < 16 and WM_LBUTTONDOWN == mclick:
        break
  
    if mclick == WM_MOUSEMOVE:
        putpixel (mousex (), mousey (), getcolor ())
        
    if mclick == WM_LBUTTONDBLCLK or \
       mclick == WM_MBUTTONDBLCLK or \
       mclick == WM_RBUTTONDBLCLK:
        tmp = 10 + random (40)
        setcolor (COLOR (random(255), random(255), random(255)))
        line (mousex () - tmp, mousey () - tmp, \
              mousex () + tmp, mousey () + tmp)
        line (mousex () + tmp, mousey () - tmp, \
              mousex () - tmp, mousey () + tmp)
        
    if mclick == WM_LBUTTONUP or \
       mclick == WM_MBUTTONUP or \
       mclick == WM_RBUTTONUP:
        tmp = 10 + random (40)
        setcolor (COLOR (random(255), random(255), random(255)))
        line (mousex (), mousey () - tmp, \
              mousex (), mousey () + tmp)
        line (mousex () - tmp, mousey (), \
              mousex () + tmp, mousey ())
      
    if mclick == WM_LBUTTONDOWN or \
         mclick == WM_MBUTTONDOWN or \
         mclick == WM_RBUTTONDOWN or \
         mclick == WM_WHEELUP or \
         mclick == WM_WHEELDOWN:
        setfillstyle (1, COLOR (random(255), random(255), random(255)))
        setcolor (COLOR (random(255), random(255), random(255)))
        if 0 == random (2):
            fillellipse (mousex (), mousey (), \
                         10 + mclick*random(40), \
                         10 + mclick*random(40))
        else:
            bar (mousex () - mclick*10, mousey () - mclick*10, \
                 mousex () + mclick*10, mousey () + mclick*10)
        
    putimage (0, 0, image, COPY_PUT)
  
closegraph ()

# ----- end of file mousetest.c
