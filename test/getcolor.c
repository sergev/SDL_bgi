/* getcolor example */

#include <graphics.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int color, midx, midy;
  char colname[35];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;
  setcolor(getmaxcolor());

  /* for centering text on the display */
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  /* get the current drawing color */
  color = getcolor();

  /* convert color value into a string */
  itoa(color, colname, 10);
  strcat(colname, " is the current drawing color.");

  /* display a message */
  outtextxy(midx, midy, colname);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
