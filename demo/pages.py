#!/usr/bin/env python3

"""
pages.py: using active and visual pages.

By Guido Gonzato, November 2022.

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

initwindow (300, 200);

settextstyle (BOLD_FONT, HORIZ_DIR, 4)
settextjustify (CENTER_TEXT, CENTER_TEXT)

setactivepage (0)
setbkcolor (BLACK)
setcolor (YELLOW)
outtextxy (150, 60, "PAGE")
outtextxy (150, 120, str (0))

setactivepage (1)
setbkcolor (BLUE)
setcolor (WHITE)
outtextxy (150, 60, "PAGE")
outtextxy (150, 120, str (1))

setactivepage (2)
setbkcolor (RED)
setcolor (BLUE)
outtextxy (150, 60, "PAGE")
outtextxy (150, 120, str (2))

setactivepage (3)
setbkcolor (GREEN)
setcolor (MAGENTA)
outtextxy (150, 60, "PAGE")
outtextxy (150, 120, str (3))

page = 0
stop = False
while stop == False:
    setvisualpage (page);
    if kdelay (500) == True:
        stop = True
    page += 1
    if page == 4:
        page = 0

closegraph ()
