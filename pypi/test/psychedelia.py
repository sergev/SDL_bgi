#!/usr/bin/env python

"""
psychedelia.py: sets up an RGB palette and produces a plasma effect.

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
from struct  import *

s_int  = sizeofint ()
WIDTH  = 800
HEIGHT = 600
SIZE   = WIDTH * HEIGHT * s_int

# -----

def bufferxy (buffer, x, y, r, g, b):
    """
    Inserts integer 'value' at (x, y) in buffer.
    """
    buffer [y * WIDTH * s_int + x]     = b
    buffer [y * WIDTH * s_int + x + 1] = g
    buffer [y * WIDTH * s_int + x + 2] = r
    buffer [y * WIDTH * s_int + x + 3] = 0xff
    
# -----

def main ():

    use_buffer = True
    counter = 0

    # screen buffer. NOTE: height x width!
    buffer = create_string_buffer (SIZE)

    k1 = 128
    k2 = 32
    k3 = 64
    d1 = -1
    d2 = 1
    d3 = 1

    initwindow (WIDTH, HEIGHT)
    setbkcolor (BLACK)
    refresh ()

    # make a palette
    for i in range (255):
        setrgbpalette (i,
                       abs ((128. - 127.*sin (i*pi/32.))),
                       abs ((128. - 127.*sin (i*pi/64.))),
                       abs ((128. - 127.*sin (i*pi/128.))) )

    showinfobox ("Left click to switch between\n"
                 "putbuffer() and putpixel()\n"
                 "right click to stop.")

    # time = SDL_GetTicks ()

    while ismouseclick (WM_RBUTTONDOWN) == False:

        for y in range (HEIGHT):
            for x in range (0, WIDTH * s_int, 4):

                col = sin ((x // 4) / 50.) * k1 \
                    + sin (y / 40.) * k2 \
                    + sin (((x // 4) + y) / 30.) * k3

                while col > 255:
                    col -= 256
                while col < 0:
                    col += 256

                r = RED_VALUE (col)
                g = GREEN_VALUE (col)
                b = BLUE_VALUE (col)
                
                # this would be slow:
                # setrgbpalette (col, r, g, b)
                # setrgbcolor (col)
                # _putpixel (x, y)
                
                if use_buffer == True:
                    bufferxy (buffer, x, y, r, g, b)
                else:
                    putpixel (x, y, COLOR(r, g, b))

        if use_buffer == True:
            putbuffer (buffer)
        refresh ()
        counter += 1

        """
        if (SDL_GetTicks () >= time + 1000) {
          printf ("%d iterations/second\n", counter)
          time = SDL_GetTicks ()
          counter = 0
        }
        """

        # change the parameters
        k1 += d1
        if k1 < 2 or k1 > 255:
            d1 *= -1
        k2 += d2
        if k2 < 2 or k2 > 255:
            d2 *= -1
        k3 += d3
        if k3 < 2 or k3 > 255:
            d3 *= -1
    
        if WM_LBUTTONDOWN == mouseclick():
            if use_buffer == True:
                use_buffer == False
            else:
                use_buffer == True
            if use_buffer == True:
                print ("Using putbuffer()")
            else:
                print ("Using putpixel()")

    print ("Bye!")
    closegraph ()

# -----

if __name__ == '__main__':
  main ()

# ----- end of file psychedelia.py
