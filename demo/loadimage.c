/* loadimage.c  -*- C -*-
 * 
 * To compile:
 * gcc -o loadimage loadimage.c -lSDL_bgi -lSDL2 -lSDL2_image
 * 
 * Shows how to load images using IMG_Load(), provided
 * by SDL_image.
 * 
 * By Guido Gonzato, January 2022.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#include <graphics.h>
#include <SDL2/SDL_image.h>

void loadimagefile (char *filename, int x1, int y1, int x2, int y2)
{
  // Reads a graphics file and displays it immediately at (x1, y1 ).

  SDL_Surface
    *image_surface,
    *tmp_surface;
  
  SDL_Rect
    src_rect,
    dest_rect;

  // load image file to surface
  image_surface = IMG_Load (filename);
  
  if (NULL == image_surface) {
    SDL_Log ("IMG_Load () error: %s\n", IMG_GetError ());
    showerrorbox ("IMG_Load () error");
    return;
  }

  // source rect, position and size
  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.w = image_surface->w;
  src_rect.h = image_surface->h;

  // destination rect, position
  dest_rect.x = x1;
  dest_rect.y = y1;

  if (0 == x2 || 0 == y2) { // keep original size
    dest_rect.w = src_rect.w;
    dest_rect.h = src_rect.h;
  }
  else { // change size
    dest_rect.w = x2 - x1 + 1;
    dest_rect.h = y2 - y1 + 1;
  }

  // get SDL surface from current window
  tmp_surface = SDL_GetWindowSurface (bgi_window);

  // blit bitmap surface to current surface
  SDL_BlitScaled (image_surface, &src_rect,
                  tmp_surface, &dest_rect);

  copysurface (tmp_surface, dest_rect.x, dest_rect.y,
               dest_rect.x + dest_rect.w, dest_rect.y + dest_rect.h);

  refresh ();
  SDL_FreeSurface (image_surface);
  SDL_FreeSurface (tmp_surface);

} // loadimagefile ()

// -----

int main (int argc, char *argv[])
{

  // Load an image in a window of the same size.

  SDL_Surface
    *image_surface;

  if (1 == argc) {
    fprintf (stderr, "Usage: %s <filename>\n", argv[0]);
    fprintf (stderr, 
	     "<filename> can be any image type supported by SDL2_image.\n");
    return 1;
  }

  IMG_Init (IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);
  
  // load the image to get its size
  image_surface = IMG_Load (argv[1]);
  
  // init a window as large as the image
  initwindow (image_surface->w, image_surface->h);
  SDL_FreeSurface (image_surface);
  
  // load the image
  loadimagefile (argv[1], 0, 0, 0, 0);
  getch ();
  
  IMG_Quit ();
  closegraph ();
  
  return 0;

}

// ----- end of file loadimage.c
