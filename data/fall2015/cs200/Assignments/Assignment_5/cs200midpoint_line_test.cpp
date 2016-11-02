// cs200midpoint_line_test.cpp
// -- test harness for midpoint line drawing algorithm
// cs200 10/14
//
// Note: this must be compiled/linked with
//         'Affine', 'Inverse', and 'RasterUtilities'
//       (in addition to 'DrawLineMidpoint').

#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include "Affine.h"
#include "Raster.h"
#include "RasterUtilities.h"
#include "DrawLineMidpoint.h"
using namespace std;


namespace {

  // global data (ugh)
  int width = 500, height = 500;
  const char *name = "Midpoint Line Test";

  int cell_count_y = 30;
  int mouse_bgn_x, mouse_bgn_y,
      mouse_end_x, mouse_end_y;
  Raster::byte *grid;

}


void Init(void) {
  grid = new Raster::byte[3*width*height];
  // hopefully this is large enough to handle most window resizes ...
}


int SnapClamp(float x, int min, int max) {
  int n = int(x + 0.5f);
  n = (n < min) ? min : n;
  n = (n > max) ? max : n;
  return n;
}


void Draw(void) {
  // clear the screen
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);

  // grid geometry
  float ratio = float(width)/float(height);
  int cell_count_x = int(ratio*cell_count_y);

  // screen to NDC transform
  Point ScreenCenter(0.5f*width,0.5f*height),
        GridCenter(0.5f*cell_count_x,0.5f*cell_count_y),
        O(0,0);
  Affine N = Scale(2.0f/width,-2.0/height) * Trans(O-ScreenCenter);

  // screen to grid transform
  Affine G = Trans(GridCenter-O)
             * Scale(float(cell_count_x+1)/float(width),
                     -float(cell_count_y+1)/float(height))
             * Trans(O-ScreenCenter),
         iG = Inverse(G);

  // grid to NDC transform
  Affine NG = N * iG;

  // draw grid
  glColor3d(0,0,0);
  for (int i=0; i <= cell_count_y; ++i) {
    glBegin(GL_LINES);
    Point P = O + i*Vector(0,1),
          Q = P + cell_count_x*Vector(1,0);
    P = NG*P;
    Q = NG*Q;
    glVertex2f(P.x,P.y);
    glVertex2f(Q.x,Q.y);
    glEnd();
  }
  for (int j=0; j <= cell_count_x; ++j) {
    glBegin(GL_LINES);
    Point P = O + j*Vector(1,0),
          Q = P + cell_count_y*Vector(0,1);
    P = NG*P;
    Q = NG*Q;
    glVertex2f(P.x,P.y);
    glVertex2f(Q.x,Q.y);
    glEnd();
  }

  // draw actual line (snap and clamp to grid)
  glColor3d(0,0,1);
  Point  A = G * Point(mouse_bgn_x,mouse_bgn_y);
  A.x = SnapClamp(A.x,0,cell_count_x);
  A.y = SnapClamp(A.y,0,cell_count_y);
  Point P = NG*A;
  Point B = G * Point(mouse_end_x,mouse_end_y);
  B.x = SnapClamp(B.x,0,cell_count_x);
  B.y = SnapClamp(B.y,0,cell_count_y);
  Point Q = NG*B;
  glBegin(GL_LINES);
  glVertex2f(P.x,P.y);
  glVertex2f(Q.x,Q.y);
  glEnd();

  // draw pixels on grid
  Vector u(0.4f,0),
         v(0,0.4f);
  int grid_width = cell_count_x + 1,
      grid_height = cell_count_y + 1,
      stride = 3*grid_width;
  Raster raster(grid,grid_width,grid_height,stride);
  FillRect(raster,0,0,grid_width,grid_height);
  raster.SetColor(1,1,1);
  DrawLineMidpoint(raster,int(A.x),int(A.y),int(B.x),int(B.y));
  for (int i=0; i < grid_height; ++i)
    for (int j=0; j < grid_width; ++j)
      if (grid[stride*i+3*j] != 0) {
        Point V1(j-0.2f,i-0.2f),
              V2 = V1 + u,
              V3 = V2 + v,
              V4 = V1 + v;
        V1 = NG*V1;
        V2 = NG*V2;
        V3 = NG*V3;
        V4 = NG*V4;
        glColor3d(1,0,0);
        glBegin(GL_POLYGON);
        glVertex2f(V1.x,V1.y);
        glVertex2f(V2.x,V2.y);
        glVertex2f(V3.x,V3.y);
        glVertex2f(V4.x,V4.y);
        glEnd();
        glColor3d(0,0,0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(V1.x,V1.y);
        glVertex2f(V2.x,V2.y);
        glVertex2f(V3.x,V3.y);
        glVertex2f(V4.x,V4.y);
        glEnd();
      }


  glutSwapBuffers();
}


void KeyPressed(unsigned char c, int x, int y) {
  if (c == '\x1b')
    exit(0);
}


void MouseButtonPressed(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    mouse_bgn_x = mouse_end_x = x;
    mouse_bgn_y = mouse_end_y = y;
  }
  else if(state == GLUT_UP) {
    mouse_end_x = x;
    mouse_end_y = y;
  } 
}


void MouseDragged(int x, int y) {
  mouse_end_x = x;
  mouse_end_y = y;
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

