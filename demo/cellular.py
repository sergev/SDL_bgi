#!/usr/bin/env python3

"""
cellular.py: simple cellular automata generator, as described at
http://mathworld.wolfram.com/ElementaryCellularAutomaton.html

Run this program as:
 
./cellular.py [rule]

where [rule] is a cellular automaton rule (1..255).

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
from sys     import argv

# -----

def run_rule (rule):

    maxx = getmaxx ()
  
    # for an explanation about rules, please see
    # http:#mathworld.wolfram.com/CellularAutomaton.html
    
    # loop: for every pixel, run the rule
    for y in range (20, int (20 + maxx / 2), 1):
        for x in range (1,  maxx - 1, 1):
          
            pixel = 0
            
            # check the neighbour pixels on the line above
            # and take note of the ones that are RED
            if (RED == getpixel (x - 1, y - 1)):
                pixel |= 4   # same as pixel += 4
            if (RED == getpixel (x, y - 1)):
                pixel |= 2
            if (RED == getpixel (x + 1, y - 1)):
                pixel |= 1
            
            # for every possible state (3 bits: 0-7), if the
            # corresponding bit in the rule is 1, then
            # plot the pixel at (x, y)
            
            for i in range (8):
                # the bit in the rule is set to 1
                if ((rule << i) & 0x80):
                    # 'pixel' matches the rule
                    if (pixel == 7 - i):
                        putpixel (x, y, RED)

# -----

def main (args):
    
    if (len (args) == 1):
        rule = 126  # nice Sierpinsky gasket
    else:
        rule = int (argv[1])
    

    # set window title and position
    setwinoptions ("Cellular Automata", 100, 100, -1)
    initwindow (1024, 1024 / 2 + 40)
    setbkcolor (WHITE)
    setcolor (RED)
    cleardevice ()
    
    stop = False
    
    while (stop != True):
    
        s = "Click to continue - Rule: %d" % rule
        outtextxy (0, 0, s)
        putpixel (getmaxx () / 2, 20, RED)  # set initial pixel
        run_rule (rule)
        refresh ()
        # srand (time(NULL))
        rule = random (255)
      
        ev = getevent()
        if (KEY_ESC == ev or WM_RBUTTONDOWN == ev or QUIT == ev):
            stop = True
        
        cleardevice ()
    
    closegraph ()

# -----

if __name__ == '__main__':
  main (argv)
    
# ----- end of file cellular.py
