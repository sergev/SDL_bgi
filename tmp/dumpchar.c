/* dumpchar.c */

/* This program dumps the bytes that make up a character */

#include <stdio.h>
#include <graphics.h>

FILE
  *dump;

void dumpchar (unsigned char ch)
{
  int
    x, y;
  char
    str[2];
  unsigned char
    byte;

  str[0] = ch;
  str[1] = '\0';
  cleardevice ();
  outtextxy (0, 0, str);

  for (y = 0; y < 8; y++) {
    byte = 0;
    for (x = 0; x < 8; x++)
      if (RED == getpixel (x, y))
	byte |= (1 << (7 - x));
    fprintf (dump, "0x%02x,", byte & 0xff);
  }
  fprintf (dump, " // %02d 0x%02x '%c'\n", ch, ch & 0xff, ch);
}

/* ----- */

int main (int argc, char *argv[])
{
  int
    x, y,
    gd = DETECT, gm,
    byte;

  dump = fopen ("vgadump.dat", "w");

  initgraph (&gd, &gm, "");
  setcolor (RED);
  setbkcolor (BLACK);

  for (byte = 0; byte < 256; byte++)
    dumpchar ((unsigned char) byte);

  closegraph ();
  fclose (dump);
  
}

/* --- end of file dumpchar.c */
