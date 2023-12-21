/* rectangle example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int left, top, right, bottom;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  left = getmaxx() / 2 - 50;
  top = getmaxy() / 2 - 50;
  right = getmaxx() / 2 + 50;
  bottom = getmaxy() / 2 + 50;

  /* draw a rectangle */
  rectangle(left, top, right, bottom);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
