/* textwidth example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int x = 0, y = 0;
  int i;
  char msg[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  y = getmaxy() / 2;
  settextjustify(LEFT_TEXT, CENTER_TEXT);
  for (i = 1; i < 11; i++) {
    /* select the text style, direction, and size */
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, i);

    /* create a message string */
    sprintf(msg, "Size: %d", i);

    /* output the message */
    outtextxy(x, y, msg);

    /* advance to the end of the text */
    x += textwidth(msg);
  }

  /* clean up */
  getch();
  closegraph();
  return 0;
}
