/* linerel example */

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

  /* create and output a message at (20,30) */
  sprintf(msg, " (%d, %d)", getx(), gety());
  outtextxy(20, 30, msg);

  /* draw line to a point a relative distance away from current CP */
  linerel(100, 100);

  /* create and output a message at CP */
  sprintf(msg, " (%d, %d)", getx(), gety());
  outtext(msg);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
