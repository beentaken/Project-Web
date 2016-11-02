#include <stdio.h>

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

void System_Initialize(void)
{
  outfile = fopen("output.txt","w");
  fprintf(outfile,"System:Initialize\n");
}
void System_End(void)
{
  fprintf(outfile,"System:Exit\n");
  fclose(outfile);
}
