/* putpixel example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define PIXEL_COUNT 1000
#define DELAY_TIME  100		/* in milliseconds */

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int i, x, y, color, maxx, maxy, maxcolor, seed;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  maxx = getmaxx() + 1;
  maxy = getmaxy() + 1;
  maxcolor = getmaxcolor() + 1;

  while (!kbhit()) {
    /* seed the random number generator */
    seed = random(32767);
    srand(seed);
    for (i = 0; i < PIXEL_COUNT; i++) {

      x = random(maxx);
      y = random(maxy);
      color = random(maxcolor);
      putpixel(x, y, color);
    }
    delay(DELAY_TIME);
    srand(seed);
    for (i = 0; i < PIXEL_COUNT; i++) {
      x = random(maxx);
      y = random(maxy);
      color = random(maxcolor);
      if (color == getpixel(x, y))
	putpixel(x, y, 0);
    }
  }

  /* clean up */
  getch();
  closegraph();
  return 0;
}
