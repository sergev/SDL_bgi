/* setwritemode example */

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

  xmax = getmaxx();
  ymax = getmaxy();

  /* select XOR drawing mode */
  setwritemode(XOR_PUT);

  /* draw a line */
  line(0, 0, xmax, ymax);
  getch();

  /* erase the line by drawing over it */
  line(0, 0, xmax, ymax);
  getch();

  /* select overwrite drawing mode */
  setwritemode(COPY_PUT);

  /* draw a line */
  line(0, 0, xmax, ymax);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
