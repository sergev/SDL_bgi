/* setviewport example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define CLIP_ON 1		/* activates clipping in viewport */

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  setcolor(getmaxcolor());

  /* message in default full-screen viewport */
  outtextxy(0, 0, "* <-- (0, 0) in default viewport");

  /* create a smaller viewport */
  setviewport(50, 50, getmaxx() - 50, getmaxy() - 50, CLIP_ON);

  /* display some text */
  outtextxy(0, 0, "* <-- (0, 0) in smaller viewport");

  /* clean up */
  getch();
  closegraph();
  return 0;
}
