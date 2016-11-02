#include "HalfPlane.h"

float dot(const Hcoords& h, const Point& Q)
{
  float result = 0;

  result += h.x * Q.x;
  result += h.y * Q.y;
  result += h.w * Q.w;

  return result;
}

Hcoords HalfPlane(const Vector& n, const Point& C)
{
  Hcoords result(n.x, n.y, 0);

  result.w = -((n.x * C.x) + (n.y * C.y));

  return result;
}

Hcoords HalfPlane(const Point& A, const Point& B, const Point& P)
{
  Hcoords result;
  result.x = (B.y - A.y)/(B.x - A.x);
  result.y = -1;
  result.w = (A.y) - (result.x * A.x);

  if(dot(result, P) > 0)
  {
  	result.x = -result.x;
  	result.y = -result.y;
  	result.w = -result.w;
  }

  return result;
}

Interval ClipSegment(const Hcoords& h, const Point& P, const Point& Q)
{
  Interval result;
  float h_P, h_Q;
  h_P = dot(h, P);
  h_Q = dot(h, Q);

  if(h_P  > 0 && h_Q <= 0)
  {
  	result.bgn = (h_P)/(h_P - h_Q);
  	result.end = 1;
  }
  if(h_P <= 0 && h_Q  > 0)
  {
  	result.bgn = 0;
  	result.end = (h_P)/(h_P - h_Q);
  }
  if(h_P <= 0 && h_Q <= 0)
  {
    result.bgn = 0;
  	result.end = 1;
  }
  if(h_P  > 0 && h_Q  > 0)
  {
    result.bgn = 0;
  	result.end = 0;
  }
  
  return result;

}
