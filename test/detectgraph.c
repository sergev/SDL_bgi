/* detectgraph example */

#include <graphics.h>
#include <conio.h>

/* the names of the various cards supported */
char *dname[] = { "requests detection",
  "a CGA",
  "an MCGA",
  "an EGA",
  "a 64K EGA",
  "a monochrome EGA",
  "an IBM 8514",
  "a Hercules monochrome",
  "an AT&T 6300 PC",
  "a VGA",
  "an IBM 3270 PC"
};

int main(int argc, char *argv[])
{
  /* used to return detected hardware info. */
  int gdriver, gmode;

  /* detect the graphics hardware available */
  detectgraph(&gdriver, &gmode);
  gdriver = 0;

  /* display the information detected */
  clrscr();
  printf("You have '%s' video display card.\n", dname[gdriver]);
  printf("Press any key to halt:");
  getch();
  return 0;
}
