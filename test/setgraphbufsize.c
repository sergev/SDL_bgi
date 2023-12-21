/* setgraphbufsize example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define BUFSIZE 1000		/* internal graphics buffer size */

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int x, y, oldsize;
  char msg[80];

  /* _set size of internal graphics buffer before calling initgraph */
  oldsize = setgraphbufsize(BUFSIZE);

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  x = getmaxx() / 2;
  y = getmaxy() / 2;

  /* output some messages */
  sprintf(msg, "Graphics buffer size: %d", BUFSIZE);
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  outtextxy(x, y, msg);
  sprintf(msg, "Old graphics buffer size: %d", oldsize);
  outtextxy(x, y + textheight("W"), msg);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
