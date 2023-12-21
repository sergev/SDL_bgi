/* sector example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy, i;
  int stangle = 45, endangle = 135;
  int xrad = 100, yrad = 50;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* loop through the fill patterns */
  for (i = EMPTY_FILL; i < USER_FILL; i++) {

    /* set the fill style */
    setfillstyle(i, getmaxcolor());

    /* draw the sector slice */
    sector(midx, midy, stangle, endangle, xrad, yrad);

    getch();
  }

  /* clean up */
  closegraph();
  return 0;
}
