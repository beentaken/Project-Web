// cs200project1.empty.cpp
// -- template for first draft of the final project
// cs200 5/15

#include <sstream>
#include <cstdlib>
#include <GL/glut.h>
using namespace std;


namespace {

  // global data (ugh)
  int width = 600, height = 600;
  const char *name = "Final Project First Draft";
  int time_last;
  double current_time;
  int frame_count;
  double frame_time;

}


void Init(void) {
  time_last = glutGet(GLUT_ELAPSED_TIME);
  frame_count = 0;
  frame_time = 0;
  current_time = 0;
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
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);


  // put rendering code here ...


  // swap in the back buffer
  glutSwapBuffers();
  current_time += dt;
}


void KeyPressed(unsigned char c, int x, int y) {
  if (c == '\x1b')
    exit(0);
}


void Resized(int W, int H) {
  width = W;
  height = H;
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
  glutReshapeFunc(Resized);
  glutIdleFunc(Loop);
  Init();
  glutMainLoop();
  return 0;
}

