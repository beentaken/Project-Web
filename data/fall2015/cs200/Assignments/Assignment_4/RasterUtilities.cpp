#include "RasterUtilities.h"

void FillRect(Raster& r, int x, int y, int width, int height)
{
  r.GotoPoint(x, y);
  r.WritePixel();
  int i, j;
  int Flag = 0;

  for(j = 0; j < height; j++)
  {
	// From i to width - 1 because we start this loop with first pixel set
	if(Flag == 0)
      for(i = 0; i < width - 1; i++)
      {
        r.IncrementX();
        r.WritePixel();
      } 
	  
	// From i to width - 1 because we start this loop with first pixel set
	if(Flag == 1)
      for(i = 0; i < width - 1; i++)
      {
        r.DecrementX();
        r.WritePixel();
      }
	
	if((j + 1) == height)
	  break;
  
  	Flag = !Flag;
    r.IncrementY();
	r.WritePixel();
  }
}

void DrawRect(Raster& r, int x, int y, int width, int height)
{
  r.GotoPoint(x, y);
  r.WritePixel();
  int flip = 0;
  int i;
	
  while(true)
  { 
    for(i = 0; i < width - 1; i++)
    {
      if(flip)
		r.DecrementX();
	  else
	    r.IncrementX();
	  r.WritePixel();
    }
    
    for(i = 0; i < height - 1; i++)
    {
	  if(flip)
		r.DecrementY();
	  else
	    r.IncrementY();
	  r.WritePixel();
    }
	
	if(flip)
      break;
  
    flip = true;
  }
}
