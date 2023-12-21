#!/usr/bin/env python

"""
multiwin.py: shows how to open multiple windows.
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
gm = SDL_320x200

setwinoptions ("First Window", 10, 10, -1)
initgraph (gd, gm, "")
cleardevice ()
outtextxy (0, 0, "Click or press a key for a SECOND window")
circle (50, 100, 50)
win1 = getcurrentwindow ()
print ("Current window: %d" % win1)
x = getmaxx () - 110
y = getmaxy () - 60
readimagefile ("./logo.bmp", x, y, x + 100, y + 50)
getevent ()
delay (500)

setwinoptions ("Second Window", 10, 200 + 50, -1)
initgraph (gd, gm, "")
cleardevice ()
setcolor (RED)
outtextxy (0, 0, "Click or press a key for a THIRD window")
circle (100, 100, 50)
win2 = getcurrentwindow ()
print ("Current window: %d" % win2)
readimagefile ("./logo.bmp", x, y, x + 100, y + 50)
getevent ()
delay (500)

setwinoptions ("Third Window", 320 + 50, 10, -1)
initwindow (640, 480)
cleardevice ()
setcolor (GREEN)
outtextxy (0, 0, "Press a key to switch back to the FIRST window")
circle (150, 100, 50)
win3 = getcurrentwindow ()
print ("Current window: %d" % win3)
x = getmaxx () - 110
y = getmaxy () - 60
readimagefile ("./logo.bmp", x, y, x + 100, y + 50)
getevent ()
delay (500)

setcurrentwindow (win1)
setcolor (WHITE)
circle (150, 100, 50)
win1 = getcurrentwindow ()
print ("Current window: %d" % win1)
setcolor (YELLOW)
outtextxy (0, 10, "Back to the FIRST window press a key")
getevent ()
setcolor (RED)
outtextxy (0, 20, "Shutting down...")

setwintitle (win3, "Window 3 is shutting down...")
delay (2000)
print ("Shutting down window %d" % win3)
closewindow (win3)

setwintitle (win2, "Window 2 is shutting down...")
delay (2000)
print ("Shutting down window %d" % win2)
closewindow (win2)

setwintitle (win1, "Window 1 is shutting down...")
delay (2000)
print ("Shutting down window %d" % win1)
closewindow (win1)

# ----- end of file multiwin.py
