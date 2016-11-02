#include "HalfPlane.h"
#include "PointContainment.h"
#include "stdio.h"

bool PointInTriangle(const Point& P, const Point& A,
                     const Point& B, const Point& C)
{
  Hcoords A_B = HalfPlane(A, B, C);
  Hcoords B_C = HalfPlane(B, C, A);
  Hcoords C_A = HalfPlane(C, A, B);

  if(dot(A_B, P) < 0)
    return true;
  printf("dot(A_B, P) = %g\n",dot(A_B, P) );
  printf("dot(B_C, P) = %g\n",dot(B_C, P) );
  printf("dot(C_A, P) = %f\n",dot(C_A, P) );

  return false;
}

bool PointInMesh(const Point& P, Mesh& mesh)
{
  int face_count = mesh.FaceCount();
  bool result = false;

  for(int i = 0; i < face_count; i++)
  {
    
    result = PointInTriangle(P, mesh.GetVertex(mesh.GetFace(i).index1),
                                mesh.GetVertex(mesh.GetFace(i).index2),
                                mesh.GetVertex(mesh.GetFace(i).index3));
    printf("Point P -- (%g, %g)\n",P.x,P.y );
    printf("Triangle #%i -- (%g, %g) (%g, %g) (%g, %g)\n", i, mesh.GetVertex(mesh.GetFace(i).index1).x, mesh.GetVertex(mesh.GetFace(i).index1).y, mesh.GetVertex(mesh.GetFace(i).index2).x, mesh.GetVertex(mesh.GetFace(i).index2).y, mesh.GetVertex(mesh.GetFace(i).index3).x, mesh.GetVertex(mesh.GetFace(i).index3).y );
    
    if(result == false)
      printf("false\n");
    else if(result == true)
      printf("true\n");
    if(result == false)
      return false;
  }
  return true;
}