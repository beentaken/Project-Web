#include <stdio.h>

int main(int argc, char **argv)
{
  int line_count = 1, char_count = 0;
  char prev_char = 0;
  FILE *fp;

  if(argc > 1)
  {
    fp = fopen(argv[1], "rt");
    if(!fp)
    {
      printf("Can't open %s\n", argv[1]);
      return 1;
    }
  }
  else
  {
    fp = stdin;
  }
  while(!feof(fp))
  {
    char c = fgetc(fp);
    if(feof(fp))
    {
      break;
    }
    char_count++;
    if(c == '\n' && prev_char == '\n')
    {
      //Do nothing
    }
    if(char_count == 1 || prev_char == '\n')
    {
      printf("%6i", line_count++);
    }
    printf("%c", c);
    prev_char = c;
  }
  if(fp != stdin)
    fclose(fp);
  return 0;
}
