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

void load_pal (const char* fname)
{
  FILE* f = fopen (fname, "rb");
  std::memset (pal, 0, sizeof (pal));
  int offs = 0;
  int k = offs;
  while (!feof (f))
    {
      rgb x;
      fread (&x.r, 1, 1, f);
      fread (&x.g, 1, 1, f);
      fread (&x.b, 1, 1, f);
      x.r = x.r * 4;
      x.g = x.g * 4;
      x.b = x.b * 4;
      std::cout <<  ((int) (x.r) & 0xFF) << ' ' 
		<<  ((int) (x.g) & 0xFF) << ' ' 
		<<  ((int) (x.b) & 0xFF) << std::endl;
      pal [k++] = x;
    }
  std::cout << k << " Colors loaded from " << fname << std::endl;
  fclose (f);
}


int main (int argc, char* argv [])
{
  load_pal (argv [1]);
  
  bitmap_image bm (256, 256);
  FILE* f = fopen (argv [2], "rb");
  int k = 0;
  while (!feof (f))
    {
      unsigned char data [256];
      fread (data, 1, 256, f);
      int py = k / 16;
      int px = k % 16;
      int imx = px*16;
      int imy = py*16;
      for (int i = 0; i<16; i++)
	{
	  for (int j = 0; j<16; j++)
	    {
	      rgb c = pal [ data [16*i + j] ];
	      bm.set_pixel (imx + j, imy + i, c.r, c.g, c.b);
	    }
	}
      k++;
    }
  fclose (f);
  std::cout << k << " tiles loaded from " << argv [2] << std::endl;
  bm.save_image ("zopa.bmp");
}
