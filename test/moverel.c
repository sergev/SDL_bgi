/* moverel example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  char msg[80];

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  /* move the CP to location (20,30) */
  moveto(20, 30);

  /* plot a pixel at the CP */
  putpixel(getx(), gety(), getmaxcolor());

  /* create and output a message at (20,30) */
  sprintf(msg, " (%d, %d)", getx(), gety());
  outtextxy(20, 30, msg);

  /* move to a point a relative distance away from the current CP */
  moverel(100, 100);

  /* plot a pixel at the CP */
  putpixel(getx(), gety(), getmaxcolor());

  /* create and output a message at CP */
  sprintf(msg, " (%d, %d)", getx(), gety());
  outtext(msg);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
