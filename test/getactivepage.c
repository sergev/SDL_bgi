/* getactivepage example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* select driver and mode that supports multiple pages */
  int gdriver = DETECT, gmode;
  int x, y, ht;
  char msg[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  x = getmaxx() / 2;
  y = getmaxy() / 2;
  ht = textheight("W");

  /*  select the off screen page for drawing */
  setactivepage(1);

  /* draw a line on page #1 */
  line(0, 0, getmaxx(), getmaxy());

  /* output a message on page #1 */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  sprintf(msg, "This is page #%d:", getactivepage());
  outtextxy(x, y, msg);
  outtextxy(x, y + ht, "Press any key to halt:");

  /* select drawing to page #0 */
  setactivepage(0);

  /* output a message  on page #0 */
  sprintf(msg, "This is page #%d:", getactivepage());
  outtextxy(x, y, msg);
  outtextxy(x, y + ht, "Press any key to view page #1:");
  getch();

  /* select page #1 as the visible page */
  setvisualpage(1);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
