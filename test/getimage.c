/* getimage example */

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>

void save_screen(void *buf[4]);
void restore_screen(void *buf[4]);

int maxx, maxy;
int main(int argc, char *argv[])
{
  int gdriver = DETECT, gmode;
  void *ptr[4];

  /* autodetect the graphics driver and mode */
  initgraph(&gdriver, &gmode, "C:\\TC\\BGI");

  maxx = getmaxx();
  maxy = getmaxy();

  /* draw an image on the screen */
  rectangle(0, 0, maxx, maxy);
  line(0, 0, maxx, maxy);
  line(0, maxy, maxx, 0);
  save_screen(ptr);		/* save the current screen */
  getch();			/* pause screen */
  cleardevice();		/* clear screen */
  restore_screen(ptr);		/* restore the screen */
  getch();			/* pause screen */

  closegraph();
  return 0;
}

void save_screen(void *buf[4])
{
  unsigned size;
  int ystart = 0, yend, yincr, block;
  yincr = (maxy + 1) / 4;
  yend = yincr;

  /* get byte size of image */
  size = imagesize(0, ystart, maxx, yend);
  for (block = 0; block <= 3; block++) {
    if ((buf[block] = malloc(size)) == NULL) {
      closegraph();
      printf("Error: not enough heap space in save_screen().\n");
      exit(1);
    }
    getimage(0, ystart, maxx, yend, buf[block]);

    ystart = yend + 1;
    yend += yincr + 1;
  }
}

void restore_screen(void *buf[4])
{
  int ystart = 0, yend, yincr, block;
  yincr = (maxy + 1) / 4;
  yend = yincr;
  for (block = 0; block <= 3; block++) {
    putimage(0, ystart, buf[block], COPY_PUT);
    free(buf[block]);
    ystart = yend + 1;

    yend += yincr + 1;
  }
}
