/* getx example */

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

  /* move to the screen center point */
  moveto(getmaxx() / 2, getmaxy() / 2);

  /* create a message string */
  sprintf(msg, "<-(%d, %d) is the here.", getx(), gety());

  /* display the message */
  outtext(msg);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
