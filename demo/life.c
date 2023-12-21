/* life.c  -*- C -*-
 * 
 * To compile:
 * gcc -o life life.c -lSDL_bgi -lSDL2
 * 
 * Conway's Game of Life.
 * By Guido Gonzato, May 2022.
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
#include <time.h>

#include <graphics.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#define SIZE  500
#define ALIVE 1
#define DEAD  0

char a[SIZE][SIZE], b[SIZE][SIZE];

void print_cells (char[SIZE][SIZE]);
void evolve_cells (char[SIZE][SIZE], char[SIZE][SIZE]);
void initialise (int);

// -----

void initialise (int percentage)
{
  for (int x = 0; x < SIZE; x++)
    for (int y = 0; y < SIZE; y++)
      a[x][y] = (random (100) < percentage) ? ALIVE: DEAD;
}

// -----

void main_loop (void)
{

  int
    stop = 0;
  
  while (! stop) {
    evolve_cells (a, b);
    print_cells (b);
    refresh ();
    evolve_cells (b, a);
    print_cells (a);
    refresh ();
    if (kbhit ()) {
      if (KEY_ESC == lastkey ())
	stop = 1;
      else
	initialise (random (100));
    }
  }

} // --- main_loop ()

// -----

int main (int argc, char *argv[])
{
  unsigned int percentage;
  
  srand (time(NULL));
  if (2 == argc)
    percentage = atoi (argv [1]); // no checks!
  else
    percentage = 10;

  showinfobox ("Press any key to restart,\n"
               "or ESC to exit.");
  
  initialise (percentage);
  initwindow (SIZE, SIZE);
  main_loop ();
  
  closegraph ();
  
} // main() ()

// -----

#define NORTH(n) (n == 0) ? (SIZE - 1) : (n - 1)
#define SOUTH(n) (n == SIZE - 1) ? (0) : (n + 1)
#define WEST(n)  (n == 0) ? (SIZE - 1) : (n - 1)
#define EAST(n)  (n == SIZE - 1) ? (0) : (n + 1)

void print_cells (char cells[SIZE][SIZE])
{
  int x, y, color;
  
  for (x = 0; x < SIZE; x++)
    for (y = 0; y < SIZE; y++) {
      if (DEAD == cells[x][y])
	color = WHITE;
      else
	color = BLUE;
      putpixel (x, y, color);
    }
} // print_cells ()

// -----

void evolve_cells (char old[SIZE][SIZE], char new[SIZE][SIZE])
{
  // evolve cells in array new[][] based on the state of array old[][]
  int x, y, n;
  
  for (x = 0; x < SIZE; x++)
    for (y = 0; y < SIZE; y++) {
      // count alive neighbours of (x, y) cell, 
      // starting from N, clockwise
      n = 0;
      if (ALIVE == old[x][NORTH(y)])         // N
	n++;
      if (ALIVE == old[EAST(x)][NORTH(y)])   // NE
	n++;
      if (ALIVE == old[EAST(x)][y])          // E
	n++;
      if (ALIVE == old[EAST(x)][SOUTH(y)])   // SE
	n++;
      if (ALIVE == old[x][SOUTH(y)])         // S
	n++;
      if (ALIVE == old[WEST(x)][SOUTH(y)])   // SW
	n++;
      if (ALIVE == old[WEST(x)][y])          // W
	n++;
      if (ALIVE == old[WEST(x)][NORTH(y)])   // NW
	n++;
      if (n < 2 || n > 3)
        new[x][y] = DEAD;
      else
        if (3 == n)
          new[x][y] = ALIVE;
        else
          new[x][y] = old[x][y];
    }
  
} // evolve_cells ()

// ----- end of file life.c
