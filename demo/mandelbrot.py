#!/usr/bin/env python3

"""
mandelbrot.py: This is an unoptimised, simple but effective program
for plotting the Mandelbrot set. Left click to zoom in on a point,
right click to zoom out, middle click to restore the initial boundary,
ESC to quit.
If the Numba compiler is available, the program is *much* faster.

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
    from numba import jit
    NUMBA_FOUND = True
except ModuleNotFoundError or ImportError:
    NUMBA_FOUND = False

max_iter = 100  # max iteration
maxx, maxy = getscreensize ()

# -----

if NUMBA_FOUND == False:
    print ("Numba not found - program will be slow.")
    def jit (*args, **kwargs):  # define dummy decorator
        def dummy (func):
            return func
        return dummy

# -----       

@jit(nopython3=True)
def iterate (a, b, d, x, y):
    counter = 0

    while d <= 4.0 and counter < max_iter:
        tx = a*a - b*b + x
        b = 2*b*a + y
        a = tx
        d = a*a + b*b
        counter += 1

    return counter

# -----

def mandelbrot (x1, y1, x2, y2):

    dy = (y2 - y1) / maxy
    dx = dy
    
    x = x1
    for xx in range (maxx):
      
        y = y1
        for yy in range (maxy):
          
            counter = 0
            a = 0.0
            b = 0.0
            d = 0
            
            # iteration: z(n+1) = z(n)^2 + c z = ai + b c = yi + x
            
            counter = iterate (a, b, d, x, y)
          
            setrgbcolor (counter)
            fputpixel (xx, yy)
            y += dy
          
        x += dx
    
# -----

def amber_palette ():
    global max_iter
  
  # initialize rgb palette entries 0 - (max_iter - 1)
    for c in range (max_iter):
        setrgbpalette (c, max_iter - c, 50 + 2 * c, c)
  
    setrgbpalette (max_iter, 0x30, 0, 0x30) # the Mandelbrot set is purple

# -----

def purple_palette ():
    global max_iter
    
    for c in range (max_iter):
        setrgbpalette (c, 50 + 2 * c, c, max_iter - c)
    
    setrgbpalette (max_iter, 0, 0, 0) # the Mandelbrot set is black
  
# -----

def blue_palette ():
    global max_iter
  
    for c in range (max_iter):
      setrgbpalette (c, 0, c, 50 + 2 * c)
    setrgbpalette (max_iter, 0, 0, 0) # the Mandelbrot set is black

# -----

def help ():

    showinfobox ("Press '1', '2', or '3' to change the palette\n"
                 "left click to zoom in on a point\n"
                 "right click to zoom out\n"
                 "middle click or 'R' to restore the initial boundary\n"
                 "'i' or '+', 'd' or '-' to increase/decrease max iterations\n"
                 "arrow keys to move around\n"
                 "'Q' to quit the program")
# -----

def explain ():

    i = 330
    inc = 5
    
    setbkcolor (COLOR (0, 0, 32)) # don't use a palette
    cleardevice ()
    setcolor (COLOR (255, 255, 0))
  
    settextstyle (TRIPLEX_FONT, HORIZ_DIR, 4)
    settextjustify (CENTER_TEXT, CENTER_TEXT)
    c = 2*textheight ("H")
    outtextxy (maxx / 2, maxy / 2 - 4*c,
             "Press '1', '2', or '3' to change the palette")
    outtextxy (maxx / 2, maxy / 2 - 3*c,
             "left click to zoom in on a point")
    outtextxy (maxx / 2, maxy / 2 - 2*c,
             "right click to zoom out")
    outtextxy (maxx / 2, maxy / 2 - c,
             "middle click or 'R' to restore the initial boundary")
    outtextxy (maxx / 2, maxy / 2,
             "'i' or '+', 'd' or '-' to increase/decrease max iterations")
    outtextxy (maxx / 2, maxy / 2 + c,
             "arrow keys to move around")
    outtextxy (maxx / 2, maxy / 2 + 2*c,
             "'H' to get help")
    
    outtextxy (maxx / 2, maxy / 2 + 3*c,
             "'Q' to quit the program")
    
    while (True):
    
        setcolor (COLOR (i, 0, 0))
        outtextxy (maxx / 2, maxy / 2 + 4*c, "PRESS A KEY OR CLICK TO BEGIN")
        i += inc
        if (255 == i):
            inc = -5
        if (0 == i):
            inc = 5
        delay(1)
        refresh ()
        
        event ()
        ev = eventtype ()
        if ev == SDL_KEYDOWN or ev == SDL_MOUSEBUTTONDOWN:
            break
      
    cleardevice ()
    
    settextstyle (DEFAULT_FONT, HORIZ_DIR, 1)
    settextjustify (LEFT_TEXT, TOP_TEXT)
  
# -----

def main():
  
    initwindow (1024, 768)
    
    global maxx, maxy, max_iter
    
    maxx = getmaxx ()
    maxy = getmaxy ()
    # initial coordinates of the middle point of the screen
    xm = -1.2
    ym = 0.0
    # initial range: xm +- xstep, ym +- ystep 
    ystep = 1.2
    xstep = ystep * maxx / maxy
    init = True
    flag = True
    redraw = True
    
    explain ()
    
    purple_palette ()
    palette = 1
    
    ch = 'G'
    stop = False
    
    while (stop == False):
      
        xmin = xm - xstep
        ymin = ym - ystep
        xmax = xm + xstep
        ymax = ym + ystep
    
        if redraw == True:
            mandelbrot (xmin, ymin, xmax, ymax)
            refresh ()
            if (flag == True):
                setcolor (WHITE)
                s = "%d" % max_iter
                outtextxy (0, 0, s)
                flag = False
                refresh ()
            redraw = False
        
        ch = getevent () # wait for a key, mouse click, or wheel motion
        
        if ch == ord ('h') or ch == ord ('H'):
            help ()
          
        if ch == WM_LBUTTONDOWN or ch == WM_WHEELUP:
            xm = xmin + 2 * xstep * mousex () / maxx
            ym = ymin + 2 * ystep * mousey () / maxy
            xstep /= 2.0
            ystep /= 2.0
            init = False
            redraw = True
    
        if ch == WM_RBUTTONDOWN or ch == WM_WHEELDOWN:
            xstep *= 2.0
            ystep *= 2.0
            init = False
            redraw = True
          
        if ch == WM_MBUTTONDOWN or ch == ord ('r') or ch == ord ('R'):
            if (0 == init):
                xm = -1.2
                ym = 0.0
                ystep = 1.2
                xstep = ystep * maxx / maxy
                redraw = True
                max_iter = 100
                purple_palette ()
          
        if ch == ord ('1'):
            purple_palette ()
            if 1 != palette:
                redraw = True
                palette = True
          
        if ch == ord ('2'):
            blue_palette ()
            if 2 != palette:
                redraw = True
                palette = 2
          
        if ch == ord ('3'):
            amber_palette ()
            if 3 != palette:
                redraw = True
                palette = 3
        
        if ch == ord ('i') or ch == ord ('+'):
            max_iter += 50
            if max_iter > getrgbpalettesize ():
                max_iter = getrgbpalettesize ()
            flag = True
            redraw = True
            purple_palette ()
            blue_palette ()
            amber_palette ()
        
        if ch == ord ('d') or ch == ord ('-'):
            max_iter -= 50
            if max_iter < 50:
              max_iter = 50
            flag = True
            redraw = True
            purple_palette ()
            blue_palette ()
            amber_palette ()
    
        if ch == KEY_LEFT:
            xm -= (xmax - xmin) / 20
            redraw = True
    
        if ch == KEY_RIGHT:
            xm += (xmax - xmin) / 20
            redraw = True
    
        if ch == KEY_UP:
            ym -= (ymax - ymin) / 20
            redraw = True
    
        if ch == KEY_DOWN:
            ym += (ymax - ymin) / 20
            redraw = True
        
        if ch == ord ('q') or ch == ord ('Q') or ch == KEY_ESC:
            stop = True
    
        # !!! redraw = 0
  
    closegraph ()
    return 0

# -----

if __name__ == '__main__':
    main ()


# ----- end of file mandelbrot.c
