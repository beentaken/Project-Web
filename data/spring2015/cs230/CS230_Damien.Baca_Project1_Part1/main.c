#include "GameStateList.h"
#include "System.h"
#include "GameStateManager.h"
#include "Input.h"

extern int next;
extern int current;
extern int previous;
extern void(*pLoad)(void);
extern void(*pInitialize)(void);
extern void(*pUpdate)(void);
extern void(*pDraw)(void);
extern void(*pFree)(void);
extern void(*pUnload)(void);

int main(void)
{
  System_Initialize();
  GSM_Initialize(Level_1);
  
  while(current != Quit)
  {
    if(current != Restart)
    {
      GSM_Update();
      pLoad();
    }
    else
    {
      next = previous;
      current = previous;
    }
    pInitialize();
    
    while(next == current)
    {
      Input_Handle();
      pUpdate();
      pDraw();
    }
    pFree();
    if(next != Restart)
      pUnload();
    
    previous = current;
    current = next;
  }
  System_End();
  return 0;
}