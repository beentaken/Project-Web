// QuadMesh.cpp
// -- non-inline implementations
// cs200 9/14

#include "QuadMesh.h"


const Point QuadMesh::vertices[4]
  = { Point(1,0), Point(2,2), Point(1,1),  Point(0,2) };


const Mesh::Face QuadMesh::faces[2]
  = { Face(0,1,2), Face(0,2,3) };


const Mesh::Edge QuadMesh::edges[4]
  = { Edge(0,1), Edge(1,2), Edge(2,3), Edge(3,0) };

