// RasterUtilities.h
// -- rasterization functions
// cs200 9/14

#ifndef CS200_RASTERUTILITIES_H
#define CS200_RASTERUTILITIES_H

#include "Raster.h"
#include "Affine.h"


// These 2 functions are to be implemented in 'RasterUtilities.cpp':
void FillRect(Raster& r, int x, int y, int width, int height);
void DrawRect(Raster& r, int x, int y, int width, int height);

// This function is to be implemented in 'DrawLine.cpp':
void DrawLine(Raster& r, const Point& P, const Point& Q);

// *DO NOT* implement this function in this assignment:
void FillTriangle(Raster& r, const Point& P, const Point& Q, const Point& R);


#endif

