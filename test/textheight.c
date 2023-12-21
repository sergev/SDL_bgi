/* textheight example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int y = 0;
  int i;
  char msg[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  /* draw some text on the screen */
  for (i = 1; i < 11; i++) {
    /* select the text style, direction, and size */
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, i);

    /* create a message string */
    sprintf(msg, "Size: %d", i);

    /* output the message */
    outtextxy(1, y, msg);

    /* advance to the next text line */
    y += textheight(msg);
  }

  /* clean up */
  getch();
  closegraph();
  return 0;
}
