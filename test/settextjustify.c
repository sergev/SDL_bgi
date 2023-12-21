/* settextjustify example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

/* function prototype */
void xat(int x, int y);

/* horizontal text justification settings */
char *hjust[] = { "LEFT_TEXT", "CENTER_TEXT", "RIGHT_TEXT" };

/* vertical text justification settings */
char *vjust[] = { "BOTTOM_TEXT", "CENTER_TEXT", "TOP_TEXT" };

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy, hj, vj;

  char msg[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* loop through text justifications */
  for (hj = LEFT_TEXT; hj <= RIGHT_TEXT; hj++)
    for (vj = LEFT_TEXT; vj <= RIGHT_TEXT; vj++) {
      cleardevice();

      /* set the text justification */
      settextjustify(hj, vj);

      /* create a message string */
      sprintf(msg, "%s  %s", hjust[hj], vjust[vj]);

      /* create crosshairs on the screen */
      xat(midx, midy);

      /* output the message */
      outtextxy(midx, midy, msg);

      getch();
    }

  /* clean up */
  closegraph();
  return 0;
}

void xat(int x, int y)
{				/* draw an x at (x,y) */
  line(x - 4, y, x + 4, y);
  line(x, y - 4, x, y + 4);
}
