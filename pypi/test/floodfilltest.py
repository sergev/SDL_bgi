#!/usr/bin/env python

"""
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

stop = False

initwindow (800, 600)
setbkcolor (BLACK)
setcolor (RED)
cleardevice ()
settextjustify (CENTER_TEXT, CENTER_TEXT)

outtextxy (getmaxx() / 2, getmaxy () / 2 - 15, 
	 "Left click to draw a random circle")
outtextxy (getmaxx() / 2, getmaxy () / 2, 
	 "right click to fill")
outtextxy (getmaxx() / 2, getmaxy () / 2 + 15, 
	 "press a key to exit.")

getevent ()

cleardevice ()
setcolor (YELLOW)
refresh ()

while (stop != True):

    mclick = getevent ()
  
    if (WM_LBUTTONDOWN == mclick):
        setcolor (YELLOW)
        circle (mousex (), mousey (), 10 + random (100))
        refresh ()
  
    if (WM_RBUTTONDOWN == mclick):
        setcolor (random (MAXCOLORS))
        setfillstyle (1 + random(USER_FILL),
                      COLOR (random (255), random (255), random (255)))
        floodfill (mousex (), mousey (), YELLOW)
        refresh ()
  
    if (KEY_ESC == mclick):
        stop = True

closegraph ()

# ----- end of file floodfilltest.py
