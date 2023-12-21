/* boo.c  -*- C -*-
 * 
 * To compile:
 * gcc -o boo boo.c -lSDL_bgi -lSDL2
 *
 * boo.c shows how to make a window transparent and change some of
 * its properties.
 * By Guido Gonzato, August 2020
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
#include <time.h>

// -----

int main (int argc, char *argv[])
{

  char
    *boo = "BOOO!";
  int
    winid,
    x, y,
    stop = 0;

  // physical screen size, regardless of window dimensions
  getscreensize (&x, &y);
  
  setwinoptions ("", SDL_WINDOWPOS_CENTERED,
		 SDL_WINDOWPOS_CENTERED, SDL_WINDOW_BORDERLESS);
  initwindow (32 * strlen (boo), 32); // 4 times the usual size
  setbkcolor (BLACK);
  cleardevice ();
  winid = getcurrentwindow ();

  // make the window opaque
  SDL_SetWindowOpacity (bgi_window, 1.0);
  setcolor (RED);
  settextstyle (DEFAULT_FONT, HORIZ_DIR, 4);
  outtextxy (0, 0, boo);
  refresh ();
  
  while (! stop) {
    
    srand (time(NULL));
    stop = edelay (random (1000));
    if (stop)
      break;
    
    // make the window transparent (disappear)
    SDL_SetWindowOpacity (bgi_window, 0.0);
    refresh ();
    stop = edelay (random (1500));
    
    // opaque again - reappear
    resetwinoptions (winid, "", random (x), random (y));
    SDL_SetWindowOpacity (bgi_window, 1.0);
    refresh ();
  
  } // while
  
  closegraph ();
  return 0;
  
}

// ----- end of file boo.c
