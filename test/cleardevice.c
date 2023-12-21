/* cleardevice example */

#include <graphics.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;
  setcolor(getmaxcolor());

  /* for centering screen messages */
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  /* output a message to the screen */
  outtextxy(midx, midy, "Press any key to clear the screen:");

  getch();			/* wait for a key */
  cleardevice();		/* clear the screen */

  /* output another message */
  outtextxy(midx, midy, "Press any key to quit:");

  /* clean up */
  getch();
  closegraph();
  return 0;
}
