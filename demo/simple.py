#!/usr/bin/env python3

"""
simple.py: displays simple shapes in 'slow' and 'fast' mode.
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

# -----

def message (str):

    cleardevice ()
    settextstyle (DEFAULT_FONT, HORIZ_DIR, 2)
    settextjustify (CENTER_TEXT, CENTER_TEXT)
    setcolor (RED)
    outtextxy (maxx / 2, maxy / 2, str)
    settextstyle (DEFAULT_FONT, HORIZ_DIR, 1)
    setcolor (YELLOW)
    outtextxy (maxx / 2, maxy - 20, "Press a key to continue")
    getevent ()
    cleardevice ()
    settextjustify (LEFT_TEXT, TOP_TEXT)

# -----

def circles ():

      stop = 0
    
      while (stop != 1):
          setcolor (YELLOW)
          outtextxy (0, 0, "Press a key to continue")
          for i in range (300):
              setcolor (BLACK + random (MAXCOLORS))
              circle  (random (maxx), random(maxy), random (100))
          refresh ()
          stop = kdelay (1000)
          cleardevice ()

# ---

def lines ():

    stop = 0
    
    while (stop != 1):
        setcolor (YELLOW)
        outtextxy (0, 0, "Press a key to continue")
        for i in range (500):
            setlinestyle (SOLID_LINE, 0, NORM_WIDTH)
            setcolor (BLACK + random (MAXCOLORS))
            line (random(maxx), random(maxy), random(maxx), random(maxy))
        refresh ()
        stop = kdelay (1000)
        cleardevice ()

# -----

def pixels ():

    stop = 0
    
    while (stop != 1):
        setcolor (YELLOW)
        outtextxy (0, 0, "Press a key to continue")
        # srand (2015)
        for i in range (300):
            col = BLACK + random (MAXCOLORS)
            putpixel (random(maxx), random(maxy), col)
        refresh ()
        stop = kdelay (1000)
        if (stop):
            break
        # srand (2015)
        for i in range (300):
            col = 1 + random (MAXCOLORS)
            putpixel (random(maxx), random(maxy), BLACK)
        refresh ()
        stop = kdelay (1000)
        cleardevice ()

# -----

initgraph (0, 0, "")
maxx = getmaxx ()
maxy = getmaxy ()
setbkcolor (BLACK)
cleardevice ()

message ("Drawing in SLOW mode:")
sdlbgislow ()

circles ()
lines ()
pixels ()

message ("Drawing in FAST mode:")
sdlbgifast ()

circles ()
lines ()
pixels ()

closegraph ()
  
# ----- end of file simple.py
