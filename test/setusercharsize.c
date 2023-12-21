/* setusercharsize example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  /* select a text style */
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);

  /* move to the text starting position */
  moveto(0, getmaxy() / 2);

  /* output some normal text */
  outtext("Norm ");

  /* make the text 1/3 the normal width */
  setusercharsize(1, 3, 1, 1);
  outtext("Short ");

  /* make the text 3 times normal width */
  setusercharsize(3, 1, 1, 1);

  outtext("Wide");

  /* clean up */
  getch();
  closegraph();
  return 0;
}
