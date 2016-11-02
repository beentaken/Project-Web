// cs200_h3_driver.cpp
// -- world-to-camera, normalized device coordinates
// jsh 9/14

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <GL/glut.h>
#include "Affine.h"
#include "Camera.h"
using namespace std;


namespace {

  // global data (ugh)
  int width = 700, height = 500;
  const char *name = "CS200 Assignment #3 [use 1,2,w,a,s,d,z,x,_ keys]";
  int time_last;

  // unit square, centered at origin
  const int vertex_count = 4;
  const Point vertices[vertex_count]
      = { Point(0.5f,0.5f), Point(-0.5f,0.5f),
          Point(-0.5f,-0.5f), Point(0.5f,-0.5f) };
  Point temp_verts[vertex_count];

  const int face_count = 2;
  const struct { int index1, index2, index3; }
      faces[face_count] = { {0,1,2}, {0,2,3} };

  const int edge_count = 4;
  const struct { int index1, index2; }
      edges[edge_count] = { 0,1, 1,2, 2,3, 3,0 };

  // buildings
  const int building_count = 200;
  Affine buildings[building_count];
  struct color { float r, g, b; };
  color building_colors[building_count];

  // rotating square
  Affine square = Scale(4);
  const float pi = 4.0f*atan(1.0f),
              rate = 2*pi/5.0f;
  color square_clr = { 0,0,1 };

  // cameras 0 = stationary, 1 = moving
  Camera camera[2];

  // viewing mode
  bool moving_mode = false;
  float distance_increment = 0.05f,
        angle_increment = 2*pi/100.0f,
        zoom_increment = 0.95f;


  float frand(float a=0, float b=1) {
    return a + (b-a)*float(rand())/float(RAND_MAX);
  }


  // draw rectangle
  void DrawObject(const Affine& B, const color& clr) {
    for (int i=0; i < vertex_count; ++i)
      temp_verts[i] = B * vertices[i];
    glColor3f(clr.r,clr.g,clr.b);
    glBegin(GL_TRIANGLES);
    for (int k=0; k < face_count; ++k) {
      glVertex2f(temp_verts[faces[k].index1].x,
                 temp_verts[faces[k].index1].y);
      glVertex2f(temp_verts[faces[k].index2].x,
                 temp_verts[faces[k].index2].y);
      glVertex2f(temp_verts[faces[k].index3].x,
                 temp_verts[faces[k].index3].y);
    }
    glEnd();
    glColor3d(0,0,0);
    glBegin(GL_LINES);
    for (int k=0; k < edge_count; ++k) {
      glVertex2f(temp_verts[edges[k].index1].x,
                 temp_verts[edges[k].index1].y);
      glVertex2f(temp_verts[edges[k].index2].x,
                 temp_verts[edges[k].index2].y);
    }
    glEnd();
  }

}


void Init(void) {
  srand(unsigned(time(0)));
  time_last = glutGet(GLUT_ELAPSED_TIME);
  for (int i=0; i < building_count; ++i) {
    float w = frand(0.5f,2),
          h = frand(0,5);
    buildings[i] = Trans(Vector(frand(-50,50),0.5f*h)) * Scale(w,h);
  }
  float *clr = &(building_colors[0].r);
  for (int i=0; i < 3*building_count; ++i)
    clr[i] = frand();
  float aspect = float(width)/float(height);
  camera[0] = Camera(Point(0,0),Vector(0,1),aspect*10,10);
  camera[1] = Camera(Point(0,0),Vector(0,1),aspect*5,5);
}


void Draw(void) {
  // compute the time elapsed since the last call to 'Draw' (in seconds)
  int t = glutGet(GLUT_ELAPSED_TIME);
  double dt = 0.001*(t-time_last);
  time_last = t;

  // clear the screen
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);

  // world-to-NDC transform
  int icam = moving_mode ? 1 : 0;
  Affine  WorldToNDC = CameraToNDC(camera[icam])
                       * WorldToCamera(camera[icam]);

  // draw the buildings
  for (int n=0; n < building_count; ++n) {
    Affine B = WorldToNDC * buildings[n];
    DrawObject(B,building_colors[n]);
  }

  // draw the rotating square
  square = Rot(rate*dt) * square;
  Affine B = WorldToNDC * square;
  DrawObject(B,square_clr);

  // draw camera rectangle?
  if (!moving_mode) {
    glColor3f(1,0,0);
    Affine B = WorldToNDC
               * CameraToWorld(camera[1])
               * NDCToCamera(camera[1])
               * Scale(2,2);
    for (int i=0; i < vertex_count; ++i)
      temp_verts[i] = B * vertices[i];
    glBegin(GL_LINES);
    for (int k=0; k < edge_count; ++k) {
      glVertex2f(temp_verts[edges[k].index1].x,
                 temp_verts[edges[k].index1].y);
      glVertex2f(temp_verts[edges[k].index2].x,
                 temp_verts[edges[k].index2].y);
    }
    glEnd();
  }

  glutSwapBuffers();
}


void KeyPressed(unsigned char c, int, int) {
  switch (c) {
    case '\x1b':
      exit(0);
    case 'd':
      camera[1].MoveRight(distance_increment*camera[1].Height());
      break;
    case 'a':
      camera[1].MoveRight(-distance_increment*camera[1].Height());
      break;
    case 'w':
      camera[1].MoveUp(distance_increment*camera[1].Height());
      break;
    case 'x':
      camera[1].MoveUp(-distance_increment*camera[1].Height());
      break;
    case 's':
      camera[1].Rotate(angle_increment);
      break;
    case '1':
      camera[1].Zoom(zoom_increment);
      break;
    case '2':
      camera[1].Zoom(1.0f/zoom_increment);
      break;
    case ' ':
      moving_mode = !moving_mode;
      break;
    default:
      camera[1] = Camera(camera[0].Center(),camera[0].Up(),
                         0.5f*camera[0].Width(),0.5f*camera[0].Height());
  }
}


void Resized(int W, int H) {
  glViewport(0,0,W,H);
  float ratio = float(W)/float(H);
  for (int i=0; i < 2; ++i)
    camera[i] = Camera(camera[i].Center(),camera[i].Up(),
                       ratio*camera[i].Height(),camera[i].Height());
}


void Loop(void) {
  glutPostRedisplay();
}


int main(int argc, char *argv[]) {
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(width,height);
  glutCreateWindow(name);
  glutDisplayFunc(Draw);
  glutKeyboardFunc(KeyPressed);
  glutReshapeFunc(Resized);
  glutIdleFunc(Loop);
  Init();
  glutMainLoop();
  return 0;
}

