//******************************************************************************
// Name       : Damien Baca
// Assignment : Final Project First Draft
// Course     : CS200
// Term       : Fall 2015
//
// Description:
//   Final Project First Draft
// 
// How to Use:
//   'space' - Switches cameras
//   'q'     - Changes the direction of camera 2's rotation to clockwise
//   'w'     - Changes the direction of camera 2's rotation to counter clockwise
//******************************************************************************

#include <vector>
// Affine.h is included in "MyMesh.h"
#include "MyMesh.h"
#include "QuadMesh.h"
#include "Camera.h"
#include <sstream>
#include <cstdlib>
#include <GL/glut.h>

using namespace std;

namespace {

  // global data (ugh)
  const char *name = "Final Project First Draft";
  int width = 600, height = 600;
  const float PI = 4.0f*atan(1.0f);
  const float rate = (2 * PI)/5.0f;
  const float rate_faster = (4 * PI)/5.0f;
  int currcam = 0;
  bool clockwise = true;
  QuadMesh quadmesh;
  MyMesh mymesh;
  Camera camera[2];
  vector<Affine> Transform;
  
   // unit square, centered at origin
  const int vertex_count = 4;
  const Point vertices[vertex_count] = { Point(0.5f,0.5f),
                                         Point(-0.5f,0.5f),
                                         Point(-0.5f,-0.5f),
                                         Point(0.5f,-0.5f) };
  Point temp_verts[vertex_count];
  const int face_count = 2;
  const struct { int index1, index2, index3; }
      faces[face_count] = { {0,1,2}, {0,2,3} };

  const int edge_count = 4;
  const struct { int index1, index2; }
      edges[edge_count] = { {0,1}, {1,2}, {2,3}, {3,0} };
      
  Point *temp;
  Point start(0,0);
  Point meshcenter(0,0);
  const Point origin(0,0);
  
  int time_last;
  double current_time;
  int frame_count;
  double frame_time;
}

void DrawMesh(Mesh* m, const Affine& A, const Hcoords& Fillcolor, const Hcoords& LineColor)
{
  temp = new Point[m->VertexCount()];

  for(int i = 0; i < m->VertexCount(); i++)
    temp[i] = A * m->GetVertex(i);
  glColor3f(Fillcolor.x, Fillcolor.y, Fillcolor.w);
  glBegin(GL_TRIANGLES);
  for(int i = 0; i < m->FaceCount(); i++)
  {
    Mesh::Face face = m->GetFace(i);
    glVertex2f(temp[face.index1].x, temp[face.index1].y);
    glVertex2f(temp[face.index2].x, temp[face.index2].y);
    glVertex2f(temp[face.index3].x, temp[face.index3].y);
  }
  glEnd();
  glColor3f(LineColor.x, LineColor.y, LineColor.w);
  glBegin(GL_LINES);
  for(int i = 0; i < m->EdgeCount(); i++)
  {
    Mesh::Edge edge = m->GetEdge(i);
    glVertex2f(temp[edge.index1].x, temp[edge.index1].y);
    glVertex2f(temp[edge.index2].x, temp[edge.index2].y);
  }
  glEnd();
}

void Init(void) 
{
  int i = 0;
  time_last = glutGet(GLUT_ELAPSED_TIME);
  frame_count = 0;
  frame_time = 0;
  current_time = 0;
  float aspect = float(width)/float(height);
  camera[0] = Camera(Point(0,0), Vector(0,1), aspect * 10,10);
  camera[1] = Camera(Point(0,0), Vector(0,1), aspect *  5, 5);

  // Red Figure ----------------------------------------------------------------
    while(i < 6)
    {
      start.x -= .5;
      Affine MeshToWorld = Trans(start - origin)
                         * Scale(0.5f/quadmesh.Dimensions().x, 0.5f/quadmesh.Dimensions().y)
                         * Rot(PI/2)
                         * Trans(origin - quadmesh.Center());
      
      Transform.push_back(MeshToWorld);
      i++;
    }
    start.x = 0;
    while(i < 12)
    {
      start.x += .5;
      Affine MeshToWorld = Trans(start - origin)
                         * Scale(-0.5f/quadmesh.Dimensions().x, 0.5f/quadmesh.Dimensions().y)
                         * Rot(PI/2)
                         * Trans(origin - quadmesh.Center());
      
      Transform.push_back(MeshToWorld);
      i++;
    }
    start.x = 0;
    while(i < 18)
    {
      start.y -= .5;
      Affine MeshToWorld = Trans(start - origin)
                         * Scale(0.5f/quadmesh.Dimensions().x, -0.5f/quadmesh.Dimensions().y)
                         * Trans(origin - quadmesh.Center());
      
      Transform.push_back(MeshToWorld);
      i++;
    }
    start.y = 0;
    while(i < 24)
    {
      start.y += .5;
      Affine MeshToWorld = Trans(start - origin)
                         * Scale(0.5f/quadmesh.Dimensions().x, 0.5f/quadmesh.Dimensions().y)
                         * Trans(origin - quadmesh.Center());
      
      Transform.push_back(MeshToWorld);
      i++;
    }
    start.y = 0;
    
  //---------------------------------------------------------------------------- 
  
  // Blue Figure --------------------------------------------------------------- 
    while(i < 30)
    {
      start.x -= .5;
      Affine MeshToWorld = Trans(start - origin)
                         * Scale(0.5f/quadmesh.Dimensions().x, 0.5f/quadmesh.Dimensions().y)
                         * Rot(PI/2)
                         * Trans(origin - quadmesh.Center());
      
      Transform.push_back(MeshToWorld);
      i++;
    }
    start.x = 0;
    while(i < 36)
    {
      start.x += .5;
      Affine MeshToWorld = Trans(start - origin)
                         * Scale(-0.5f/quadmesh.Dimensions().x, 0.5f/quadmesh.Dimensions().y)
                         * Rot(PI/2)
                         * Trans(origin - quadmesh.Center());
      
      Transform.push_back(MeshToWorld);
      i++;
    }
    start.x = 0;
    while(i < 42)
    {
      start.y -= .5;
      Affine MeshToWorld = Trans(start - origin)
                         * Scale(0.5f/quadmesh.Dimensions().x, -0.5f/quadmesh.Dimensions().y)
                         * Trans(origin - quadmesh.Center());
      
      Transform.push_back(MeshToWorld);
      i++;
    }
    start.y = 0;
    while(i < 48)
    {
      start.y += .5;
      Affine MeshToWorld = Trans(start - origin)
                         * Scale(0.5f/quadmesh.Dimensions().x, 0.5f/quadmesh.Dimensions().y)
                         * Trans(origin - quadmesh.Center());
      
      Transform.push_back(MeshToWorld);
      i++;
    }
  //---------------------------------------------------------------------------
    
  //First Center Object -------------------------------------------------------  
  Affine A = Scale(0.5f/mymesh.Dimensions().x, 0.5f/mymesh.Dimensions().y)
           * Trans(origin - mymesh.Center());
  Transform.push_back(A);
  //---------------------------------------------------------------------------

  //Second Center Object ------------------------------------------------------  
  Affine B = Scale(0.5f/mymesh.Dimensions().x, 0.5f/mymesh.Dimensions().y)
           * Trans(origin - mymesh.Center());
  Transform.push_back(B);
  //---------------------------------------------------------------------------

  //Orbiting MyMesh -----------------------------------------------------------
  start.x = 3;
  start.y = 3;
  meshcenter.x = start.x;
  meshcenter.y = start.y;
  Affine C = Trans(start - origin)
           * Scale(1.0f/mymesh.Dimensions().x, 1.0f/mymesh.Dimensions().y)
           * Trans(origin - mymesh.Center());
  Transform.push_back(C);
  //---------------------------------------------------------------------------
}

void Draw(void) {
  // compute the time elapsed since the last call to 'Draw' (in seconds)
  int t = glutGet(GLUT_ELAPSED_TIME);
  double dt = 0.001*(t-time_last);
  time_last = t;

  // compute and display the frame rate
  ++frame_count;
  frame_time += dt;
  if (frame_time >= 0.5) {
    double fps = frame_count/frame_time;
    frame_count = 0;
    frame_time = 0;
    stringstream ss;
    ss << name << " [fps=" << int(fps) << "]";
    glutSetWindowTitle(ss.str().c_str());
  }

  // clear the screen
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  // WorldToNDC Transform
  Affine WorldToNDC = CameraToNDC(camera[currcam])
                    * WorldToCamera(camera[currcam]);

  //Drawing the red arrow figure
  for(int i = 0; i < 24; i++)
  {
    Transform[i] = Rot(rate * dt) * Transform[i];
    Affine A = WorldToNDC * Transform[i];
    DrawMesh(&quadmesh,A,Hcoords(1,0,0),Hcoords(0,0,1));
  }
  //Drawing the Blue arrow Figure
  for(int i = 24; i < 48; i++)
  {
    Transform[i] = Rot(-rate * dt) * Transform[i];
    Affine A = WorldToNDC * Transform[i];
    DrawMesh(&quadmesh,A,Hcoords(0,0,1),Hcoords(1,0,0));
  }
  
  // Creating blue spinning MyMesh
  Transform[48] = Rot(rate * dt) * Transform[48];
  Affine A = WorldToNDC * Transform[48];
  DrawMesh(&mymesh,A,Hcoords(0,0,1),Hcoords(0,0,1));
  
  // Creating Red spinning MyMesh
  Transform[49] = Rot(-rate * dt) * Transform[49];   
  Affine B = WorldToNDC * Transform[49];
  DrawMesh(&mymesh,B,Hcoords(1,0,0),Hcoords(0,0,1));
  
  if (currcam == 0) 
  {
    glColor3f(1,0,0);
    Affine C = WorldToNDC 
             * CameraToWorld(camera[1])
             * NDCToCamera(camera[1])
             * Scale(2, 2);
    C = Rot(-rate * dt) * C;
    for (int i = 0; i < vertex_count; ++i)
      temp_verts[i] = C * vertices[i];
    glBegin(GL_LINES);
    for (int k = 0; k < edge_count; ++k) 
    {
      glVertex2f(temp_verts[edges[k].index1].x,
                 temp_verts[edges[k].index1].y);
      glVertex2f(temp_verts[edges[k].index2].x,
                 temp_verts[edges[k].index2].y);
    }
    glEnd();
  }
  
  // Orbiting MyMesh
  Transform[50] = Rot(rate_faster * dt)
                * Trans(meshcenter - origin)
                * Rot(rate_faster * dt)
                * Trans(origin - meshcenter)
                * Transform[50];
  meshcenter = Rot(rate_faster * dt) * meshcenter;
  Affine C = WorldToNDC * Transform[50];
  DrawMesh(&mymesh,C,Hcoords(0,0,1),Hcoords(1,0,0));

  
  if(clockwise == true)
    camera[1].Rotate(rate * dt);
  if(clockwise == false)
    camera[1].Rotate(-rate * dt);
  
  glutSwapBuffers();
  current_time += dt;
}

void KeyPressed(unsigned char c, int x, int y) {
  if (c == '\x1b')
    exit(0);
  
  if (c == 'q')
      clockwise = false;
  else if (c == 'w')
      clockwise = true;
  else if (c == ' ')
      currcam = !currcam;
}

void Resized(int W, int H) 
{
  width = W;
  height = H;
  glViewport(0,0,W,H);  
  float ratio = float(W)/float(H);
  for (int i=0; i < 2; ++i)
    camera[i] = Camera(camera[i].Center(),
                       camera[i].Up(),
                       ratio * camera[i].Height(),
                       camera[i].Height());
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
