/* outtextxy example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* output text at center of the screen; CP doesn't get changed */
  outtextxy(midx, midy, "This is a test.");

  /* clean up */
  getch();
  closegraph();
  return 0;
}
