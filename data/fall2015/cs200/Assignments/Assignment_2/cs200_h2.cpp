//******************************************************************************
// Name       :Damien Baca
// Assignment :Assignment #2
// Course     :CS200
// Term       :Fall 2015
//******************************************************************************
#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include "MyMesh.h"
#include "QuadMesh.h"
using namespace std;

MyMesh mesh; 
QuadMesh Qmesh;
Point *temp;
Point RotP(0,0);
const Point origin(0,0);
int width = 1000;
int height = 1000;
const float PI = 4.0f*atan(1.0f);
const float rate = (2 * PI)/5.0f;
const char *name = "CS200 Assignment 2";
int time_last = time_last = glutGet(GLUT_ELAPSED_TIME);
Affine BigtoWorld = Scale(1.0f/mesh.Dimensions().x, 1.0f/mesh.Dimensions().y)
                  * Trans(origin - mesh.Center());

// Draws the mesh
void DrawMesh(Mesh *m, const Affine& A, const Hcoords& color)
{
  temp = new Point[m->VertexCount()];
  
  //Transform the vertexes of the mesh
  for(int i = 0; i < m->VertexCount(); i++)
    temp[i] = A * m->GetVertex(i);
  
  glColor3f(color.x, color.y, color.w);
  glBegin(GL_TRIANGLES);
    for(int i = 0; i < m->FaceCount(); i++)
    {
      Mesh::Face face = m->GetFace(i);
      //Access the x/y component of a vertex that is referenced by a face
      glVertex2f(temp[face.index1].x, temp[face.index1].y);
      glVertex2f(temp[face.index2].x, temp[face.index2].y);
      glVertex2f(temp[face.index3].x, temp[face.index3].y);
    }
  glEnd();
  glColor3f(0, 0, 0);
  glBegin(GL_LINES);
    for(int i = 0; i < m->EdgeCount(); i++)
    {
      Mesh::Edge edge = m->GetEdge(i);
      //Access the x/y component of a vertex that is referenced by an edge
      glVertex2f(temp[edge.index1].x, temp[edge.index1].y);
      glVertex2f(temp[edge.index2].x, temp[edge.index2].y);
    }
  glEnd();
  // I didn't see you deleting this in your example 
  // I did, however it seems that drmemory goes crazy when I run it 
  // on both your example and this file.
  // I assume since it did that with yours as well, that this is something that
  // is going on with glut not my code.
  delete temp;
}

void Draw(void)
{
  int t = glutGet(GLUT_ELAPSED_TIME);
  double dt = 0.001 * (t - time_last);
  time_last = t;
  
  //Sets the color with whitch to clear the screen
  glClearColor(0,0,0,0);
  //Clears the screen
  glClear(GL_COLOR_BUFFER_BIT);
  
  // Drawing The Rotating "MyMesh" ---------------------------------------------
  BigtoWorld = Trans(RotP - origin) 
             * Rot(rate * dt) 
             * Trans(origin - RotP) 
             * BigtoWorld;    
  DrawMesh(&mesh,BigtoWorld,Hcoords(0,0,1));
  
  
  // Drawing The "QuadMesh" Rotation Point -------------------------------------
  float scale_x = 0.1 / Qmesh.Dimensions().x,
        scale_y = 0.1 / Qmesh.Dimensions().y;  
  // Create a new "center" at the point of the QuadMesh for easier rotation
  Point Center(Qmesh.Center().x, Qmesh.Center().y - (Qmesh.Dimensions().y/2));
  for(int i = 0; i < 3; i++)
  {
    // Each loop an arrow is drawn with an additional 120 degree rotation
    // starting at 0 * 120 degrees
    Affine ToWorld = Trans(RotP - origin) 
                   * Rot(i * ((2 * PI)/3.0f)) 
                   * Scale(scale_x, scale_y) 
                   * Trans(origin - Center);
    DrawMesh(&Qmesh,ToWorld,Hcoords(1,0,0));
  }
  
  glutSwapBuffers();
}

void MouseButtonPressed(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    Point NewRotP(x, y);
    Point WorldCenter(width/2.0f, height/2.0f);
    
    RotP = Scale(2.0f/width, -2.0f/height) 
         * Trans(origin - WorldCenter) 
         * NewRotP;
  }
}

void Resized(int W, int H)
{
  // Updating width and height variables for use in transforms when the window 
  // has been resized
  glViewport(0,0,W,H);
  width = glutGet(GLUT_WINDOW_WIDTH);
  height = glutGet(GLUT_WINDOW_HEIGHT);
}

void Loop(void)
{
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(width,height);
  glutCreateWindow(name);
  glutDisplayFunc(Draw);
  glutMouseFunc(MouseButtonPressed);
  glutReshapeFunc(Resized);
  glutIdleFunc(Loop);
  //Got rid of Init, did not understand how to properly use it.
  //Init();
  glutMainLoop();
  return 0;
}