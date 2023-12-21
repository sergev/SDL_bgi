#include <graphics.h>

int main (int argc, int *argv[])
{
  SDL_Surface *surface;
  
  initwindow (256, 256);
  setbkcolor (COLOR (240, 240, 240));
  setfillstyle (SOLID_FILL, WHITE);
  cleardevice ();
  fillellipse (128, 128, 110, 110);
  settextstyle (TRIPLEX_FONT, HORIZ_DIR, 9);
  for (int r = 110; r < 180; r++) {
    setcolor (COLOR (r / 2, 30 + r / 2, 60 + r / 2));
    circle (128, 128, r);
  }
  setcolor (COLOR (0, 0, 160));
  outtextxy (42, 40, "SDL");
  outtextxy (43, 40, "SDL");
  outtextxy (42, 41, "SDL");
  outtextxy (43, 41, "SDL");
  setcolor (COLOR (180, 0, 0));
  outtextxy (72, 90, "BGI");
  outtextxy (73, 90, "BGI");
  outtextxy (72, 91, "BGI");
  outtextxy (73, 91, "BGI");
  refresh ();
  surface = SDL_GetWindowSurface (bgi_window);
  SDL_SaveBMP (surface, "icon.bmp");
  
  getch ();
  closegraph ();
}