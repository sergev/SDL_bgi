/* grapherrormsg example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define NONSENSE -50

int main(int argc, char *argv[])
{
  /* force an error to occur */
  int gdriver = NONSENSE, gmode, errorcode;

  /* initialize graphics mode */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  /* read result of initialization */
  errorcode = graphresult();

  /* if an error occurred, then output descriptive error message */
  if (errorcode != grOk) {
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
