#include <stdio.h>

extern FILE *outfile;

void Load2(void)
{
  
  fprintf(outfile,"Level2:Load\n");
}

void Initialize2(void)
{
  fprintf(outfile,"Level2:Initialize\n");
}

void Update2(void)
{
  fprintf(outfile, "Level2:Update\n");
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
