// QuadMesh.h
// -- simple quadrilateral
// cs200 9/14

#ifndef CS200_QUADMESH_H
#define CS200_QUADMESH_H

#include "Mesh.h"
#include "Affine.h"


class QuadMesh : public Mesh {
  public:
    int VertexCount(void);
    Point GetVertex(int i);
    Vector Dimensions(void);
    float Height(void);
    Point Center(void);
    int FaceCount(void);
    Face GetFace(int i);
    int EdgeCount(void);
    Edge GetEdge(int i);
  private:
    static const Point vertices[4];
    static const Face faces[2];
    static const Edge edges[4];
};


inline
int QuadMesh::VertexCount(void) {
  return 4;
}


inline
Point QuadMesh::GetVertex(int i) {
  return vertices[i];
}


inline
Vector QuadMesh::Dimensions(void) {
  return Vector(2,2);
}


inline
Point QuadMesh::Center(void) {
  return Point(1,1);
}


inline
int QuadMesh::FaceCount(void) {
  return 2;
}


inline
Mesh::Face QuadMesh::GetFace(int i) {
  return faces[i];
}


inline
int QuadMesh::EdgeCount(void) {
  return 4;
}


inline
Mesh::Edge QuadMesh::GetEdge(int i) {
  return edges[i];
}


#endif

