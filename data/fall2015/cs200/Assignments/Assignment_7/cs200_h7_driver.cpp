// cs200_h7_driver.cpp
// -- test of point containment functions
// cs200 10/15
//
// Note: you will need to compile/link this driver with
//       'Affine.cpp', 'Inverse.cpp', 'QuadMesh.cpp',
//       'HalfPlane.cpp', and 'PointContainment.cpp'.

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <GL/glut.h>
#include "PointContainment.h"
#include "QuadMesh.h"
#include "stdio.h"
using namespace std;


namespace {

  // global data (ugh)
  int width = 600, height = 600;
  const char *name = "Point Containment Test";
  int time_last;

  Point center;     // object center
  Vector offset;    // offset from mouse click point to object center
  Affine ScrToDev;  // screen to world/device coordinates
  bool dragging;    // is object being dragged?
  QuadMesh quad;

}


void Init(void) {
  time_last = glutGet(GLUT_ELAPSED_TIME);
  center = Point(0,0);
  ScrToDev = Scale(2.0f/width,-2.0f/height)
             * Trans(Vector(-0.5f*width,-0.5f*height));
  dragging = false;
}


void Draw(void) {
  // compute the time elapsed since the last call to 'Draw' (in seconds)
  int t = glutGet(GLUT_ELAPSED_TIME);
  double dt = 0.001*(t-time_last);
  time_last = t;

  // clear the screen
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);

  // draw figure
  vector<Point> temp_verts;
  Point O(0,0);
  Affine ObjToDev = Trans(center-O)
                    * Scale(1.0f/quad.Dimensions().x)
                    * Trans(O-quad.Center());
  for (int i=0; i < quad.VertexCount(); ++i)
    temp_verts.push_back(ObjToDev * quad.GetVertex(i));

  glColor3f(0,1,0);
  glBegin(GL_TRIANGLES);
  for (int i=0; i < quad.FaceCount(); ++i) {
    Mesh::Face face = quad.GetFace(i);
    glVertex2f(temp_verts[face.index1].x,
               temp_verts[face.index1].y);
    glVertex2f(temp_verts[face.index2].x,
               temp_verts[face.index2].y);
    glVertex2f(temp_verts[face.index3].x,
               temp_verts[face.index3].y);
  }
  glEnd();

  glColor3f(0,0,0);
  glBegin(GL_LINES);
  for(int i=0; i < quad.EdgeCount(); ++i) {
    Mesh::Edge edge = quad.GetEdge(i);
    glVertex2f(temp_verts[edge.index1].x,
               temp_verts[edge.index1].y);
    glVertex2f(temp_verts[edge.index2].x,
               temp_verts[edge.index2].y);
  }
  glEnd();

  glutSwapBuffers();
}


void MouseButtonPressed(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    
    // screen to object transformation
    Point O(0,0);
    Affine ObjToDev = Trans(center-O)
                      * Scale(1.0f/quad.Dimensions().x)
                      * Trans(O-quad.Center()),
           DevToObj = Inverse(ObjToDev);
    // mouse click point in device/world coordinates
    Point mouse_start = ScrToDev * Point(x,y);
    // offset from mouse click point to object center
    offset = center - mouse_start;
    // convert mouse click point to object coordinates & test containment
    printf("Mouse Pos (%g, %g)\n",(DevToObj*mouse_start).x,(DevToObj*mouse_start).y);
    dragging = PointInMesh(DevToObj*mouse_start,quad);
    
  }
  else if (state == GLUT_UP) {
    dragging = false;
  }
}


void MouseDragged(int x, int y) {
  if (dragging) {
    // compute new object center
    Point mouse_end = ScrToDev * Point(x,y);
    center = mouse_end + offset;
  }
}


void KeyPressed(unsigned char c, int x, int y) {
  switch (c) {
    case '\x1b':
      exit(0);
  }
}


void Resized(int W, int H) {
  glViewport(0,0,W,H);
  width = W;
  height = H;
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
  glutMouseFunc(MouseButtonPressed);
  glutMotionFunc(MouseDragged);
  glutReshapeFunc(Resized);
  glutIdleFunc(Loop);
  Init();
  glutMainLoop();
  return 0;
}

