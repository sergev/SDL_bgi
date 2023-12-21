/* putimage example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define ARROW_SIZE 10

void draw_arrow(int x, int y);

int main(int argc, char *argv[])
{
  /* request autodetection */
  int gdriver = DETECT, gmode;
  void *arrow;
  int x, y, maxx;
  unsigned int size;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  maxx = getmaxx();
  x = 0;
  y = getmaxy() / 2;
  draw_arrow(x, y);

  /* calculate the size of the image and allocate space for it */
  size =
      imagesize(x, y - ARROW_SIZE, x + (4 * ARROW_SIZE), y + ARROW_SIZE);
  arrow = malloc(size);

  /* grab the image */
  getimage(x, y - ARROW_SIZE, x + (4 * ARROW_SIZE), y + ARROW_SIZE, arrow);

  /* repeat until a key is pressed */
  while (!kbhit()) {
    /* erase old image */
    putimage(x, y - ARROW_SIZE, arrow, XOR_PUT);
    x += ARROW_SIZE;
    if (x >= maxx)
      x = 0;

    /* plot new image */
    putimage(x, y - ARROW_SIZE, arrow, XOR_PUT);
  }

  free(arrow);
  closegraph();
  return 0;
}

void draw_arrow(int x, int y)
{
  moveto(x, y);
  linerel(4 * ARROW_SIZE, 0);
  linerel(-2 * ARROW_SIZE, -1 * ARROW_SIZE);
  linerel(0, 2 * ARROW_SIZE);

  linerel(2 * ARROW_SIZE, -1 * ARROW_SIZE);
}
