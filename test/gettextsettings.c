/* gettextsettings example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

/* the names of the supported fonts */
char *font[] =
{ "DEFAULT_FONT", "TRIPLEX_FONT", "SMALL_FONT", "SANS_SERIF_FONT",
  "GOTHIC_FONT" };

/* the names of the text directions supported */
char *dir[] = { "HORIZ_DIR", "VERT_DIR" };

/* horizontal text justifications supported */
char *hjust[] = { "LEFT_TEXT", "CENTER_TEXT", "RIGHT_TEXT" };

/* vertical text justifications supported */

char *vjust[] = { "BOTTOM_TEXT", "CENTER_TEXT", "TOP_TEXT" };

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  struct textsettingstype textinfo;
  int midx, midy, ht;
  char fontstr[80], dirstr[80], sizestr[80];
  char hjuststr[80], vjuststr[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* get information about current text settings */
  gettextsettings(&textinfo);

  /* convert text information into strings */
  sprintf(fontstr, "%s is the text style.", font[textinfo.font]);
  sprintf(dirstr, "%s is the text direction.", dir[textinfo.direction]);
  sprintf(sizestr, "%d is the text size.", textinfo.charsize);
  sprintf(hjuststr, "%s is the horizontal justification.",
	  hjust[textinfo.horiz]);
  sprintf(vjuststr, "%s is the vertical justification.",
	  vjust[textinfo.vert]);

  /* display the information */
  ht = textheight("W");
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(midx, midy, fontstr);
  outtextxy(midx, midy + 2 * ht, dirstr);
  outtextxy(midx, midy + 4 * ht, sizestr);
  outtextxy(midx, midy + 6 * ht, hjuststr);

  outtextxy(midx, midy + 8 * ht, vjuststr);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
