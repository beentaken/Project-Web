#include "Camera.h"
#include "Affine.h"
#define PI 3.14159


Camera::Camera()
{
  Vector R(1, 0);
  Vector U(0, 1);
  right = R;
  up = U;
  width = 2;
  height = 2;
}

Camera::Camera(const Point& C, const Vector& up, float W, float H)
: up(up), width(W), height(H), center(C)
{
  Vector UP(up.x, up.y);
  UP.Normalize();
  
  // The up, width, height, and center, are set in the initialization list
  Point origin(0,0);
  // Constructing right vector by rotating the up vector by 90 degrees
  right = Trans(C - origin) * Rot(PI/2) * Trans(origin - C) * UP;
}


// Getters ---------------------------------------------------------------------
Point Camera::Center(void) const
{
  return center;
}

Vector Camera::Right(void) const
{
  return right;
}

Vector Camera::Up(void) const
{
  return up;
}

float Camera::Width(void) const
{
  return width;
}

float Camera::Height(void) const
{
  return height;
}
//------------------------------------------------------------------------------


// Setters ---------------------------------------------------------------------
Camera& Camera::MoveRight(float x)
{
  Vector RIGHT(right.x, right.y);
  Hcoords CENTER(center.x, center.y, 0);
  
  RIGHT.x = RIGHT.x * x;
  RIGHT.y = RIGHT.x * x;
  CENTER = CENTER + RIGHT;
  
  center.x = CENTER.x;
  center.y = CENTER.y;
  
  return *this;
}

Camera& Camera::MoveUp(float y)
{
  center.y += y;
  
  return *this;
}

Camera& Camera::Rotate(float t)
{
  Point origin(0,0);
  right = Trans(center - origin) * Rot(t) * Trans(origin - center) * right;
  up = Trans(center - origin) * Rot(t) * Trans(origin - center) * up;
  
  return *this;
}

Camera& Camera::Zoom(float f)
{
  width = width*f;
  height = height*f;
  
  return *this;
}
//------------------------------------------------------------------------------


// Transforms ------------------------------------------------------------------
Affine CameraToWorld(const Camera& cam)
{
  Affine CtW(cam.Right(), cam.Up(), cam.Center());
  
  return CtW;
}

Affine WorldToCamera(const Camera& cam)
{
  // Construct CameraToWorld Transform
  Affine CtW = CameraToWorld(cam);
  // Construct the inverse of CameraToWorld
  Affine WtC = Inverse(CtW);
  
  return WtC;
}

Affine CameraToNDC(const Camera& cam)
{
  Affine CtD = Scale(2/cam.Width(), 2/cam.Height());
  
  return CtD;
}

Affine NDCToCamera(const Camera& cam)
{
  Affine DtC = Scale(cam.Width()/2, cam.Height()/2);
  
  return DtC;
}
//------------------------------------------------------------------------------
