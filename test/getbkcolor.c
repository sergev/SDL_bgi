/* getbkcolor example */

#include <graphics.h>
#include "dos.h"

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int bkcolor, midx, midy;
  char bkname[35];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;
  setcolor(getmaxcolor());

  /* for centering text on the display */
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  /* get the current background color */
  bkcolor = getbkcolor();

  /* convert color value into a string */
  itoa(bkcolor, bkname, 10);
  strcat(bkname, " is the current background color.");

  /* display a message */
  outtextxy(midx, midy, bkname);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
