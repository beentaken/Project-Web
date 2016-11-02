// MyMeshTest.cpp
// -- test of triangular mesh derived from Mesh class.
// cs200 9/14

#include <cstdlib>
#include <GL/glut.h>
#include "MyMesh.h"
using namespace std;


// global data (ugh)
int width = 600, height = 600;
const char *name = "MyMesh Test Driver";
int time_last;

const Point O(0,0);
const float rate = 8.0f*atan(1.0f)/5.0f;
Affine BigToWorld;
MyMesh mesh;
Point *temp;


// helper function to draw a mesh
void DrawMesh(Mesh *m, const Affine& A, const Hcoords& color) {
  for (int i=0; i < m->VertexCount(); ++i)
    temp[i] = A * m->GetVertex(i);
  glColor3f(color.x,color.y,color.w);
  glBegin(GL_TRIANGLES);
    for (int i=0; i < m->FaceCount(); ++i) {
      Mesh::Face face = m->GetFace(i);
      glVertex2f(temp[face.index1].x,temp[face.index1].y);
      glVertex2f(temp[face.index2].x,temp[face.index2].y);
      glVertex2f(temp[face.index3].x,temp[face.index3].y);
    }
  glEnd();
  glColor3f(0,0,0);
  glBegin(GL_LINES);
    for (int i=0; i < m->EdgeCount(); ++i) {
      Mesh::Edge edge = m->GetEdge(i);
      glVertex2f(temp[edge.index1].x,temp[edge.index1].y);
      glVertex2f(temp[edge.index2].x,temp[edge.index2].y);
    }
  glEnd();
}


void Init(void) {
  time_last = glutGet(GLUT_ELAPSED_TIME);
  temp = new Point[mesh.VertexCount()];
  BigToWorld = Scale(1.0f/mesh.Dimensions().x,1.0f/mesh.Dimensions().y)
               * Trans(O-mesh.Center());
}


void Draw(void) {
  // compute the time elapsed since the last call to 'Draw' (in seconds)
  int t = glutGet(GLUT_ELAPSED_TIME);
  double dt = 0.001*(t-time_last);
  time_last = t;

  // clear the screen
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);

  // array of purple objects in background
  const int COUNT = 8;
  float factor = 2.0f/COUNT,
        scale_x = 0.95f*factor/mesh.Dimensions().x,
        scale_y = 0.95f*factor/mesh.Dimensions().y;
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < COUNT; ++j) {
      float sign_x = (i%2 == 0) ? 1 : -1,
            sign_y = (j%2 == 0) ? 1 : -1;
      Affine ToWorld = Trans(Vector((i+0.5f)*factor-1,(j+0.5f)*factor-1))
                       * Scale(sign_x*scale_x,sign_y*scale_y)
                       * Trans(O-mesh.Center());
      DrawMesh(&mesh,ToWorld,Hcoords(1,0,0));
    }
  }

  // rotating green object in foreground
  BigToWorld = Rot(rate*dt) * BigToWorld;
  DrawMesh(&mesh,BigToWorld,Hcoords(0,0,1));

  glutSwapBuffers();
}


void KeyPressed(unsigned char c, int x, int y) {
  if (c == '\x1b')
    exit(0);
}


void MouseButtonPressed(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
}


void MouseDragged(int x, int y) {
}


void Resized(int W, int H) {
  glViewport(0,0,W,H);
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

