/* fillellipse example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy, i;
  int xradius = 100, yradius = 50;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* loop through the fill patterns */
  for (i = EMPTY_FILL; i < USER_FILL; i++) {
    /* set fill pattern */
    setfillstyle(i, getmaxcolor());

    /* draw a filled ellipse */
    fillellipse(midx, midy, xradius, yradius);
    getch();
  }

  /* clean up */
  closegraph();

  return 0;
}
