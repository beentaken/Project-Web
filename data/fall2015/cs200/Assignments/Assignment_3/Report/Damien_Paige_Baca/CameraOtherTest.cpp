// CameraOtherTest.cpp
// cs200 6/15

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Camera.h"
using namespace std;


float frand(float a, float b) {
  return a + (b-a)*float(rand())/float(RAND_MAX);
}


bool near(float a, float b) {
  return Hcoords::Near(a,b);
}


bool near(const Hcoords& u, const Hcoords& v) {
  return Hcoords::Near(abs(u-v),0);
}


int main(void) {
  const int COUNT = 1000;
  srand(unsigned(time(0)));

  bool pass,
       okay = true;

  // default constructor test
  pass = true;
  Camera cam;
  pass = near(cam.Center(),Point(0,0))
         && near(cam.Right(),Vector(1,0))
         && near(cam.Up(),Vector(0,1))
         && near(cam.Width(),2)
         && near(cam.Height(),2);
  if (!pass) {
    cout << "  -- failed default constructor test" << endl;
    okay = false;
  }

  // non-default constructor test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    Camera cam(center,up,width,height);
    up.Normalize();
    Vector right(up.y,-up.x);
    pass = near(cam.Center(),center)
           && near(cam.Right(),right)
           && near(cam.Up(),up)
           && near(cam.Width(),width)
           && near(cam.Height(),height);
  }
  if (!pass) {
    cout << "  -- failed non-default constructor test" << endl;
    okay = false;
  }

  // move right test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    up.Normalize();
    Camera cam(center,up,width,height);
    Vector right(up.y,-up.x);
    float x = frand(-10,10);
    cam.MoveRight(x);
    pass = near(cam.Center(),center+x*cam.Right())
           && near(cam.Right(),right)
           && near(cam.Up(),up)
           && near(cam.Width(),width)
           && near(cam.Height(),height);
  }
  if (!pass) {
    cout << "  -- failed move right test" << endl;
    okay = false;
  }

  // move up test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    up.Normalize();
    Camera cam(center,up,width,height);
    Vector right(up.y,-up.x);
    float y = frand(-10,10);
    cam.MoveUp(y);
    pass = near(cam.Center(),center+y*cam.Up())
           && near(cam.Right(),right)
           && near(cam.Up(),up)
           && near(cam.Width(),width)
           && near(cam.Height(),height);
  }
  if (!pass) {
    cout << "  -- failed move up test" << endl;
    okay = false;
  }

  // rotate test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    up.Normalize();
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    Camera cam(center,up,width,height);
    Vector right(up.y,-up.x);
    const float pi = 4.0f*atan(1.0f);
    float t = 2*pi*frand(-1,1);
    cam.Rotate(t);
    Affine R = Rot(t);
    pass = near(cam.Center(),center)
           && near(cam.Right(),R*right)
           && near(cam.Up(),R*up)
           && near(cam.Width(),width)
           && near(cam.Height(),height);
  }
  if (!pass) {
    cout << "  -- failed rotate test" << endl;
    okay = false;
  }

  // zoom test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    up.Normalize();
    Camera cam(center,up,width,height);
    Vector right(up.y,-up.x);
    float f = frand(0.01f,100);
    cam.Zoom(f);
    pass = near(cam.Center(),center)
           && near(cam.Right(),right)
           && near(cam.Up(),up)
           && near(cam.Width(),f*width)
           && near(cam.Height(),f*height);
  }
  if (!pass) {
    cout << "  -- failed zoom test" << endl;
    okay = false;
  }

  // cam-to-world test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    up.Normalize();
    Camera cam(center,up,width,height);
    Vector right(up.y,-up.x);
    Affine Mc = CameraToWorld(cam);
    pass = near(Mc*Vector(1,0),cam.Right())
           && near(Mc*Vector(0,1),cam.Up())
           && near(Mc*Point(0,0),cam.Center());
  }
  if (!pass) {
    cout << "  -- failed camera-to-world test" << endl;
    okay = false;
  }

  // world-to-cam test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    up.Normalize();
    Camera cam(center,up,width,height);
    Vector right(up.y,-up.x);
    Affine iMc = WorldToCamera(cam);
    pass = near(iMc*cam.Right(),Vector(1,0))
           && near(iMc*cam.Up(),Vector(0,1))
           && near(iMc*cam.Center(),Point(0,0));
  }
  if (!pass) {
    cout << "  -- failed world-to-camera test" << endl;
    okay = false;
  }

  // cam-to-ndc test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    up.Normalize();
    Camera cam(center,up,width,height);
    Vector right(up.y,-up.x);
    Affine N = CameraToNDC(cam);
    pass = near(N*Vector(cam.Width(),0),Vector(2,0))
           && near(N*Vector(0,cam.Height()),Vector(0,2))
           && near(N*Point(0,0),Point(0,0));
  }
  if (!pass) {
    cout << "  -- failed camera-to-ndc test" << endl;
    okay = false;
  }

  // ndc-to-cam test
  pass = true;
  for (int i=0; pass && i < COUNT; ++i) {
    Point center(frand(-10,10),frand(-10,10));
    Vector up(frand(-1,1),frand(-1,1));
    float width = frand(0.01f,100),
          height = frand(0.01f,100);
    up.Normalize();
    Camera cam(center,up,width,height);
    Vector right(up.y,-up.x);
    Affine iN = NDCToCamera(cam);
    pass = near(iN*Vector(2,0),Vector(cam.Width(),0))
           && near(iN*Vector(0,2),Vector(0,cam.Height()))
           && near(iN*Point(0,0),Point(0,0));
  }
  if (!pass) {
    cout << "  -- failed ndc-to-camera test" << endl;
    okay = false;
  }

  if (okay)
    cout << "  -- passed all tests" << endl;

  return 0;
}

