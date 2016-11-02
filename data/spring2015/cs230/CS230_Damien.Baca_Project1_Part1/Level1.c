#include <stdio.h>
#include "GameStateList.h"

extern int next;
static int Level1_Health;
extern FILE *outfile;

void Load1(void)
{ 
  FILE *pfile;
  pfile = fopen("Level1_Health.txt","r");
  fscanf(pfile,"%i", &Level1_Health);
  fclose(pfile);
  
  fprintf(outfile,"Level1:Load\n");
}

void Initialize1(void)
{
  fprintf(outfile,"Level1:Initialize\n");
}

void Update1(void)
{
  fprintf(outfile,"Level1:Update\n");
  Level1_Health--;
  
  if(Level1_Health == 0)
    next = Level_2;
}

void Draw1(void)
{
  fprintf(outfile,"Level1:Draw\n");
}

void Free1(void)
{
  fprintf(outfile,"Level1:Free\n");
}

void Unload1(void)
{
  fprintf(outfile,"Level1:Unload\n");
}
