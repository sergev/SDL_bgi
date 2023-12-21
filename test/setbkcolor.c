/* setbkcolor example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* _select driver and mode that supports multiple background colors */
  int gdriver = DETECT, gmode;
  int bkcol, maxcolor, x, y;
  char msg[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  /* maximum color index supported */
  maxcolor = getmaxcolor();

  /* for centering text messages */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  x = getmaxx() / 2;
  y = getmaxy() / 2;

  /* loop through the available colors */
  for (bkcol = 0; bkcol <= maxcolor; bkcol++) {

    /* clear the screen */
    cleardevice();

    /* select a new background color */
    setbkcolor(bkcol);

    /* output a messsage */
    if (bkcol == WHITE)
      setcolor(BLUE);
    sprintf(msg, "Background color: %d", bkcol);
    outtextxy(x, y, msg);
    getch();
  }

  /* clean up */
  closegraph();
  return 0;
}
