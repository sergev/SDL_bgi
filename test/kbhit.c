#include <graphics.h>
#include <stdio.h>		// Provides sprintf

void outintxy(int x, int y, int value);

int main(int argc, char *argv[])
{
  int gdriver = DETECT, gmode, i;

  // Initialize the graphics window.
  initgraph (&gdriver, &gmode, "C:\\TC\\BGI");

  // Convert some numbers to strings and draw them in graphics window:
  outtextxy(20, 130, "Click in this graphics window,");
  outtextxy(20, 140, "and then press a key to stop.");
  outtextxy(10, 10, "Here are some numbers:");
  for (i = 0; !kbhit(); i++) {
    outintxy(20 + (i / 10) * 40, (i % 10) * +10, i);
    delay(1000);
  }

  closegraph();
}

void outintxy(int x, int y, int value)
{
  char digit_string[20];
  sprintf(digit_string, "%d", value);
  outtextxy(x, y, digit_string);
}
