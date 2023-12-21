/* settextstyle example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

/* the names of the text styles supported */
char *fname[] = { "DEFAULT font", "TRIPLEX font",
  "SMALL font", "SANS SERIF_font",
  "GOTHIC_font", "SCRIPT font",
  "SIMPLEX font", "TRIPLEX SCRIPT font",
  "COMPLEX font", "EUROPEAN font",
  "BOLD font"
};

int main(int argc, char *argv[])
{
  /* request autodetection */

  int gdriver = DETECT, gmode;
  int style, midx, midy;
  int size = 1;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  /* loop through the available text styles */
  for (style = DEFAULT_FONT; style <= BOLD_FONT; style++) {
    cleardevice();
    if (style == TRIPLEX_FONT)
      size = 4;
    /* select the text style */
    settextstyle(style, HORIZ_DIR, size);

    /* output a message */
    outtextxy(midx, midy, fname[style]);
    getch();
  }
  /* clean up */

  closegraph();
  return 0;
}
