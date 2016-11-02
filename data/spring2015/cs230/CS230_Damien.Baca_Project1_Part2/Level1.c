#include <stdio.h>

extern FILE *outfile;

void Load1(void)
{ 
  fprintf(outfile,"Level1:Load\n");
}

void Initialize1(void)
{
  fprintf(outfile,"Level1:Initialize\n");
}

void Update1(void)
{
  fprintf(outfile, "Level1:Update\n");
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
