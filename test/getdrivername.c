/* getdrivername example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;

  /* stores the device driver name */
  char *drivername;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");
  setcolor(getmaxcolor());

  /* get the name of the device driver in use */
  drivername = getdrivername();

  /* for centering text onscreen */
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  /* output the name of the driver */
  outtextxy(getmaxx() / 2, getmaxy() / 2, drivername);

  /* clean up */
  getch();
  closegraph();

  return 0;
}
