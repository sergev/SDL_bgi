#!/usr/bin/env python

"""
boo.py: shows how to make a window transparent and change some of its
properties.
  
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

try:
    from sdl2 import *
except:
    print ("Sorry, I need the sdl2 module.")
    exit ()

boo = "BOOO!"
stop = 0

# physical screen size, regardless of window dimensions
x, y = getscreensize ()

setwinoptions ("", SDL_WINDOWPOS_CENTERED,
		 SDL_WINDOWPOS_CENTERED, SDL_WINDOW_BORDERLESS)
initwindow (32 * len (boo), 32)  # 4 times the usual size
setbkcolor (BLACK)
cleardevice ()
winid = getcurrentwindow ()

# make the window opaque
SDL_SetWindowOpacity (bgi_window, 1.0)
setcolor (RED)
settextstyle (DEFAULT_FONT, HORIZ_DIR, 4)
outtextxy (0, 0, boo)
refresh ()

while (stop != 1):

    stop = edelay (random (1000))
    if (stop):
      break
  
    # make the window transparent (disappear)
    SDL_SetWindowOpacity (bgi_window, 0.0)
    refresh ()
    stop = edelay (random (1500))
  
    # opaque again - reappear
    resetwinoptions (winid, "", random (x), random (y))
    SDL_SetWindowOpacity (bgi_window, 1.0)
    refresh ()

closegraph ()

# ----- end of file boo.py
