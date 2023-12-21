// chr_decoder.c	-*- C -*-
// CHR font decoder.
// Marco Diego Aurélio Mesquita
// Modifications by GG

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum opcode {
  OP_STOP = 0, OP_UNKNOW, OP_MOVETO, OP_LINETO
};

struct stroke {
  int x;
  int y;
  enum opcode op;
};

struct font {
  int capheight;
  int baseheight;
  int decheight;
  struct stroke *glyphs[256];
  int widths[256];
};

// globals

struct font
  *chr_font;

char
fontname[13];

int
  num_glyphs = 0;

// -----

static struct font *decode (FILE *fp)
{
  // Decodes a .CHR font and build its internal tables.
  // Information here:
  // http://www.fileformat.info/format/borland-chr/corion.htm

  char *fontinfo = NULL;
  size_t fontinfolen = 0;
  size_t tmp;

  /* All my small sample of borland fonts start with PK\X08\X08 */
  char magic[4];
  tmp = fread (magic, 4, 1, fp);
  if (magic[0] != 'P' ||
      magic[1] != 'K' ||
      magic[2] != 0x08 ||
      magic[3] != 0x08) {
    fprintf (stderr, "File does not seem to be a valid font.\n");
    return NULL;
  }

  /* Now we scan the file until we find 0x1a */
  while (!feof(fp)) {
    char c = fgetc(fp);
    // TODO: add checks for *alloc
    fontinfo = (char*)realloc(fontinfo, fontinfolen+1);
    if (c == 0x1a){
      fontinfo[fontinfolen] = '\0';
      break;
    } else
      fontinfo[fontinfolen] = c;
    fontinfolen++;
  }

  // printf("Font info is: %s\n", fontinfo);
  free (fontinfo);

  /* We'll now info about font version */
  uint16_t headerlen;
  char fontname[4];
  uint16_t fontlen;
  unsigned char ver_major;
  unsigned char ver_minor;
  unsigned char bgi_ver_major;
  unsigned char bgi_ver_minor;

  tmp = fread (&headerlen, sizeof(uint16_t), 1, fp);
  tmp = fread (&fontname, 1, 4, fp);
  tmp = fread (&fontlen, sizeof(uint16_t), 1, fp);
  ver_major = fgetc (fp);
  // mute "unused variable" compiler warning
  (void) ver_major;
  ver_minor = fgetc (fp);
  (void) ver_minor;
  bgi_ver_major = fgetc (fp);
  (void) bgi_ver_major;
  bgi_ver_minor = fgetc (fp);
  (void) bgi_ver_minor;

  /*
  printf("Header length: %d\n", headerlen);
  printf("Font name: %c%c%c%c\n",
	 fontname[0], fontname[1],
	 fontname[2], fontname[3]);
  printf("Font lenght: %d\n", fontlen);
  printf("Version: %d.%d\n", ver_major, ver_minor);
  printf("Min, bgi version: %d.%d\n", bgi_ver_major, bgi_ver_minor);
   */

  /* Inital header seems padded with zeroes */
  fseek (fp, 0x80, SEEK_SET);

  /* Now some info about offsets */
  char signature = fgetc (fp);
  (void) signature;
  // printf("Signature: %c\n", signature);

  uint16_t nglyphs;
  tmp = fread (&nglyphs, sizeof(uint16_t), 1, fp);
  // printf("Number of glyphs: %d\n", nglyphs);

  char unknown0 = fgetc (fp);
  (void) unknown0;
  // printf("unknown 0: %d\n", (int)unknown0);

  char firstglyph = fgetc (fp);
  (void) firstglyph;
  // printf("First glyph is: %c (%d) \n", firstglyph, firstglyph);

  uint16_t glyphs_offset;
  tmp = fread (&glyphs_offset, sizeof(uint16_t), 1, fp);
  // printf("Glyphs offset: %d\n", glyphs_offset);

  char is_scan = fgetc (fp);
  (void) is_scan;
  // printf("Is scannable: %d\n", (int) is_scan);

  // distance from origin to top of capital
  signed char capheight = fgetc (fp);
  // printf("Capital height: %d\n", (int) capheight);

  // distance from origin to baseline; always 0
  signed char baseheight = fgetc (fp);
  // printf("Base height: %d\n", (int) baseheight);

  // distance from origin to bottom descender
  signed char decheight = fgetc (fp);
  // printf("Descender height: %d\n", (int) decheight);
  
  // exit (0);

  /* Don't know why it has 2 names */
  char fontname2[4];
  tmp = fread (&fontname2, 1, 4, fp);
  /*
  printf("Font name: %c%c%c%c\n",
	 fontname2[0], fontname2[1],
	 fontname2[2], fontname2[3]);
   */

  char unknown1 = fgetc (fp);
  (void) unknown1;
  // printf("unknown 1: %d\n", (int)unknown1);

  /* Let's load the offset table */
  // printf ("Loading offset table...");
  uint16_t *offset_table = (uint16_t*) malloc(nglyphs*sizeof(uint16_t));
  for (int i = 0; i < nglyphs; i++)
    tmp = fread(&offset_table[i], sizeof(uint16_t), 1, fp);
  // printf("done.\n");

  /*
  for(int i = 0; i < nglyphs; i++)
    printf("char %c, offset: 0x%04x\n", (char)(i+firstglyph), (int)offset_table[i]);
   */

  /* Let's load the width table */
  // printf("Loading width table...");
  char *width_table = (char*) calloc(nglyphs, sizeof(char));
  for (int i = 0; i < nglyphs; i++)
    tmp = fread (&width_table[i], sizeof(char), 1, fp);
  // printf("done.\n");

  (void) tmp;

  /*
  for(int i = 0; i < nglyphs; i++)
    printf("char %c, width: %d\n", (char)(i+firstglyph), (int)width_table[i]);
   */

  /* Let's allocate the font */
  struct font *ret = calloc(1, sizeof(struct font));
  ret->capheight = capheight;
  ret->baseheight = baseheight;
  ret->decheight = decheight;
  
  /* What follows now is the list of strokes */
  int i = firstglyph;
  int j = 0;

  // Find real first char
  int real_first = 0;
  while (!offset_table[++real_first + 1]);
  // printf("Real first char is: %d\n", real_first);

  if(real_first != 1)
    i += real_first;

  char b0, b1;

  while ( EOF != fscanf (fp, "%c%c", &b0, &b1) ) {

    int opcode = ((b0&0x80) >> 6) + ((b1&0x80) >> 7);
    int x = ((int)((signed char)((b0&0x7f) << 1)))/2;
    int y = ((int)((signed char)((b1&0x7f) << 1)))/2;
    // printf("Opcode: %d, x = %d, y = %d\n", opcode, x, y);

    j++;

    ret->widths[i] = 0;

    ret->glyphs[i] =
      realloc(ret->glyphs[i], j*sizeof(struct stroke));
    ret->glyphs[i][j - 1].x = x;
    ret->glyphs[i][j - 1].y = y;
    ret->glyphs[i][j - 1].op = opcode;

    if (opcode == OP_STOP) {
      ret->widths[i] = width_table[i - firstglyph];
      // printf("char %d(%c) should have width: %d\n", i, i, ret->widths[i]);
      i++;
      if (i == nglyphs + firstglyph)
	break;
      j = 0;
    } // if

  } // while

  free (offset_table);
  free (width_table);

  return ret;
}

// -----

void display_glyph (struct font *f, int c, float scale_x,
		    float scale_y, int x, int y)
{
  if (!f)
    return;

  if (!f->glyphs[c])
    return;

  int offset_y = scale_y * (f->capheight + f->baseheight) -
    (scale_y - 1) * f->decheight;

  int offset_x = scale_x * (f->capheight + f->baseheight) -
    (scale_x - 1) * f->decheight;

  int oldx, oldy, ch;
  int counter = 0;

  ch = c - ' ' + 1;
  
  if (c > '~')
    printf ("// Glyph %d\n", c);
  else
    printf ("// Glyph '%c'\n", c);
  printf ("#define %s_%d_width %d\n", 
	  fontname, ch, f->widths[c]);
  printf ("static const char %s_%d[] = {\n",
	  fontname, ch);
  
  for (int j = 0; f->glyphs[c][j].op != OP_STOP; j++) {

    struct stroke st = f->glyphs[c][j];
    int dx;
    int dy;

    dx = st.x*scale_x + x;
    dy = -st.y*scale_y + y;

    if (st.op == OP_LINETO) {
      printf ("  %3d,%3d,%3d,%3d,   \n", oldx, oldy, dx, dy);
      counter += 4;
      oldx = dx;
      oldy = dy;
    }
    else {
      oldx = dx;
      oldy = dy;
    }

  } // for

  printf ("};\n");
  printf ("#define %s_%d_size %d\n\n", 
	  fontname, ch, counter);

  num_glyphs++;

} // display_glyph ()

// -----

struct font *load_font (char *filename)
{
  // Loads a .CHR font from disk.

  struct font *ret;
  FILE *fp;

  // !!! the following file is open with "rb"
  // for compatibility with Mingw64

  fp = fopen (filename, "rb");
  if (!fp)
    return NULL;

  ret = decode(fp);

  fclose (fp);
  return ret;
}

// -----

int main (int argc, char *argv[0])
{ 
  int ch;
  
  if (1 == argc) {
    fprintf (stderr, "Usage: %s <FONT>\n", argv[0]);
    exit (1);
  }
  
  chr_font = load_font (argv[1]);
  if (NULL == chr_font) {
    fprintf (stderr, "Invalid font file '%s'\n", argv[1]);
    exit (1);
  }
  
  for (int i = 0; i < 4; i++)
    fontname[i] = tolower (argv[1][i]);
  fontname[4] = '\0';
  
  printf ("// %s.h\n", fontname);
  printf ("// This file is part of SDL_BGI\n");
  printf ("// Decoded from original BGI font %s\n\n", argv[1]);
  
  for (ch = 32; ch < 256; ch++)
    display_glyph (chr_font, ch, 1.0, 1.0,
                 0, chr_font->capheight - chr_font->decheight);
  
  printf ("#define %s_NGLYPHS %d\n\n", fontname, num_glyphs);
  
  printf ("// Font height\n");
  printf ("#define %s_height %d\n\n", 
	  fontname, chr_font->capheight - chr_font->decheight);
  printf ("// Descender height\n");
  printf ("#define %s_desc_height %d\n\n", 
	  fontname, chr_font->decheight);
  
  printf ("// Widths of the glyphs\n");
  printf ("static char %s_width[] = {\n", fontname);
    
  int counter = 0;
  printf ("  ");
  for (int i = 1; i < num_glyphs + 1; i++) {
    printf ("%s_%d_width, ", fontname, i);
    counter ++;
    if (4 == counter) {
      printf ("\n  ");
      counter = 0;
    }
  }
  printf ("\n};\n\n");
  
  printf ("// Number of bytes in each glyph\n");
  printf ("static const int %s_size[] = {\n", fontname);
  
  counter = 0;
  printf ("  ");
  for (int i = 1; i < num_glyphs + 1; i++) {
    printf ("%s_%d_size, ", fontname, i);
    counter ++;
    if (4 == counter) {
      printf ("\n  ");
      counter = 0;
    }
  }
  printf ("\n};\n\n");
  
  printf ("// Pointers to glyph data\n");
  printf ("static const char *%s[] = {\n", fontname);
  
  counter = 0;
  printf ("  ");
  for (int i = 1; i < num_glyphs + 1; i++) {
    printf ("&%s_%d[0], ", fontname, i);
    counter ++;
    if (4 == counter) {
      printf ("\n  ");
      counter = 0;
    }
  }
  printf ("\n};\n\n");
  
  printf ("// --- end of file %s.h\n", fontname);
  
}

// --- end of chr_decoder.c
