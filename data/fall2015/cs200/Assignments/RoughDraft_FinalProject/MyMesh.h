//******************************************************************************
// Name       : Damien Baca
// Assignment : Final Project First Draft
// Course     : CS200
// Term       : Fall 2015
//
// Description:
//   My created mesh that is used in the project(source)
//******************************************************************************
#include "Mesh.h"
#include "Affine.h"

class MyMesh : public Mesh
{
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
    static const Point vertices[8];
    static const Face faces[6];
    static const Edge edges[13];
};


inline int MyMesh::VertexCount(void)
{
  return 8;
}


inline Point MyMesh::GetVertex(int i) 
{
  return vertices[i];
}


inline Vector MyMesh::Dimensions(void) 
{
  return Vector(8,8);
}


inline Point MyMesh::Center(void) 
{
  return Point(0,0);
}


inline int MyMesh::FaceCount(void) 
{
  return 6;
}


inline Mesh::Face MyMesh::GetFace(int i) 
{
  return faces[i];
}


inline int MyMesh::EdgeCount(void) 
{
  return 13;
}


inline Mesh::Edge MyMesh::GetEdge(int i) 
{
  return edges[i];
}


