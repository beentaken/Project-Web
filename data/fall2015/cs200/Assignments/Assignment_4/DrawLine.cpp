//******************************************************************************
// Name       :Damien Baca
// Assignment :Assignment #4
// Course     :CS200
// Term       :Fall 2015
//
// Description:
//   Line Drawing Algorithm
//******************************************************************************
#include "RasterUtilities.h"
int round(float);

void DrawLine(Raster& r, const Point& P, const Point& Q)
{
	float M = (Q.y - P.y)/(Q.x - P.x);
	
	// Section 1
	if(abs(M) <= 1 && (Q.x - P.x) > 0)
	{
        int imin = round(P.x);
		int imax = round(Q.x);
		float y = P.y;
		int j;
		
		for(int i = imin; i <= imax; i++)
		{
			j = round(y);
	        r.GotoPoint(i, j);
			r.WritePixel();
			y += M;
		}
	} 
	
	// Section 2
	else if(abs(M) > 1 && (Q.y - P.y) < 0)
	{
		int jmin = round(Q.y);
		int jmax = round(P.y);
		float x = P.x;
		int i;
		
		for(int j = jmax; jmin <= j; j--)
		{
		    i = round(x);
	        r.GotoPoint(i, j);
			r.WritePixel();
			x -= (1/M);
		}
	}
	
	// Section 3
	else if(abs(M) <= 1 && (Q.x - P.x) < 0)
	{
		int imin = round(Q.x);
		int imax = round(P.x);
		float y = P.y;
		int j;
		
		for(int i = imax; imin <= i; i-- )
		{
		    j = round(y);
	        r.GotoPoint(i, j);
			r.WritePixel();
			y -= M;
		}
		
	}
	
	// Section 4
	else if(abs(M) > 1 && (Q.y - P.y) > 0)
	{
		int jmin = round(P.y);
		int jmax = round(Q.y);
		float x = P.x;
		int i;
		
		for(int j = jmin; j <= jmax; j++)
		{
		    i = round(x);
	        r.GotoPoint(i, j);
			r.WritePixel();
			x += (1/M);
		}
	}
}

int round(float u)
{
	int r = static_cast<int>(u + 0.5);
	return r;
}
