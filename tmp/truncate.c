/*

This program shows a bug in Turbo C 2.01; the first string is not
truncated in TC, but is truncated in SDL_bgi as per original Borland
documentation: "If a string is printed with the default font using
outtext or outtextxy, any part of the string that extends outside the
current viewport is truncated." (Borland Turbo C 2.0 Reference Guide
1988, pag. 261)

*/

#include <graphics.h>

int main (int argc, char *argv[])
{
  int
    gd = DETECT, gm;
  
  initgraph (&gd, &gm, "");
  setbkcolor (WHITE);
  setcolor (RED);
  cleardevice ();
  rectangle (100, 100, 500, 400);
  setviewport (100, 100, 500, 400, 1);
  outtextxy (0, 0, "The text below is NOT truncated:");
  outtextxy (0, 40, "The text below is truncated:");
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  outtextxy (0, 20, "Truncated");
  settextstyle (TRIPLEX_FONT, HORIZ_DIR, 4);
  outtextxy (0, 80, "Truncated");
  
  getch ();
  closegraph ();
  
}

/* --- end of file truncate.c */
