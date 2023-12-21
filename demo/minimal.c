#include <graphics.h>

int main (int argc, char *argv[])
{
  int i, gd = DETECT, gm = 0;
  initgraph (&gd, &gm, "");
  setbkcolor (BLACK);
  cleardevice ();
  outtextxy (0, 0, "Drawing 1000 lines...");
  for (i = 0; i < 1000; i++) {
    setcolor (1 + random (15));
    line (random(getmaxx()), random(getmaxy()),
         random (getmaxx()), random(getmaxy()) );
  }
  getch ();
  closegraph ();
  return 0;
}
