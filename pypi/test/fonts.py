#!/usr/bin/env python

"""
fonts.py: fonts demonstration.
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

def message (x, y, str):
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 0)
  settextjustify (LEFT_TEXT, TOP_TEXT)
  setusercharsize (1, 1, 2, 1)
  setcolor (RED)
  outtextxy (x, y, str)
  setcolor (BLUE)
  setusercharsize (1, 1, 1, 1)

# -----

pos_y = 5
  
str_AZ  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
str_sym = "!\"#$%&'()*+,-./0123456789:<=>?@"
str_az  = "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
  
gd = DETECT
gm = VGA
  
initgraph (gd, gm, "")
setcolor (BLUE)
setbkcolor (WHITE)
cleardevice ()
  
message (0, pos_y, "TRIPLEX_FONT, default size")
settextstyle (TRIPLEX_FONT, HORIZ_DIR, 0)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 20
  
message (0, pos_y, "SMALL_FONT, default size")
settextstyle (SMALL_FONT, HORIZ_DIR, 0)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 20

message (0, pos_y, "SANS_SERIF_FONT, default size")
settextstyle (SANS_SERIF_FONT, HORIZ_DIR, 0)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 20

message (0, pos_y, "GOTHIC_FONT, default size")
settextstyle (GOTHIC_FONT, HORIZ_DIR, 0)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)

getevent ()
cleardevice ()

pos_y = 5
message (0, pos_y, "SCRIPT_FONT, default size")
settextstyle (SCRIPT_FONT, HORIZ_DIR, 0)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 20

message (0, pos_y, "SIMPLEX_FONT, default size")
settextstyle (SIMPLEX_FONT, HORIZ_DIR, 0)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 20

message (0, pos_y, "TRIPLEX_SCR_FONT, default size")
settextstyle (TRIPLEX_SCR_FONT, HORIZ_DIR, 0)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 20

getevent ()
cleardevice ()

pos_y = 5
message (0, pos_y, "COMPLEX_FONT, default size")
settextstyle (COMPLEX_FONT, HORIZ_DIR, 0)
pos_y += 15
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 15

message (0, pos_y, "EUROPEAN_FONT, default size")
settextstyle (EUROPEAN_FONT, HORIZ_DIR, 0)
pos_y += 15
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 15

message (0, pos_y, "BOLD_FONT, default size")
settextstyle (BOLD_FONT, HORIZ_DIR, 0)
pos_y += 15
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip

getevent ()
cleardevice ()

settextstyle (TRIPLEX_FONT, VERT_DIR, 0)
# setusercharsize (2, 1, 2, 1)
settextjustify (CENTER_TEXT, CENTER_TEXT)
outtextxy (getmaxx () * 0.25, getmaxy () / 2,
	"This is VERTICAL TEXT")

settextstyle (SANS_SERIF_FONT, VERT_DIR, 0)
# setusercharsize (2, 1, 2, 1)
settextjustify (CENTER_TEXT, CENTER_TEXT)
outtextxy (getmaxx () * 0.5, getmaxy () / 2,
	"This is VERTICAL TEXT")

settextstyle (GOTHIC_FONT, VERT_DIR, 0)
# setusercharsize (2, 1, 2, 1)
settextjustify (CENTER_TEXT, CENTER_TEXT)
outtextxy (getmaxx () * 0.75, getmaxy () / 2,
	"This is VERTICAL TEXT")

getevent ()
cleardevice ()

pos_y = 5
message (0, pos_y, "SMALL_FONT, size 1")
settextstyle (SMALL_FONT, HORIZ_DIR, 0)
setusercharsize (1, 1, 1, 1)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 20

message (0, pos_y, "SMALL_FONT, size 3")
settextstyle (SMALL_FONT, HORIZ_DIR, 0)
setusercharsize (3, 1, 3, 1)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)
pos_y += v_skip + 20

message (0, pos_y, "SMALL_FONT, size 4 x 8")
settextstyle (SMALL_FONT, HORIZ_DIR, 0)
setusercharsize (4, 1, 8, 1)
pos_y += 20
v_skip = textheight ("X")
outtextxy (10, pos_y, str_sym)
pos_y += v_skip
outtextxy (10, pos_y, str_AZ)
pos_y += v_skip
outtextxy (10, pos_y, str_az)

getevent ()
closegraph ()

# ----- end of file fonts.py
