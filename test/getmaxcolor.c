/* getmaxcolor example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;
  char colstr[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* grab the color info. and convert it to a string */
  sprintf(colstr, "This mode supports colors 0..%d", getmaxcolor());

  /* display the information */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(midx, midy, colstr);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
