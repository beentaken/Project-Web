#include <Windows.h>

extern HWND hWnd;
extern WNDCLASS wc;

void Create_Window(HINSTANCE instance, int show)
{
  LPCTSTR WndName = L"CS230 Project 1 Part 2";  /* The name of the window                                                                                                       */

  hWnd = CreateWindow(wc.lpszClassName,
      WndName,								    /*The window caption                                                                                                            */
	  WS_OVERLAPPEDWINDOW,						/*The style of the window, which determines if the window will have a minimize/maximize buttons, if its frame is thick or not.. */
	  200,										/*The X position of the top left corner of the window. Remember that (0,0) is the top left of the monitor                       */
	  100,										/*The Y position of the top left corner of the window. Remember that (0,0) is the top left of the monitor                       */
	  640,										/*The width of the window                                                                                                       */
	  480,										/*The heiht of the window                                                                                                       */
	  NULL,										/*Handle to the parent window (in case this was a child window)                                                                 */
	  NULL,										/*Handle to a menu (In case there is a menu for this window)                                                                    */
	  instance,									/*Instance of the module associated with the window. This is the 1st paramter passed to the WinMain function                    */
	  NULL);									/*Pointer to a value sent to the window in the WM_CREATE message                                                                */
  ShowWindow(hWnd, show);
  UpdateWindow(hWnd);
}