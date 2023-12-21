/* psychedelia.c  -*- C -*-
 *
 * To compile:
 * gcc -o psychedelia psychedelia.c -lSDL_bgi -lSDL2 -lm
 *
 * Variation on 'plasma.c', as described here:
 * https://lodev.org/cgtutor/plasma.html
 * By Guido Gonzato, 2018 - 2019.
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

#include <math.h>
#include <graphics.h>

#define  WIDTH  800
#define  HEIGHT 600

// -----

int main (int argc, char **argv)
{

  int
    col,
    r, g, b,      // colour indices
    d1, d2, d3,   // used to change the plasma parameters
    use_buffer = 1,
    counter = 0;

  // screen buffer. NOTE: height x width!
  Uint32 buffer [HEIGHT][WIDTH];

  float
    k1, k2, k3; // plasma parameters

  // parameters are allowed to range from 0 to 256

  k1 = 128.;
  k2 = 32.;
  k3 = 64.;
  d1 = -1;
  d2 = 1;
  d3 = 1;

  initwindow (WIDTH, HEIGHT);
  setbkcolor (BLACK);
  refresh ();

  // make a palette
  for (int i = 0; i < 255; i++)
    setrgbpalette (i,
		   (int) fabs((128. - 127.*sinf((float)i*M_PI/32.))),
		   (int) fabs((128. - 127.*sinf((float)i*M_PI/64.))),
		   (int) fabs((128. - 127.*sinf((float)i*M_PI/128.))) );

  showinfobox ("Left click to switch between\n"
	       "putbuffer() and putpixel();\n"
	       "right click to stop.");

  int time = SDL_GetTicks ();

  while (! ismouseclick (WM_RBUTTONDOWN)) {

    int
      maxx = getmaxx (),
      maxy = getmaxy ();

    for (int y = 0; y < maxy; y++) {
      for (int x = 0; x < maxx; x++) {

	col = (int) (
	  sinf (x / 50.) * k1 +
	  sinf (y / 40.) * k2 +
	  sinf ((x + y) / 30.) * k3);

	while (col > 255)
	  col -= 256;
	while (col < 0)
	  col += 256;

	r = RED_VALUE (col);
	g = GREEN_VALUE (col);
	b = BLUE_VALUE (col);

	// this would be slow:
	// setrgbpalette (col, r, g, b);
	// setrgbcolor (col);
	// _putpixel (x, y);

	if (1 == use_buffer)
	  buffer [y][x] = colorRGB (r, g, b);
	else
	  putpixel (x, y, COLOR(r, g, b));

      } // for x
    } // for y

    if (1 == use_buffer)
      putbuffer (buffer[0]);
    refresh ();
    counter++;

    if (SDL_GetTicks () >= time + 1000) {
      printf ("%d iterations/second\n", counter);
      time = SDL_GetTicks ();
      counter = 0;
    }

    // change the parameters
    k1 += d1;
    if (k1 < 2 || k1 > 255)
      d1 *= -1;
    k2 += d2;
    if (k2 < 2 || k2 > 255)
      d2 *= -1;
    k3 += d3;
    if (k3 < 2 || k3 > 255)
      d3 *= -1;

    if (WM_LBUTTONDOWN == mouseclick()) {
      use_buffer ^= 1;
      if (use_buffer)
	puts ("Using putbuffer()");
      else
	puts ("Using putpixel()");
    }

  } // while

  puts ("Bye!");
  closegraph ();
  return 0;

}

// ----- end of file psychedelia.c
