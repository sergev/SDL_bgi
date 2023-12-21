/* floodfill example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int maxx, maxy;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  maxx = getmaxx();
  maxy = getmaxy();

  /* select drawing color */
  setcolor(getmaxcolor());

  /* select fill color */
  setfillstyle(SOLID_FILL, getmaxcolor());

  /* draw a border around the screen */
  rectangle(0, 0, maxx, maxy);

  /* draw some circles */
  circle(maxx / 3, maxy / 2, 50);
  circle(maxx / 2, 20, 100);
  circle(maxx - 20, maxy - 50, 75);
  circle(20, maxy - 20, 25);

  /* wait for a key */
  getch();

  /* fill in bounded region */
  floodfill(2, 2, getmaxcolor());

  /* clean up */
  getch();
  closegraph();
  return 0;
}
