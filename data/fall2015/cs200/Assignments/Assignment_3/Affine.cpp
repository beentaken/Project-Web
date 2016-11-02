//******************************************************************************
// Name       :Damien Baca
// Assignment :Assignment #1
// Course     :CS200
// Term       :Fall 2015
//
// Description:
//   Implements a basic transform library.
//******************************************************************************
#include "Affine.h"

Hcoords::Hcoords(void)
{
  x = 0;
  y = 0;
  w = 0;
}

Hcoords::Hcoords(float X, float Y, float W)
{
  x = X;
  y = Y;
  w = W;
}

Point::Point(void)
{
  x = 0;
  y = 0;
  w = 1;
}

Point::Point(float X, float Y)
{
  x = X;
  y = Y;
  w = 1;
}

Vector::Vector(void)
{
  x = 0;
  y = 0;
  w = 0;
}

Vector::Vector(float X, float Y)
{
  x = X;
  y = Y;
  w = 0;
}

bool Vector::Normalize(void)
{
  float magnitude;
  
  if(y == 0 && x == 0) //Checking for a zero vector
    return false;
  
  magnitude = sqrt((x * x) +
                   (y * y) +
                   (w * w));
    
  x = x / magnitude;
  y = y / magnitude;
  w = w / magnitude;
  
  return true;
}

Affine::Affine(void)
{
  row[0][0] = 0;
  row[0][1] = 0;
  row[0][2] = 0;
  
  row[1][0] = 0;
  row[1][1] = 0;
  row[1][2] = 0;
  
  row[2][0] = 0;
  row[2][1] = 0;
  row[2][2] = 1;
  
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Point& disp)
{
  row[0][0] = Lx.x;
  row[0][1] = Ly.x;
  row[0][2] = disp.x;
  
  row[1][0] = Lx.y;
  row[1][1] = Ly.y;
  row[1][2] = disp.y;
  
  row[2][0] = Lx.w;
  row[2][1] = Ly.w;
  row[2][2] = disp.w;
  
}
Hcoords operator+(const Hcoords& u, const Hcoords& v)
{
  Hcoords Result;
  
  Result.x = u.x + v.x;
  Result.y = u.y + v.y;
  Result.w = u.w + v.w;
  
  return Result;
}

Hcoords operator-(const Hcoords& u, const Hcoords& v)
{
	Hcoords Result;
  
  Result.x = u.x - v.x;
  Result.y = u.y - v.y;
  Result.w = u.w - v.w;
  
  return Result;
}

Hcoords operator-(const Hcoords& v)
{
  Hcoords Result;
  
  Result.x = v.x - (2 * v.x);
  Result.y = v.y - (2 * v.y);
  Result.w = v.w - (2 * v.w);
  
  return Result;
}

Hcoords operator*(float r, const Hcoords& v)
{
	Hcoords Result;
  
  Result.x = r * v.x;
  Result.y = r * v.y;
  Result.w = r * v.w;
  
  return Result;
}

Hcoords operator*(const Affine& A, const Hcoords& v)
{
  Hcoords Result;
  
  Result.x = (A[0][0] * v.x) +
             (A[0][1] * v.y) +
             (A[0][2] * v.w);
             
  Result.y = (A[1][0] * v.x) +
             (A[1][1] * v.y) +
             (A[1][2] * v.w);
             
	Result.w = (A[2][0] * v.x) +
             (A[2][1] * v.y) +
             (A[2][2] * v.w);

  return Result;
}

Affine operator*(const Affine& A, const Affine& B)
{
  Affine Result;
  
  Result[0][0] = (A[0][0] * B[0][0]) +
                 (A[0][1] * B[1][0]) +
                 (A[0][2] * B[2][0]);
                           
  Result[0][1] = (A[0][0] * B[0][1]) +
                 (A[0][1] * B[1][1]) +
                 (A[0][2] * B[2][1]);  
                          
  Result[0][2] = (A[0][0] * B[0][2]) +
                 (A[0][1] * B[1][2]) +
                 (A[0][2] * B[2][2]);
                    
  Result[1][0] = (A[1][0] * B[0][0]) +
                 (A[1][1] * B[1][0]) +
                 (A[1][2] * B[2][0]); 
                           
  Result[1][1] = (A[1][0] * B[0][1]) +
                 (A[1][1] * B[1][1]) +
                 (A[1][2] * B[2][1]);
                           
  Result[1][2] = (A[1][0] * B[0][2]) +
                 (A[1][1] * B[1][2]) +
                 (A[1][2] * B[2][2]);
                    
  Result[2][0] = (A[2][0] * B[0][0]) +
                 (A[2][1] * B[1][0]) +
                 (A[2][2] * B[2][0]);
                      
  Result[2][1] = (A[2][0] * B[0][1]) +
                 (A[2][1] * B[1][1]) +
                 (A[2][2] * B[2][1]);
                      
  Result[2][2] = (A[2][0] * B[0][2]) +
                 (A[2][1] * B[1][2]) +
                 (A[2][2] * B[2][2]);
  
	return Result;
}

float dot(const Vector& u, const Vector& v)
{
  float Result;
  
  Result = (u.x * v.x) +
           (u.y * v.y) +
           (u.w * v.w);
           
  return Result;
}

float abs(const Vector& v)
{
  float Length;
  
  Length = sqrt((v.x * v.x) +
                (v.y * v.y) +
                (v.w * v.w));

  return Length;
}

Affine Rot(float t)
{
  Affine Result;
  
  Result[0][0] = cos(t);
  Result[0][1] = -(sin(t));
  Result[0][2] = 0;

  Result[1][0] = sin(t);
  Result[1][1] = cos(t);
  Result[1][2] = 0;

  Result[2][0] = 0;
  Result[2][1] = 0;
  Result[2][2] = 1;
  
  return Result;
}

Affine Trans(const Vector& v)
{
	Affine Result;
  
  Result[0][0] = 1;
  Result[0][1] = 0;
  Result[0][2] = v.x;
  
  Result[1][0] = 0;
  Result[1][1] = 1;
  Result[1][2] = v.y;
  
  Result[2][0] = 0;
  Result[2][1] = 0;
  Result[2][2] = 1;
  
  return Result;
}

Affine Scale(float r)
{
	Affine Result;
  
  Result[0][0] = r;
  Result[0][1] = 0;
  Result[0][2] = 0;
  
  Result[1][0] = 0;
  Result[1][1] = r;
  Result[1][2] = 0;
  
  Result[2][0] = 0;
  Result[2][1] = 0;
  Result[2][2] = 1;
  
  return Result;
}

Affine Scale(float rx, float ry)
{
	Affine Result;
  
  Result[0][0] = rx;
  Result[0][1] = 0;
  Result[0][2] = 0;
  
  Result[1][0] = 0;
  Result[1][1] = ry;
  Result[1][2] = 0;
  
  Result[2][0] = 0;
  Result[2][1] = 0;
  Result[2][2] = 1;
  
  return Result;
}