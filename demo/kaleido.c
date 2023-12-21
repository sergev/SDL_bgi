/* kaleido.c  -*- C -*-
 * 
 * To compile:
 * gcc -o kaleido kaleido.c -lSDL_bgi -lSDL2
 *
 * A nice kaleidoscope.
 * By Guido Gonzato, December 2018
 * 
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

#include <graphics.h>
#include <stdlib.h>
#include <time.h>

int xc, yc;

void rnd_circles (int x, int y, int r)
{
  fillellipse (xc + x, yc + y, r, r);
  fillellipse (xc - x, yc + y, r, r);
  fillellipse (xc - x, yc - y, r, r);
  fillellipse (xc + x, yc - y, r, r);
  fillellipse (xc + y, yc + x, r, r);
  fillellipse (xc - y, yc + x, r, r);
  fillellipse (xc - y, yc - x, r, r);
  fillellipse (xc + y, yc - x, r, r);
  // outlines
  setcolor (COLOR (random (256), random (256), random (256)));
  ellipse (xc + x, yc + y, 0, 360, r, r);
  ellipse (xc - x, yc + y, 0, 360, r, r);
  ellipse (xc - x, yc - y, 0, 360, r, r);
  ellipse (xc + x, yc - y, 0, 360, r, r);
  ellipse (xc + y, yc + x, 0, 360, r, r);
  ellipse (xc - y, yc + x, 0, 360, r, r);
  ellipse (xc - y, yc - x, 0, 360, r, r);
  ellipse (xc + y, yc - x, 0, 360, r, r);
}

void rnd_bars (int x, int y, int r)
{
  bar (xc + x - r/2, yc + y - r/2, xc + x + r/2, yc + y + r/2);
  bar (xc - x - r/2, yc + y - r/2, xc - x + r/2, yc + y + r/2);
  bar (xc - x - r/2, yc - y - r/2, xc - x + r/2, yc - y + r/2);
  bar (xc + x - r/2, yc - y - r/2, xc + x + r/2, yc - y + r/2);
  bar (xc + y - r/2, yc + x - r/2, xc + y + r/2, yc + x + r/2);
  bar (xc - y - r/2, yc + x - r/2, xc - y + r/2, yc + x + r/2);
  bar (xc - y - r/2, yc - x - r/2, xc - y + r/2, yc - x + r/2);
  bar (xc + y - r/2, yc - x - r/2, xc + y + r/2, yc - x + r/2);
  // outlines
  setcolor (COLOR (random (256), random (256), random (256)));
  rectangle (xc + x - r/2, yc + y - r/2, xc + x + r/2, yc + y + r/2);
  rectangle (xc - x - r/2, yc + y - r/2, xc - x + r/2, yc + y + r/2);
  rectangle (xc - x - r/2, yc - y - r/2, xc - x + r/2, yc - y + r/2);
  rectangle (xc + x - r/2, yc - y - r/2, xc + x + r/2, yc - y + r/2);
  rectangle (xc + y - r/2, yc + x - r/2, xc + y + r/2, yc + x + r/2);
  rectangle (xc - y - r/2, yc + x - r/2, xc - y + r/2, yc + x + r/2);
  rectangle (xc - y - r/2, yc - x - r/2, xc - y + r/2, yc - x + r/2);
  rectangle (xc + y - r/2, yc - x - r/2, xc + y + r/2, yc - x + r/2);
}

// -----

int main (int argc, char *argv[])
{
  int
    x, y, r,
    n,
    stop = NOPE;
  
  srand (time(NULL));
  setwinoptions ("", -1, -1, SDL_WINDOW_FULLSCREEN);
  initwindow (0, 0); // fullscreen
  setbkcolor (BLACK);
  setcolor (RED);
  cleardevice ();
  refresh ();
  xc = getmaxx () / 2;
  yc = getmaxy () / 2;
  /*
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  outtextxy (xc, yc, "Press a key to start:");
  getch ();
   */
  n = 0;
  
  while (!stop) {
    
    // define random position and radius of a circle
    x = random (xc);
    y = random (yc);
    r = 5 + random (25);
    setcolor (COLOR (random (256), random (256), random (256)));
    setfillstyle (USER_FILL, getcolor ());
    // draw 8 filled circles
    rnd_circles (x, y, r);
    
    // define a random box
    x = random (xc);
    y = random (yc);
    r = 5 + random (25);
    setcolor (COLOR (random (256), random (256), random (256)));
    setfillstyle (USER_FILL, getcolor ());
    // draw 8 boxes
    rnd_bars (x, y, r);
    
    if (10 == ++n)
      refresh ();
    
    // every 20 frames, fade the graphics
    if (20 == n) {
    
      // black with minimum transparency
      setcolor (COLOR (0, 0, 0));
      setalpha (getcolor (), 1);
      setfillstyle (SOLID_FILL, getcolor ());
      // overlap the screen with transparent black
      bar (0, 0, getmaxx (), getmaxy());
      refresh ();
      n = 0;
      
      if (WM_LBUTTONDOWN == mouseclick() || kbhit ())
	stop = YEAH;
      
    }
  }
  
  closegraph ();
  
}

// ----- end of file kaleido.c
