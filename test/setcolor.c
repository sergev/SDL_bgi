/* setcolor example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* select driver and mode that supports multiple drawing colors */
  int gdriver = DETECT, gmode;
  int color, maxcolor, x, y;
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
  for (color = 1; color <= maxcolor; color++) {
    cleardevice();		/* clear the screen */
    setcolor(color);		/* select new background color */

    /* output a messsage */
    sprintf(msg, "Color: %d", color);
    outtextxy(x, y, msg);
    getch();
  }

  /* clean up */
  closegraph();
  return 0;
}
