#include <Windows.h>
#include <stdio.h>
#include "GameStateList.h"
#include "GameStateManager.h"
#include "Input.h"
#include "CreateWnd.h"
#include "System.h"


extern int next;                 /* (extern) Flags a change in game state                                       */
extern int current;              /* (extern) Indicates the current game state                                   */
extern int previous;             /* (extern) Indicates the previous game state                                  */
extern void(*pLoad)(void);       /* (extern) Function pointer set to the currently selected levels Load()       */
extern void(*pInitialize)(void); /* (extern) Function pointer set to the currently selected levels Initialize() */
extern void(*pUpdate)(void);     /* (extern) Function pointer set to the currently selected levels Update()     */
extern void(*pDraw)(void);       /* (extern) Function pointer set to the currently selected levels Draw()       */
extern void(*pFree)(void);       /* (extern) Function pointer set to the currently selected levels Free()       */
extern void(*pUnload)(void);     /* (extern) Function pointer set to the currently selected levels Unload()     */
extern FILE *outfile;            /* (extern) File pointer set to the output file "Output.txt                    */
extern WNDCLASS wc;              /* (extern) Creates a new window class named "wc"                              */
extern MSG msg;                  /* (extern) Creates a Msg/Event named "msg                                     */


int WINAPI WinMain(HINSTANCE instance, HINSTANCE hPreviousInstance, LPSTR command_line, int show)
{
  LARGE_INTEGER StartingTime;                          /* Initializes a type LARGE_INTEGER for the Start Time of the frame */
  LARGE_INTEGER EndingTime;                            /* Initializes a type LARGE_INTEGER for the End Time of the frame   */
  LARGE_INTEGER Frequency;                             /* The number of counts the system is running per second            */
  double Min_Frame_Time;                               /* The time in seconds that the frame time should be locked too     */
  int Max_Frame_Rate;                                  /* The Maximum Frame Rate that the game loop should run at          */
  int FrameCounter = 0;                                /* The number of frames since the beginning of the application      */
                                                       /*                                                                  */
  FILE *pfile;                                         /* Creates a file pointer                                           */
  fopen_s(&pfile, "Max_Frames.txt", "r");              /* Open "Max_Frames.txt" for read and point pfile at it             */
  fscanf_s(pfile, "%i", &Max_Frame_Rate, sizeof(int)); /* Scan in an int and store it in "Max_Frame_Rate                   */
  fclose(pfile);                                       /* Close the file you opened!!                                      */
                                                       /*                                                                  */
  System_Initialize(instance);                         /* Initializes the window class                                     */
  Create_Window(instance, show);                       /* Creates the window                                               */
  GSM_Initialize(Level_1);                             /* Points all function ptrs to the functions of Level_1             */

  QueryPerformanceFrequency(&Frequency);                        /* The number of counts per second run by the system       */
  Min_Frame_Time = (1.0 / Max_Frame_Rate) * Frequency.QuadPart; /* The minimum frame time that a frame is aloud to take    */

  while (current != Quit)      /* While the current game state is not Quit                                           */
  {                            /*                                                                                    */
	if (current != Restart)    /* If the current game state is Restatrt                                              */
	{                          /*                                                                                    */
	  GSM_Update();            /* Updates all function ptrs to point at the currently selected game states functions */
	  pLoad();                 /* Calls the load function of the current game state                                  */
    }
    else
    {
      next = previous;
      current = previous;
    }
    pInitialize();
 
    while (next == current)
    {
      QueryPerformanceCounter(&StartingTime);

      Input_Handle();
      pUpdate();
      pDraw();

      do
      {
        QueryPerformanceCounter(&EndingTime);

      }
	  while (EndingTime.QuadPart - StartingTime.QuadPart < Min_Frame_Time);

      fprintf(outfile,"%f\n", (double)(EndingTime.QuadPart - StartingTime.QuadPart) / Frequency.QuadPart); 
      FrameCounter++;
    }
    pFree();

    if (next != Restart)
      pUnload ();
 
    previous = current;
    current = next;
  }
  System_End();
  UnregisterClass(wc.lpszClassName, instance);

  return (int)msg.wParam;
}

