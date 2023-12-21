/* setfillstyle example */

#include <graphics.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

/* the names of the fill styles supported */
char *fname[] =
{ "EMPTY_FILL", "SOLID_FILL", "LINE_FILL", "LTSLASH_FILL",
  "SLASH_FILL", "BKSLASH_FILL", "LTBKSLASH_FILL", "HATCH_FILL",
  "XHATCH_FILL", "INTERLEAVE_FILL", "WIDE_DOT_FILL", "CLOSE_DOT_FILL",
  "USER_FILL" };

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int style, midx, midy;

  char stylestr[40];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  for (style = EMPTY_FILL; style < USER_FILL; style++) {
    /* select the fill style */
    setfillstyle(style, getmaxcolor());

    /* convert style into a string */
    strcpy(stylestr, fname[style]);

    /* fill a bar */
    bar3d(0, 0, midx - 10, midy, 0, 0);

    /* output a message */
    outtextxy(midx, midy, stylestr);

    /* wait for a key */
    getch();
    cleardevice();
  }

  /* clean up */
  getch();
  closegraph();

  return 0;
}
