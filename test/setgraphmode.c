/* setgraphmode example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int x, y;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  x = getmaxx() / 2;
  y = getmaxy() / 2;

  /* output a message */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(x, y, "Press any key to exit graphics:");
  getch();

  /* restore system to text mode */
  restorecrtmode();
  printf("We're now in text mode.\n");
  printf("Press any key to return to graphics mode:");
  getch();

  /* return to graphics mode */
  setgraphmode(getgraphmode());

  /* output a message */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(x, y, "We're back in graphics mode.");
  outtextxy(x, y + textheight("W"), "Press any key to halt:");

  /* clean up */
  getch();
  closegraph();
  return 0;
}
