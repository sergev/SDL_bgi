/* getdefaultpalette example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  char str[80];

  /* pointer to palette structure */
  struct palettetype *pal = NULL;
  int i;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  /* return a pointer to the default palette */
  pal = getdefaultpalette();
  for (i = 0; i < pal->size; i++) {
    printf(str, "colors[%d] = %d\n", i, pal->colors[i]);
    getch();
  }

  /* clean up */
  getch();
  closegraph();
  return 0;
}
