/* ellipse example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;
  int stangle = 0, endangle = 360;
  int xradius = 100, yradius = 50;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;
  setcolor(getmaxcolor());

  /* draw ellipse */
  ellipse(midx, midy, stangle, endangle, xradius, yradius);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
