/* graphdefaults example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int maxx, maxy;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  maxx = getmaxx();
  maxy = getmaxy();

  /* output line with nondefault settings */
  setlinestyle(DOTTED_LINE, 0, 3);
  line(0, 0, maxx, maxy);
  outtextxy(maxx / 2, maxy / 3, "Before default values are restored.");
  getch();

  /* restore default values for everything */
  graphdefaults();

  /* clear the screen */
  cleardevice();

  /* output line with default settings */
  line(0, 0, maxx, maxy);
  outtextxy(maxx / 2, maxy / 3, "After restoring default values.");

  /* clean up */
  getch();
  closegraph();
  return 0;
}
