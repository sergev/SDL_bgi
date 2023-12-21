/* line example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{

  /* request autodetection */
  int gdriver = DETECT, gmode;
  int xmax, ymax;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  setcolor(getmaxcolor());
  xmax = getmaxx();
  ymax = getmaxy();

  /* draw a diagonal line */
  line(0, 0, xmax, ymax);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
