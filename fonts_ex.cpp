#include <iostream>
#include <iomanip>
#include <stdio.h>

int main (int argc, char* argv [])
{
  FILE* f = fopen (argv [1], "rb");
  int k = 0;
  while (!feof (f))
    {
      unsigned char v;
      fread (&v, 1, 1, f);
      unsigned int vi = v & 0xFF;
      std::cout << std::setw (3) << k << " ";
      unsigned int mask [] = { 256, 128, 64, 16, 8, 4, 2, 1};
      for (int i=0; i<8; i++)
	{
	  if ((v & mask [i]) == mask [i])
	    {
	      std::cout << '+';
	    }
	  else
	    {
	      std::cout << ' ';
	    }
	}
      k++;
      std::cout << std::endl;
    }
  return 0;
}
