/* delay example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  int gdriver = DETECT, gmode;
  int midx, midy, i;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* loop through the fill patterns with 4 second delays */
  for (i = SOLID_FILL; i < USER_FILL; i++) {
    /* set the fill style */
    setfillstyle(i, getmaxcolor());

    /* draw the bar */
    bar(midx - 50, midy - 50, midx + 50, midy + 50);
    delay(500);
  }

  /* clean up */
  closegraph();
  return 0;
}
