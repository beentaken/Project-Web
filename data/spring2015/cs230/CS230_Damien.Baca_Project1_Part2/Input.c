#include <stdio.h>
#include <Windows.h>
#include "GameStateList.h"

extern FILE *outfile;
extern int next;
extern MSG msg;

void Input_Handle(void)
{
  fprintf(outfile,"Input:Handle\n");
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
	  TranslateMessage(&msg);
	  DispatchMessage(&msg);
  }
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		/* when the window is created */
	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		if (wParam == VK_NUMPAD2)
			next = Level_2;
		if (wParam == 'Q')
			next = Quit;
		if (wParam == 'R')
			next = Restart;
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
