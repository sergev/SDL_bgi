/* setfillpattern example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int maxx, maxy;

  /* a user-defined fill pattern */
  char pattern[8] = { 0x00, 0x70, 0x20, 0x27, 0x24, 0x24, 0x07, 0x00 };

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  maxx = getmaxx();
  maxy = getmaxy();
  setcolor(getmaxcolor());

  /* select a user-defined fill pattern */
  setfillpattern(pattern, getmaxcolor());

  /* fill the screen with the pattern */
  bar(0, 0, maxx, maxy);

  /* clean up */
  getch();
  closegraph();

  return 0;
}
