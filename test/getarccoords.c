/* getarccoords example */

#include <graphics.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  struct arccoordstype arcinfo;
  int midx, midy;
  int stangle = 45, endangle = 270;
  char sstr[80], estr[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* draw arc and get coordinates */
  setcolor(getmaxcolor());
  arc(midx, midy, stangle, endangle, 100);
  getarccoords(&arcinfo);

  /* convert arc information into strings */
  sprintf(sstr, "*- (%d, %d)", arcinfo.xstart, arcinfo.ystart);

  sprintf(estr, "*- (%d, %d)", arcinfo.xend, arcinfo.yend);

  /* output the arc information */
  outtextxy(arcinfo.xstart, arcinfo.ystart, sstr);
  outtextxy(arcinfo.xend, arcinfo.yend, estr);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
