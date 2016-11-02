// cs200triangle_test.cpp
// -- test harness for triangle filling algorithm (y-axis up)
// cs200 10/15
//
// Note: this must be linked with 'Affine.cpp', 'Inverse.cpp',
//       and 'RasterUtilities.cpp'

#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include "Affine.h"
#include "Raster.h"
#include "RasterUtilities.h"
using namespace std;


namespace {

  // global data (ugh)
  int width = 500, height = 500;
  const char *name = "Triangle Test";

  int cell_count_y = 30,
      cell_count_x,
      xmin, ymin, xmax, ymax;
  int vertices[6];
  int vertex_count;
  Raster::byte *grid;

}


void Init(void) {
  // hopefully this is large enough to handle most window resizes ...
  grid = new Raster::byte[3*width*height];
  for (int i=0; i < 6; ++i)
    vertices[i] = 0;
  vertex_count = 0;
}


void Draw(void) {
  // clear the screen
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);

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

  // draw actual edge
  glColor3d(0,0,1);
  if (vertex_count == 1) {
    Point A[2], P[2];
    for (int i=0; i < 2; ++i) {
      A[i] = G * Point(vertices[2*i],vertices[2*i+1]);
      P[i] = NG*A[i];
    }
    glBegin(GL_LINES);
    glVertex2f(P[0].x,P[0].y);
    glVertex2f(P[1].x,P[1].y);
    glEnd();
  }

  else {
    Point A[3], P[3];
    for (int i=0; i < 3; ++i) {
      A[i] = G * Point(vertices[2*i],vertices[2*i+1]);
      P[i] = NG*A[i];
    }
    glBegin(GL_LINE_LOOP);
    for (int i=0; i < 3; ++i)
      glVertex2f(P[i].x,P[i].y);
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
    FillTriangle(raster,A[0],A[1],A[2]);
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
          glColor3d(0,1,1);
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
  }

  glutSwapBuffers();
}


void KeyPressed(unsigned char c, int, int) {
  if (c == '\x1b')
    exit(0);
}


void MouseButtonPressed(int, int state, int x, int y) {
  if (x < xmin || x > xmax || y < ymin || y > ymax)
    return;
  if (state == GLUT_DOWN) {
    vertices[2*vertex_count] = x;
    vertices[2*vertex_count+1] = y;
    ++vertex_count;
    if (vertex_count < 3) {
      vertices[2*vertex_count] = x;
      vertices[2*vertex_count+1] = y; }
    else
      vertex_count = 0;
  }
}


void MouseMoved(int x, int y) {
  if (x < xmin || x > xmax || y < ymin || y > ymax)
    return;
  if (vertex_count != 0) {
    vertices[2*vertex_count] = x;
    vertices[2*vertex_count+1] = y;
  }
}


void Resized(int W, int H) {
  glViewport(0,0,W,H);
  width = W;
  height = H;
  float ratio = float(width)/float(height);
  cell_count_x = int(ratio*cell_count_y);
  // grid bounds in screen space
  xmin = int(0.5f*width/(cell_count_x+1));
  xmax = int((cell_count_x+0.5f)*width/(cell_count_x+1));
  ymin = int(0.5f*height/(cell_count_y+1));
  ymax = int((cell_count_y+0.5f)*height/(cell_count_y+1));
  for (int i=0; i < 6; i+=2) {
    vertices[i] = xmin;
    vertices[i+1] = ymin;
  }
  vertex_count = 0;
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
  glutPassiveMotionFunc(MouseMoved);
  glutReshapeFunc(Resized);
  glutIdleFunc(Loop);
  Init();
  glutMainLoop();
  return 0;
}

