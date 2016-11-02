// Camera.h
// cs200 1/15

#ifndef CS200_CAMERA_H
#define CS200_CAMERA_H

#include "Affine.h"


class Camera {
  public:
    Camera(void);
    Camera(const Point& C, const Vector& up, float W, float H);
    Point Center(void) const;
    Vector Right(void) const;
    Vector Up(void) const;
    float Width(void) const;
    float Height(void) const;
    Camera& MoveRight(float x);
    Camera& MoveUp(float y);
    Camera& Rotate(float t);
    Camera& Zoom(float f);
  private:
    Point center;
    Vector right, up;
    float width, height;
};


Affine CameraToWorld(const Camera& cam);
Affine WorldToCamera(const Camera& cam);
Affine CameraToNDC(const Camera& cam);
Affine NDCToCamera(const Camera& cam);

#endif

