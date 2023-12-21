/* setallpalette example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  struct palettetype pal;
  int color, maxcolor, ht;
  int y = 10;
  char msg[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  maxcolor = getmaxcolor();
  ht = 2 * textheight("W");

  /* grab a copy of the palette */
  getpalette(&pal);

  /* display the default palette colors */
  for (color = 1; color <= maxcolor; color++) {
    setcolor(color);
    sprintf(msg, "Color: %d", color);
    outtextxy(1, y, msg);
    y += ht;
  }

  /* wait for a key */
  getch();

  /* black out the colors one by one */
  for (color = 1; color <= maxcolor; color++) {
    setpalette(color, BLACK);
    getch();
  }

  /* restore the palette colors */
  /* in Turbo C, onscreen pixels are also chenged */
  setallpalette(&pal);
  
  /* !!! ADDED - display the default palette colors */
  y = 0;
  for (color = 1; color <= maxcolor; color++) {
    setcolor(color);
    sprintf(msg, "Color: %d", color);
    outtextxy(1, y, msg);
    y += ht;
  }

  /* clean up */
  getch();
  closegraph();
  return 0;
}
