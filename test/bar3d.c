/* bar3d example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy, i;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* loop through the fill patterns */
  for (i = EMPTY_FILL; i < USER_FILL; i++) {
    /* set the fill style */
    setfillstyle(i, getmaxcolor());

    /* draw the 3-d bar */
    bar3d(midx - 50, midy - 50, midx + 50, midy + 50, 10, 1);
    getch();
  }

  /* clean up */
  closegraph();

  return 0;
}
