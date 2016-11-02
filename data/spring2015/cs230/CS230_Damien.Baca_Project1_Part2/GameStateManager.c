#include <stdio.h>
#include "Level1.h"
#include "Level2.h"
#include "GameStateList.h"

int next;
int current;
int previous;
void(*pLoad)(void);
void(*pInitialize)(void);
void(*pUpdate)(void);
void(*pDraw)(void);
void(*pFree)(void);
void(*pUnload)(void);
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