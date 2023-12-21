/* closegraph example */

#include <graphics.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode, x, y;

  /* initialize graphics mode */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  x = getmaxx() / 2;
  y = getmaxy() / 2;

  /* output a message */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(x, y, "Press a key to close the graphics system:");

  getch();			/* wait for a key */
  
  /* closes down the graphics system */
  closegraph();
  printf("We're now back in text mode.\n");
  printf("Press any key to halt:");
  getch();
  return 0;

}
