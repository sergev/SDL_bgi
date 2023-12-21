/* getmaxy example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;
  char xrange[80], yrange[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* convert max resolution values into strings */
  sprintf(xrange, "X values range from 0..%d", getmaxx());
  sprintf(yrange, "Y values range from 0..%d", getmaxy());

  /* display the information */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(midx, midy, xrange);
  outtextxy(midx, midy + textheight("W"), yrange);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
