#include <stdio.h>
#include "Level1.h"
#include "Level2.h"
#include "GameStateList.h"

extern int next;
extern int current;
extern int previous;
extern void(*pLoad)(void);
extern void(*pInitialize)(void);
extern void(*pUpdate)(void);
extern void(*pDraw)(void);
extern void(*pFree)(void);
extern void(*pUnload)(void);
extern FILE *outfile;

void GSM_Initialize(int Initial)
{
  current = Initial;
  next = Initial;
  previous = Initial;
  fprintf(outfile,"GSM:Initialize\n");
}

void GSM_Update(void)
{
  if (current == Level_1)
  {
    pLoad = &Load1;
    pInitialize = &Initialize1;
    pUpdate = &Update1;
    pDraw = &Draw1;
    pFree = &Free1;
    pUnload = &Unload1;
  }

  if (current == Level_2)
  {
    pLoad = &Load2;
    pInitialize = &Initialize2;
    pUpdate = &Update2;
    pDraw = &Draw2;
    pFree = &Free2;
    pUnload = &Unload2;
  }
  fprintf(outfile,"GSM:Update\n");
}