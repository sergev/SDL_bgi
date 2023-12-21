/* fillpoly example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int i, maxx, maxy;

  /* our polygon array */
  int poly[8];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  maxx = getmaxx();
  maxy = getmaxy();

  poly[0] = 20;			/* first vertex */
  poly[1] = maxy / 2;
  poly[2] = maxx - 20;		/* second vertex */
  poly[3] = 20;
  poly[4] = maxx - 50;		/* third vertex */
  poly[5] = maxy - 20;
  poly[6] = maxx / 2;		/* fourth, fillpoly automatically */
  poly[7] = maxy / 2;		/* closes the polygon */


  /* loop through the fill patterns */
  for (i = EMPTY_FILL; i < USER_FILL; i++) {
    /* set fill pattern */
    setfillstyle(i, getmaxcolor());

    /* draw a filled polygon */
    fillpoly(4, poly);
    getch();
  }

  /* clean up */
  closegraph();
  return 0;
}
