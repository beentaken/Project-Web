// cs200_h1_demo.cpp
// -- graphical demo/test of 'Affine' package
// cs200 5/15
//
// To compile under Windows:
//   cl /EHsc cs200_h1_demo.cpp Affine.cpp
//   [you can substitute 'Affine.cpp' with 'Affine.lib']
// To compile under Linux:
//   g++ cs200_h1_demo.cpp Affine.cpp -lGL -lglut

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <GL/glut.h>
#include "Affine.h"
using namespace std;


// global data (ugh)
int width = 600, height = 600;
const char *name = "CS200 HW#1 Demo";
int time_last;
double current_time;

const Point vertices[4] = { Point( 0.5f, 0.5f), Point(-0.5f, 0.5f),
                            Point(-0.5f,-0.5f), Point( 0.5f,-0.5f) };
const int edges[4][2] = { {0,1}, {1,2}, {2,3}, {3,0} };
const int faces[2][3] = { {0,1,2}, {0,2,3} };
int square_count;
Point center;
float rot_rate,
      size1,
      scale;
vector<Point> temp_verts(4);

const float PI = 4.0f*atan(1.0f);
const Point O(0,0);


float frand(float a=0, float b=1) {
  return a + (b-a)*float(rand())/float(RAND_MAX);
}


void Init(void) {
  srand(unsigned(time(0)));
  time_last = glutGet(GLUT_ELAPSED_TIME);
  current_time = 0;
  // parameters for array of squares
  center = O;
  rot_rate = 2*PI/frand(5,10);
  square_count = 3;
  size1 = 1.0f/square_count;
  scale = 0.8f;
}


void Draw(void) {
  // compute the time elapsed since the last call to 'Draw' (in seconds)
  int t = glutGet(GLUT_ELAPSED_TIME);
  double dt = 0.001*(t-time_last);
  time_last = t;

  // clear the screen
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);

  // draw rotating array of squares
  Affine R = Rot(rot_rate*current_time);
  for (int i=0; i < square_count; ++i) {
    for (int j=0; j < square_count; ++j) {
      // compute modeling transform
      Vector offset = size1 * (Vector(i,j) - 0.5f*(square_count-1)*Vector(1,1));
      Affine obj2world = Trans(center-O) * R * Trans(offset) * Scale(scale*size1);
      // transform the vertices
      for (int k=0; k < 4; ++k)
        temp_verts[k] = obj2world * vertices[k];
      // draw the triangular faces
      glColor3f(1-i*size1,0.5f,1-j*size1);
      glBegin(GL_TRIANGLES);
      for (int k=0; k < 2; ++k) {
        const Point& P = temp_verts[faces[k][0]],
                     Q = temp_verts[faces[k][1]],
                     R = temp_verts[faces[k][2]];
        glVertex2f(P.x,P.y);
        glVertex2f(Q.x,Q.y);
        glVertex2f(R.x,R.y);
      }
      glEnd();
      // draw the edges
      glColor3f(0,0,0);
      glBegin(GL_LINES);
      for (int k=0; k < 4; ++k) {
        const Point& P = temp_verts[edges[k][0]],
                     Q = temp_verts[edges[k][1]];
        glVertex2f(P.x,P.y);
        glVertex2f(Q.x,Q.y);
      }
      glEnd();
    }
  }

  glutSwapBuffers();
  current_time += dt;
}


void KeyPressed(unsigned char c, int x, int y) {
  switch (c) {
    case '\x1b':
      exit(0);
    case ' ':
      square_count = 1 + square_count%10;
      size1 = 1.0f/square_count;
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
  glutReshapeFunc(Resized);
  glutIdleFunc(Loop);
  Init();
  glutMainLoop();
  return 0;
}

