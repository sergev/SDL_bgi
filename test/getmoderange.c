/* getmoderange example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;
  int low, high;
  char mrange[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* get the mode range for this driver */
  getmoderange(gdriver, &low, &high);

  /* convert mode range info. into strings */
  sprintf(mrange, "This driver supports modes %d..%d", low, high);

  /* display the information */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(midx, midy, mrange);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
