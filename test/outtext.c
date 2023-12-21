/* outtext example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int midx, midy;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* move the CP to the center of the screen */
  moveto(midx, midy);

  /* output text starting at the CP */
  outtext("This ");
  outtext("is ");
  outtext("a ");
  outtext("test.");

  /* clean up */
  getch();
  closegraph();
  return 0;
}
