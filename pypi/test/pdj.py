#!/usr/bin/env python

"""
pdj.py: plots Peter de Jong attractors.
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
from math    import *

def create_palette ():

    # blue fading to white
    for x in range (256):
        setrgbpalette (x, x, x, 255) # end at 255,255,255
  
    # white fading to red
    for x in range (256):
        setrgbpalette (x + 256, 255, 255 - x, 255 - x) # end at 255,0,0
    
    # red fading to purple
    for x in range (256):
        setrgbpalette (x + 512, 255, 0, x) # end at 255,0,255
    
    # purple fading to blue
    for x in range (256):
        setrgbpalette (x + 768, 255 - x, 0, 255) # end at 0,0,255

# -----

a = 1.641
b = 1.902
c = 0.316
d = 1.525
x = 0.0
y = 0.0

stop = False
cnt = 0

initwindow (800, 600)
create_palette ()
xm = getmaxx () / 2
ym = getmaxy () / 2
ly = ym / 2
# srand (time (NULL))
print ("a = %7.4f, b = %7.4f, c = %7.4f, d = %7.4f" %
	(a, b, c, d))

while (stop != True):

    xx = sin (a * y) - cos (b * x)
    yy = sin (c * x) - cos (d * y)
  
    color = cnt / 16384
    if color > 1024:
      cnt = 0
      color = 0
  
    setrgbcolor (color)
    fputpixel (xm + ly * xx, ym + ly * yy)
    x = xx
    y = yy
  
    cnt += 1
    if (0 == cnt % 5000):
  
        refresh ()
    
        if (WM_LBUTTONDOWN == mouseclick ()):
          a = -4.0 + 8 * random (10000) / 10000.0
          b = -4.0 + 8 * random (10000) / 10000.0
          c = -4.0 + 8 * random (10000) / 10000.0
          d = -4.0 + 8 * random (10000) / 10000.0
          print ("a = %7.4f, b = %7.4f, c = %7.4f, d = %7.4f" %
                 (a, b, c, d))
          cleardevice ()
    
        if kbhit ():
          stop = True
          break

getch ()
closegraph ()

# ----- end of file pdj.py
