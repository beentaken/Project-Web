/******************************************************************************
  Author: Damien Paige Baca
  Date  : Thursday January 28, 2016
  LOC   : 204
  
  Breif:
    OpenAl sound Demo
******************************************************************************/
#include <GL/glut.h>
#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <fstream>
#include <cstring>

#define FALSE
#define TRUE

void update(void);
void draw(void);
void keyboard(unsigned char key, int x, int y);
char* loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size);
int convertToInt(char* buffer,int len);
bool isBigEndian();

static float red   = 0.0f;
static float green = 0.0f;
static float blue  = 0.0f;
static float alpha = 0.0f;

static unsigned int sourceid, bufferid, format;
static int channel, sampleRate, bps, size;
static ALCcontext *context;
static ALCdevice *device;
static char *data;

int main(int argc, char **argv)
{
    data = loadWAV("Laser_Shoot12.wav",channel,sampleRate,bps,size);

	// Window Creation
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Linux Game Project");
    
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(draw);

    device = alcOpenDevice(NULL);

    if(device == NULL)
    {
      std::cout << "cannot open sound card" << std::endl;
      return 0;
    }

    context = alcCreateContext(device, NULL);

    if(context == NULL)
    {
      std::cout << "cannot open context" << std::endl;
      return 0;
    }

      alcMakeContextCurrent(context);
      alGenBuffers(1, &bufferid);

      if(channel == 1)
      {
        if(bps == 8)
          format = AL_FORMAT_MONO8;
        else
          format = AL_FORMAT_MONO16;               
      }else{
        if(bps == 8)
          format = AL_FORMAT_STEREO8;
        else
          format = AL_FORMAT_STEREO16;                 
      }

      alBufferData(bufferid, format, data, size, sampleRate);
      alGenSources(1, &sourceid);
      alSourcei(sourceid, AL_BUFFER, bufferid);

    glutMainLoop();
}

void draw(void)
{
	glClearColor( red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
      case 'r':
        printf("R was pressed - BOOP!\n");
        alSourcePlay(sourceid);
        if(red == 0.0f)
          red = 1.0f;
        else
          red = 0.0f;
        break;

      case 'g':
        printf("G was pressed - BOOP!\n");
        alSourcePlay(sourceid);
        if(green == 0.0f)
          green = 1.0f;
        else
          green = 0.0f;
        break;

      case 'b':
      	printf("B was pressed - BOOP!\n");
      	alSourcePlay(sourceid);
        if(blue == 0.0f)
          blue = 1.0f;
        else
          blue = 0.0f;
        break;

      // Escape Key
      case 27: 
        alDeleteSources(1, &sourceid);
        alDeleteBuffers(1, &bufferid);
        alcDestroyContext(context);
        alcCloseDevice(device);
        delete[] data;
        exit (0);
        break;

      default:
        break;
    }
    glutPostRedisplay();
}

bool isBigEndian()
{
  int num = 1;
  return !((char *) &num)[0];
}

int convertToInt(char *buffer,int len)
{
  int a = 0;
  if(!isBigEndian())
    for(int i = 0; i < len; i++)
      ((char *)&a)[i] = buffer[i];

  if(isBigEndian())
    for(int i = 0; i < len; i++)
      ((char *)&a)[3 - i] = buffer[i]; 

  return a;
}

char *loadWAV(const char *fn,int &chan,int &samplerate,int &bps,int &size)
{
  char buffer[20];
  std::ifstream in(fn,std::ios::binary);

  in.read(buffer, 4);   // Advance
  
  // If the first 4 bytes are not the string "RIFF", then it is not a WAVE file
  if(strncmp(buffer, "RIFF", 4) != 0)
  {
    std::cout << "this is not a valid WAVE file" << std::endl;
    return NULL;
  }
  
  in.read(buffer, 20);  // Advance
  
  // Read In Channel Info
  chan = convertToInt(buffer,2);
  
  in.read(buffer, 4);  // Advance

  // Read In Samplerate
  samplerate = convertToInt(buffer,4);

  in.read(buffer, 8);  // Advance
  
  // Read In BPS
  bps = convertToInt(buffer, 2);

  in.read(buffer, 8);  // Advance
  
  // Read In Size of data
  size = convertToInt(buffer, 4);
  
  // Finally Read In The Actual Data
  char* data = new char[size];
  in.read(data, size);

  return data;   
}
