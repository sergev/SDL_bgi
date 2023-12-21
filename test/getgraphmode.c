/* getgraphmode example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy, mode;
  char numname[80], modename[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* get mode number and name strings */
  mode = getgraphmode();
  sprintf(numname, "%d is the current mode number.", mode);
  sprintf(modename, "%s is the current graphics mode.", getmodename(mode));

  /* display the information */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(midx, midy, numname);
  outtextxy(midx, midy + 2 * textheight("W"), modename);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
