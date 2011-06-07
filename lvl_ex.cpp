#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstring>
#include <vector>

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

std::vector <unsigned char*> tiles;

void load_tiles (const char* fname)
{
  FILE* f = fopen (fname, "rb");
  int k = 0;
  while (!feof (f))
    {
      unsigned char* data = new unsigned char [256];
      fread (data, 1, 256, f);
      tiles.push_back (data);
      k++;
    }
  std::cout << k << " tiles loaded" << std::endl;
}

void load_level (FILE* f, int k)
{
  const int lvl_w = 14;
  const int lvl_h = 12;
  unsigned char map [lvl_w*lvl_h];
  fread (map, 1, lvl_w*lvl_h, f);
  bitmap_image bm (lvl_w*16, lvl_h*16);
  const int skp = 10;
  unsigned char skip [skp];
  fread (skip, 1, skp, f);
  for (int y=0; y<lvl_h; y++)
    {
      for (int x=0; x<lvl_w; x++)
	{
	  int px = x*16;
	  int py = y*16;
	  unsigned char* tile = tiles [map [lvl_w*y + x]];
	  for (int i=0; i<16; i++)
	    {
	      for (int j=0; j<16; j++)
		{
		  rgb c = pal [tile [16*i + j]];
		  bm.set_pixel (px + j, py + i, c.r, c.g, c.b);
		}
	    }
	}
    }
  std::ostringstream os;
  os << "level" << k << ".bmp";
  bm.save_image (os.str ().c_str ());
}

int main (int argc, char* argv [])
{
  load_pal (argv [1]);
  load_tiles (argv [2]);

  FILE* f = fopen (argv [3], "rb");
  int k = 0;
  while (!feof (f))
    {
      load_level (f, k++);
    }
  fclose (f);
  std::cout << k << " levels loaded" << std::endl;
  return 0;
}
