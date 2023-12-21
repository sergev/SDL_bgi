/* installuserfont example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

/* function prototype */
void checkerrors(void);
int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  int userfont;
  int midx, midy;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  midx = getmaxx() / 2;
  midy = getmaxy() / 2;

  /* install a user-defined font file */
  userfont = installuserfont("USER.CHR");

  /* select the user font */
  settextstyle(userfont, HORIZ_DIR, 4);

  /* output some text */
  outtextxy(midx, midy, "Testing!");

  /* clean up */
  getch();
  closegraph();
  return 0;
}
