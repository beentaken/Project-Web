#include <stdio.h>

extern FILE *outfile;

void Input_Handle(void)
{
  fprintf(outfile,"Input:Handle\n");
}