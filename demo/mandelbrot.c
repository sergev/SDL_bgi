/* mandelbrot.c  -*- C -*-
 * 
 * To compile:
 * gcc -o mandelbrot mandelbrot.c -lSDL_bgi -lSDL2
 * 
 * By Guido Gonzato, May 2015-2022
 * 
 * This is an unoptimised, simple but effective program for plotting
 * the Mandelbrot set. Left click to zoom in on a point, right click
 * to zoom out, middle click to restore the initial boundary,
 * ESC to quit.

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

int max_iter = 100; // max iteration
int maxx, maxy;

void mandelbrot (double, double, double, double);
void explain (void);
void amber_palette (void);
void blue_palette (void);
void purple_palette (void);

// -----

void mandelbrot (double x1, double y1, double x2, double y2)
{
  int xx, yy, counter;
  double dx, dy, x, y, a, b, tx, d;

  dy = (y2 - y1) / maxy;
  dx = dy;
  
  x = x1;
  for (xx = 0; xx < maxx; xx++) {
    
    y = y1;
    for (yy = 0; yy < maxy; yy++) {
      
      counter = 0;
      a = b = 0.0;
      
      // iteration: z(n+1) = z(n)^2 + c; z = ai + b; c = yi + x
      
      do {
	tx = a*a - b*b + x;
	b = 2*b*a + y;
	a = tx;
	d = a*a + b*b;
	counter++;
      } while (d <= 4.0 && counter < max_iter);
      
      setrgbcolor (counter);
      _putpixel (xx, yy);
      y += dy;
      
    } // y
    x += dx;
    
  } // x

} // mandelbrot ()

// -----

void amber_palette (void)
{
  int c;
  
  // initialize rgb palette entries 0 - (max_iter - 1)
  for (c = 0; c < max_iter; c++)
    setrgbpalette (c, max_iter - c, 50 + 2 * c, c);
  setrgbpalette (max_iter, 0x30, 0, 0x30); // the Mandelbrot set is purple

} // amber_palette ()

// -----

void purple_palette (void)
{
  int c;
  
  for (c = 0; c < max_iter; c++)
    setrgbpalette (c, 50 + 2 * c, c, max_iter - c);
  
  setrgbpalette (max_iter, 0, 0, 0); // the Mandelbrot set is black

} // purple_palette ()

// -----

void blue_palette (void)
{
  int c;
  
  for (c = 0; c < max_iter; c++)
    setrgbpalette (c, 0, c, 50 + 2 * c);
  setrgbpalette (max_iter, 0, 0, 0); // the Mandelbrot set is black

} // blue_palette ()

// -----

void help (void)
{
  showinfobox ("Press '1', '2', or '3' to change the palette\n"
	       "left click to zoom in on a point\n"
	       "right click to zoom out\n"
	       "middle click or 'R' to restore the initial boundary\n"
	       "'i' or '+', 'd' or '-' to increase/decrease max iterations\n"
	       "arrow keys to move around\n"
	       "'Q' to quit the program");
} // help ()

// -----

void explain (void)
{
  int
    i = 0,
    inc = 5,
    c;
  
  setbkcolor (COLOR (0, 0, 32)); // don't use a palette
  cleardevice ();
  setcolor (COLOR (255, 255, 0));

  settextstyle (TRIPLEX_FONT, HORIZ_DIR, 4);
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  c = 2*textheight ("H");
  outtextxy (maxx / 2, maxy / 2 - 4*c,
	   "Press '1', '2', or '3' to change the palette");
  outtextxy (maxx / 2, maxy / 2 - 3*c,
	   "left click to zoom in on a point");
  outtextxy (maxx / 2, maxy / 2 - 2*c,
	   "right click to zoom out");
  outtextxy (maxx / 2, maxy / 2 - c,
	   "middle click or 'R' to restore the initial boundary");
  outtextxy (maxx / 2, maxy / 2,
	   "'i' or '+', 'd' or '-' to increase/decrease max iterations");
  outtextxy (maxx / 2, maxy / 2 + c,
	   "arrow keys to move around");
  outtextxy (maxx / 2, maxy / 2 + 2*c,
	   "'H' to get help");
  
  outtextxy (maxx / 2, maxy / 2 + 3*c,
	   "'Q' to quit the program");
  
  int
     ev;

  while (1) {
  
    setcolor (COLOR (i, 0, 0));
    outtextxy (maxx / 2, maxy / 2 + 4*c, "PRESS A KEY OR CLICK TO BEGIN");
    i += inc;
    if (255 == i)
      inc = -5;
    if (0 == i)
      inc = 5;
    delay(1);
    refresh ();
    
    event ();
    ev = eventtype ();
    if (ev == SDL_KEYDOWN || ev == SDL_MOUSEBUTTONDOWN)
      break;
    
  }
  cleardevice ();
  
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 1);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  
} // explain ()

// -----

int main (int argc, char *argv[])
{
  int palette, ch, init, redraw, flag;
  double xm, ym, xstep, ystep, xmin, ymin, xmax, ymax;
  char s[20];
  
  initwindow (0, 0); // fullscreen
  
  maxx = getmaxx ();
  maxy = getmaxy ();
  // initial coordinates of the middle point of the screen
  xm = -1.2;
  ym = 0.0;
  // initial range: xm +- xstep, ym +- ystep 
  ystep = 1.2;
  xstep = ystep * maxx / (double) maxy;
  init = flag = redraw = 1;
  
  explain ();
  
  purple_palette ();
  palette = 1;
  
  ch = 'G';
  while (ch != 'q' && ch != 'Q' && ch != KEY_ESC) {
    
    xmin = xm - xstep;
    ymin = ym - ystep;
    xmax = xm + xstep;
    ymax = ym + ystep;

    if (redraw) {
      mandelbrot (xmin, ymin, xmax, ymax);
      refresh ();
      if (flag) {
	setcolor (WHITE);
	sprintf (s, "%d", max_iter);
	outtextxy (0, 0, s);
	flag = 0;
	refresh ();
      }
      redraw = 0;
    }
    
    ch = getevent (); // wait for a key, mouse click, or wheel motion
    
    switch (ch) {
      
    case 'h':
    case 'H':
      help ();
      break;
      
    case WM_LBUTTONDOWN:
    case WM_WHEELUP:
      xm = xmin + 2 * xstep * mousex () / maxx;
      ym = ymin + 2 * ystep * mousey () / maxy;
      xstep /= 2.0;
      ystep /= 2.0;
      init = 0;
      redraw = 1;
      break;

    case WM_RBUTTONDOWN:
    case WM_WHEELDOWN:
      xstep *= 2.0;
      ystep *= 2.0;
      init = 0;
      redraw = 1;
      break;
      
    case WM_MBUTTONDOWN:
    case 'r':
    case 'R':
      if (0 == init) {
        xm = -1.2;
        ym = 0.0;
        ystep = 1.2;
        xstep = ystep * (double) maxx / (double) maxy;
	redraw = 1;
	max_iter = 100;
	purple_palette ();
      }
      break;
      
    case '1':
      purple_palette ();
      if (1 != palette) {
	redraw = 1;
	palette = 1;
      }
      break;
      
    case '2':
      blue_palette ();
      if (2 != palette) {
	redraw = 1;
	palette = 2;
      }
      break;
      
    case '3':
      amber_palette ();
      if (3 != palette) {
	redraw = 1;
	palette = 3;
      }
      break;
    
    case 'i':
    case '+':
      max_iter += 50;
      if (max_iter > getrgbpalettesize ())
	max_iter = getrgbpalettesize ();
      flag = redraw = 1;
      purple_palette ();
      blue_palette ();
      amber_palette ();
      break;
    
    case 'd':
    case '-':
      max_iter -= 50;
      if (max_iter < 50)
	max_iter = 50;
      flag = redraw = 1;
      purple_palette ();
      blue_palette ();
      amber_palette ();
      break;

    case KEY_LEFT:
      xm -= (xmax - xmin) / 20;
      redraw = 1;
      break;

    case KEY_RIGHT:
      xm += (xmax - xmin) / 20;
      redraw = 1;
      break;

    case KEY_UP:
      ym -= (ymax - ymin) / 20;
      redraw = 1;
      break;

    case KEY_DOWN:
      ym += (ymax - ymin) / 20;
      redraw = 1;
      break;
      
    default:
      redraw = 0;
    }
    
  } // while

  closegraph ();
  return 0;
  
} // main(void) ()

// ----- end of file mandelbrot.c
