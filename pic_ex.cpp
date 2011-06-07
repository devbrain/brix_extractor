#include <iostream>
#include <stdio.h>
#include <cstring>
#include "bitmap_image.hpp"

struct rgb
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

rgb pal [256];

int main (int argc, char* argv [])
{
  std::memset (pal, 0, sizeof (pal));
  FILE* f = fopen (argv [1], "rb");

  
  for (int i=0; i<256; i++)
    {
      rgb x;
      fread (&x.r, 1, 1, f);
      fread (&x.g, 1, 1, f);
      fread (&x.b, 1, 1, f);
      x.r = x.r * 4;
      x.g = x.g * 4;
      x.b = x.b * 4;
      pal [i] = x;
    }
  
  unsigned short w;
  unsigned short h;
  
  fread (&w, 1, 2, f);
  fread (&h, 1, 2, f);
  std::cout << "dims of " << argv [1] << " " << w << "x" << h << std::endl;

  bitmap_image bm (w, h);
  for (int y=0; y<h; y++)
    {
      for (int x=0; x<w; x++)
	{
	  unsigned char v;
	  fread (&v, 1, 1, f);
	  bm.set_pixel (x, y, pal[v].r, pal[v].g, pal[v].b);
	}
    }
  fclose (f);
  bm.save_image ("zopa.bmp");
}
