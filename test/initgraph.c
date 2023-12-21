/* initgraph example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;

  /* initialize graphics mode */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  /* draw a line */
  line(0, 0, getmaxx(), getmaxy());

  /* clean up */
  getch();
  closegraph();
  return 0;
}
