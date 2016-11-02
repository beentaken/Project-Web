// RasterUtilities.h
// -- rasterization functions
// cs200 9/14

#ifndef CS200_RASTERUTILITIES_H
#define CS200_RASTERUTILITIES_H

#include "Raster.h"
#include "Affine.h"


// These functions were already implemented in the previous assignment:
void FillRect(Raster& r, int x, int y, int width, int height);
void DrawRect(Raster& r, int x, int y, int width, int height);
void DrawLine(Raster& r, const Point& P, const Point& Q);

// This function is to be implemented in 'FillTriangle.cpp':
void FillTriangle(Raster& r, const Point& P, const Point& Q, const Point& R);


#endif

