/* setaspectratio example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int xasp, yasp, midx, midy;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;
  setcolor(getmaxcolor());

  /* get current aspect ratio settings */
  getaspectratio(&xasp, &yasp);

  /* draw normal circle */
  circle(midx, midy, 100);
  getch();

  /* clear the screen */
  cleardevice();

  /* adjust the aspect for a wide circle */
  setaspectratio(xasp / 2, yasp);

  circle(midx, midy, 100);
  getch();

  /* adjust the aspect for a narrow circle */
  cleardevice();
  setaspectratio(xasp, yasp / 2);
  circle(midx, midy, 100);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
