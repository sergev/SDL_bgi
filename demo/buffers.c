/* buffers.c  -*- C -*-
 * 
 * To compile:
 * gcc -o buffers buffers.c -lSDL_bgi -lSDL2
 * 
 * getbuffer() / putbuffer() demonstration.
 *
 * !!! Does not work in Emscripten:
 * "Uncaught RuntimeError: memory access out of bounds"
 *
 * By Guido Gonzato, January 2020.
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

#include <stdlib.h>
#include <graphics.h>

#define WIDTH  256
#define HEIGHT 256

int main (int argc, char *argv[])
{
  // buffers
  Uint32
    red[HEIGHT][WIDTH],
    tmp[HEIGHT][WIDTH],
    hues[HEIGHT][WIDTH];
  
  Uint8
    alpha;
  
  int x, y;
  
  // initialise the buffers
  for (y = 0; y < HEIGHT; y++)
    for (x = 0; x < WIDTH; x++) {
      red[y][x] = 0xffff0000;
      hues[y][x] = colorRGB (x, y, abs (x - y));
    }
  
  setwinoptions ("Using putbuffer()", -1, -1, -1);
  initwindow (WIDTH, HEIGHT);
  setcolor (BLACK);
  setbkcolor (WHITE);
  cleardevice ();
  outtextxy (0, 0, "Click to continue:");
  refresh ();
  
  getclick ();
  putbuffer (red[0]);
  getbuffer (tmp[0]); // tmp == red
  setcolor (WHITE);
  outtextxy (0, 0, "Left click to continue:");
  refresh ();
  getclick ();
  
  for (alpha = 0; alpha < 128; alpha++) {
    
    for (int y = 0; y < HEIGHT; y++)
      for (int x = 0; x < WIDTH; x++) {
	hues[y][x] &= 0x00ffffff;    // clear alpha
	hues[y][x] |= (alpha << 24); // set alpha
      }
    
    putbuffer (tmp[0]);   // red screen
    putbuffer (hues[0]);  // hues + alpha on red
    refresh ();
    delay (10);
    
  } // for alpha

  outtextxy (0, 0, "Left click to finish:");
  refresh ();
  getclick ();

  closegraph ();
  
}

// ----- end of file buffers.c
