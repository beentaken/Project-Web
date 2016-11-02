#include <stdio.h>
#include "GameStateList.h"

extern int next;
static int Level2_Health;
static int Level2_Lives;
static int Level2_Reset;
extern FILE *outfile;

void Load2(void)
{
  FILE *pfile;
  pfile = fopen("Level2_Health.txt","r");
  fscanf(pfile,"%i", &Level2_Health);
  fclose(pfile);
  
  pfile = fopen("Level2_Lives.txt","r");
  fscanf(pfile,"%i", &Level2_Lives);
  fclose(pfile);
  
  Level2_Reset = Level2_Health;
  
  fprintf(outfile,"Level2:Load\n");
}

void Initialize2(void)
{
  fprintf(outfile,"Level2:Initialize\n");
}

void Update2(void)
{
  fprintf(outfile,"Level2:Update\n");
  Level2_Health--;
  
  if(Level2_Health == 0)
  {
    Level2_Lives--;
    next = Restart;
    Level2_Health = Level2_Reset;
    
    if(Level2_Lives == 0)
      next = Quit;
  }
}

void Draw2(void)
{
  fprintf(outfile,"Level2:Draw\n");
}

void Free2(void)
{
  fprintf(outfile,"Level2:Free\n");
}

void Unload2(void)
{
  fprintf(outfile,"Level2:Unload\n");
}
