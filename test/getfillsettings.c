/* getfillsettings example */

#include <graphics.h>
#include <stdlib.h>
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
  struct fillsettingstype fillinfo;

  int midx, midy;
  char patstr[40], colstr[40];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* get info about current fill pattern and color */
  getfillsettings(&fillinfo);

  /* convert fill information into strings */
  sprintf(patstr, "%s is the fill style.", fname[fillinfo.pattern]);
  sprintf(colstr, "%d is the fill color.", fillinfo.color);

  /* display the information */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(midx, midy, patstr);
  outtextxy(midx, midy + 2 * textheight("W"), colstr);

  /* clean up */

  getch();
  closegraph();
  return 0;
}
