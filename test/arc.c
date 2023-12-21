/* arc example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;
  int stangle = 45, endangle = 135;
  int radius = 100;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;
  setcolor(getmaxcolor());

  /* draw arc */
  arc(midx, midy, stangle, endangle, radius);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
