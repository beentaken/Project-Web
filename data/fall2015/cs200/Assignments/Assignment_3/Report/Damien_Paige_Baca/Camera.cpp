#include "Camera.h"
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

// COMMENT:
// -- The passed-in up vector is not necessarily a unit vector,
//    so it should be normalized.
Camera::Camera(const Point& C, const Vector& up, float W, float H)
: up(up), width(W), height(H), center(C)
{
  // The up, width, height, and center, are set in the initialization list
  Point origin(0,0);
  // Constructing right vector by rotating the up vector by 90 degrees
  right = Trans(C - origin) * Rot(PI/2) * Trans(origin - C) * up;
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

// COMMENT:
// -- Camera motion should be relative to the camera's up and right vectors
Camera& Camera::MoveRight(float x)
{
  center.x -= x;
  
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
// COMMENT:
// -- The up and right vectors don't need to be transposed, because they are in
//    camera space. In fact transposing it makes it no longer a unit vector
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
