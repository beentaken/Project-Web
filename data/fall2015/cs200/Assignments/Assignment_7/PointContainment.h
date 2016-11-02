// PointContainment.h
// -- point containment functions
// cs200 10/15

#ifndef CS200_POINTCONTAINMENT_H
#define CS200_POINTCONTAINMENT_H

#include "Affine.h"
#include "Mesh.h"


bool PointInTriangle(const Point& P, const Point& A,
                     const Point& B, const Point& C);

bool PointInMesh(const Point& P, Mesh& mesh);


#endif

