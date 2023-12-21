/* linebuffers.c  -*- C -*-
 * 
 * To compile:
 * gcc -o linebuffers linebuffers.c -lSDL_bgi -lSDL2
 * 
 * getlinebuffer() / putlinebuffer() demonstration.
 *
 * By Guido Gonzato, February 2020.
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

#include <string.h>
#include <graphics.h>

#define WIDTH  256
#define HEIGHT 256

void shiftbuffer (Uint32 *line)
{
  // scroll line elements 1 position to the right
  
  Uint32
    tmp;
  
  tmp = line[0];
  memmove ( (void *) &line[0], (const void *) &line[1],
	    (size_t) ((WIDTH - 1) * sizeof (Uint32)) );
  line[WIDTH - 1] = tmp;
  
} // shiftbuffer()

// -----

int main (int argc, char *argv[])
{
  Uint32
    hues[HEIGHT][WIDTH],
    linebuffer[WIDTH];
  
  // initialise the buffer

  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
      hues[y][x] = colorRGB (x, y, abs (x - y));
  
  setwinoptions ("Using putlinebuffer()", -1, -1, -1);
  initwindow (WIDTH, HEIGHT);
  refresh ();
  showinfobox ("Left click to switch between\n"
	       "putlinebuffer() and putpixel();\n"
	       "right click to stop.");
  
  putbuffer (hues[0]);
  refresh ();
  int mode = 1;
  
  while (! ismouseclick (WM_RBUTTONDOWN)) {
    
    if (1 == mode) {
      for (int y = 0; y < HEIGHT; y++) {
	getlinebuffer (y, linebuffer);
	shiftbuffer (linebuffer);
	putlinebuffer (y, linebuffer);
      }
    }
    else {
      for (int y = 0; y < HEIGHT; y++) {
	for (int x = 0; x < WIDTH; x++)
	  linebuffer[x] = getpixel (x, y);
	shiftbuffer (linebuffer);
	for (int x = 0; x < WIDTH; x++)
	  putpixel (x, y, COLOR32(linebuffer[x]));
      }
    }
    
    if (WM_LBUTTONDOWN == mouseclick ()) {
      mode ^= 1;
      if (1 == mode)
	puts ("using getlinebuffer()/putlinebuffer()");
      else
	puts ("using getpixel()/putpixel()");
    }
    
    delay (5);
    refresh ();
  
  } // while
  
  puts ("Bye!");
  closegraph();
  
}

// ----- end of file linebuffers.c
