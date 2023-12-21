/* mousetest.c  -*- C -*-
 *
 * To compile:
 * gcc -o mousetest mousetest.c -lSDL_bgi -lSDL2
 *
 * Shows how to detect and use mouse clicks.
 * By Guido Gonzato, 2015-2022
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

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

int main (int argc, char *argv[])
{

  int
    gd, gm = SDL_800x600,
    mclick, len,
    isize;
  char
    s[] = " Click around (click here to exit) ",
    *image;
  
  initgraph (&gd, &gm, "");
  setbkcolor (BLACK);
  setcolor (RED);
  setviewport (20, 20, getmaxx() - 20, getmaxy() - 20, 1);
  clearviewport ();
  rectangle (0, 0, getmaxx () - 40, getmaxy () - 40);

  len = strlen(s) * 8; // assumes an 8x8 font
  rectangle (0, 0, len, 16);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  outtextxy (0, 4, s);
  isize = imagesize (0, 0, len, 16);
  image = malloc (isize); // no checks, come on...
  getimage (0, 0, len, 16, image);
  setlinestyle (SOLID_LINE, 0, THICK_WIDTH);
  
  int
    tmp;

  while (1) {
  
    // we use mouseclick () because we want to check for
    // all mouse events; ismouseclick () is much faster
    mclick = mouseclick ();
    
    if (0 == mclick)
      continue;
    
    // top left box
    if (mousex () < len && mousey () < 16 &&
      WM_LBUTTONDOWN == mclick)
      break;

    switch (mclick) {
    
    case WM_MOUSEMOVE:
      putpixel (mousex (), mousey (), getcolor ());
      break;
      
    // double click - draw an X
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
      tmp = 10 + random (40);
      setcolor (COLOR (random(255), random(255), random(255)));
      line (mousex () - tmp, mousey () - tmp,
	    mousex () + tmp, mousey () + tmp);
      line (mousex () + tmp, mousey () - tmp,
	    mousex () - tmp, mousey () + tmp);
      break;
      
    // mouse release - draw a +
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
      tmp = 10 + random (40);
      setcolor (COLOR (random(255), random(255), random(255)));
      line (mousex (), mousey () - tmp,
	    mousex (), mousey () + tmp);
      line (mousex () - tmp, mousey (),
	    mousex () + tmp, mousey ());
      break;
      
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_WHEELUP:
    case WM_WHEELDOWN:
      setfillstyle (1, COLOR (random(255), random(255), random(255)));
      setcolor (COLOR (random(255), random(255), random(255)));
      if (0 == random (2))
        fillellipse (mousex (), mousey (),
		     10 + mclick*random(40),
		     10 + mclick*random(40));
      else
        bar (mousex () - mclick*10, mousey () - mclick*10,
             mousex () + mclick*10, mousey () + mclick*10);
      break;
      
      default:
        ;
    
    } // switch (mclick)
  
    putimage (0, 0, image, COPY_PUT);
      
  } // while
  
  closegraph ();
  return 0;

} // main ()

// ----- end of file mousetest.c
