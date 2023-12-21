/* graphresult example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode, errorcode;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  /* read result of initialization */
  errorcode = graphresult();

  if (errorcode != grOk) {	/* an error occurred */
    printf("Graphics error: %s\n", grapherrormsg(errorcode));
    printf("Press any key to halt:");
    getch();
    exit(1);			/* terminate with an error code */
  }

  /* draw a line */
  line(0, 0, getmaxx(), getmaxy());

  /* clean up */
  getch();
  closegraph();
  return 0;
}
