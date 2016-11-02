#include <stdio.h>
#include <Windows.h>
#include "WinProc.h"

int current;
int next;
int previous;
void(*pLoad)(void);
void(*pInitialize)(void);
void(*pUpdate)(void);
void(*pDraw)(void);
void(*pFree)(void);
void(*pUnload)(void);
FILE *outfile;
WNDCLASS wc;
HWND hWnd;
MSG msg;

void System_Initialize(HINSTANCE instance)
{
	LPCTSTR CsName = L"Window Class";

	wc.style = CS_HREDRAW | CS_VREDRAW;							/*Class style*/
	wc.lpfnWndProc = WinProc;									/*A function pointer which should point to the procedure function. Remember that the procedure function handles the window messages*/
	wc.cbClsExtra = 0;											/*The number of extra bytes you want to allocate for this window class structure. The default value is 0*/
	wc.cbWndExtra = 0;											/*The number of extra bytes you want to allocate for the window instance*/
	wc.hInstance = instance;									/*Instance of the module associated with the window. This is the 1st paramter passed to the WinMain function*/
	wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);					/*Handle to the icon class which will be displayed on the top left part of the window*/
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					/*Handle to the cursor class which will be used in this window class*/
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		/*Handle to the class background brush. It can be a color value*/
	wc.lpszMenuName = NULL;										/*Pointer to a null terminated string which specifies the name of the menu resources used by this window class (if any)*/
	wc.lpszClassName = CsName;						/*String that specifies thw window class name. This is needed to create any window based on this window class*/

	RegisterClass(&wc);

  fopen_s(&outfile, "output.txt","w");
  fprintf(outfile,"System:Initialize\n");
}

void System_End(void)
{
  fprintf(outfile,"System:Exit\n");
  fclose(outfile);
}
