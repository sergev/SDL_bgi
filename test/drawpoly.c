/* drawpoly example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int maxx, maxy;

  int poly[10];			/* our polygon array */

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
  poly[6] = maxx / 2;		/* fourth vertex */
  poly[7] = maxy / 2;
  poly[8] = poly[0];		/* drawpoly doesn't automatically close */
  poly[9] = poly[1];		/* the polygon, so we close it */

  drawpoly(5, poly);		/* draw the polygon */

  /* clean up */
  getch();
  closegraph();
  return 0;
}
