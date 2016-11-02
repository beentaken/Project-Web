#include <unistd.h> /* ADD COMMENTS */
#include <getopt.h> /* ADD COMMENTS */
#include <stdio.h>  /* ADD COMMENTS */
#include <stdlib.h>

#define CONVERT_CASE 32
#define TAB 9

enum                              /*|  Binary   | Base 10 |*/
{                                 /*-----------------------*/
    REMOVE_BLANK_LINES = 1 << 0,  /*| 0000 0001 |      1  |*/
    DELETE_X           = 1 << 1,  /*| 0000 0010 |      2  |*/
    HELP               = 1 << 2,  /*| 0000 0100 |      4  |*/
    TO_LOWER           = 1 << 3,  /*| 0000 1000 |      8  |*/
    LINE_NUMBERS       = 1 << 4,  /*| 0001 0000 |     16  |*/
    REPLACE_XY         = 1 << 5,  /*| 0010 0000 |     32  |*/
    TO_UPPER           = 1 << 6,  /*| 0100 0000 |     64  |*/
    EXPAND_TABS        = 1 << 7   /*| 1000 0000 |    128  |*/
};

static int Options; /* Value for storage of the above flags bits */
static int Num_Files;
char** Filenames;

static char delete_arg;
static char* replace_arg;
static char* space_per_tab;


/* STATIC FUNCTIONS WITH COMMENTS */
static void command_parser(int argc, char** argv)
{
    int i = 0;
    int c;
    while (1) 
    {   
        int option_index = 0;
        static struct option long_options[] = 
        {
            {"remove-blank-lines", no_argument      , NULL,  'b'},
            {"delete"            , required_argument, NULL,  'd'},
            {"help"              , no_argument      , NULL,  'h'},
            {"tolower"           , no_argument      , NULL,  'l'},
            {"replace"           , required_argument, NULL,  'r'},
            {"line-numbers "     , no_argument      , NULL,  'n'},
            {"toupper"           , no_argument      , NULL,  'u'},
            {"expand-tabs"       , optional_argument, NULL,  'x'},
            {NULL                ,                 0, NULL,    0}
        };
    
        c = getopt_long(argc, argv, "-:bd:hlr:nux::", long_options, &option_index);
        if (c == -1)
            break;
        
        switch (c)
        { 
            case 0:
                printf("long option %s", long_options[option_index].name);
                if (optarg)
                printf(" with arg %s", optarg);
                printf("\n");
                break;

            case 'b':
                Options |= REMOVE_BLANK_LINES;
                break;

            case 'd':
                Options |= DELETE_X;
                delete_arg = *optarg;
                break;

            case 'h':
                Options |= HELP;
                break;

            case 'l':
                Options |= TO_LOWER;
                break;

            case 'r':
                Options |= REPLACE_XY;
                replace_arg = optarg;
                break;

            case 'n':
                Options |= LINE_NUMBERS;
                break;

            case 'u':
                Options |= TO_UPPER;
                break;

            case 'x':
                space_per_tab = optarg;;
                Options |= EXPAND_TABS;
                break;

            case '?':
                printf("Unknown option: %c\n", optopt);
                Options |= HELP;
                break;

            case ':':
                printf("Missing arg for %c\n", optopt);
                Options |= HELP;
                break;

            case 1:
                Filenames[i] = optarg;
                i++;
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n", c);
                Options |= HELP;
        }
    }
}

void help()
{
    printf("Usage: filter [options] [file1 file2 ...]\n\n");
    printf("Options:\n");
    printf("-b  --remove-blank-lines  removes empty lines.\n");
    printf("-d  --delete=X            deletes all occurrences of char X.\n");
    printf("-h  --help                display this information.\n");
    printf("-l  --tolower             convert all characters to lower case.\n");
    printf("-n  --line-numbers        prepend line numbers to each line.\n");
    printf("-r  --replace=XY          replace all char X with char Y.\n");
    printf("-u  --toupper             convert all characters to upper case.\n");
    printf("-x  --expand-tabs[=X]     convert tabs to X spaces (default is 8).\n");
}

void file_count(int argc, char** argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "-:bd:hlr:nux::")) != -1)
        if(opt == 1)
            Num_Files++;
    optind = 1;
}

static void filter(FILE *input, char* num_spaces_ascii, char* Rep_xy, char del_arg)
{
    int i;
    int num_spaces = 0;  
    char currchar = 0;
    char lastchar = 0;
    int char_counter = 0;
    int line_counter = 0;

    if(Options & EXPAND_TABS)
    {
        if(num_spaces_ascii)
            num_spaces = atoi(num_spaces_ascii);
        else
            num_spaces = 8;
    }

    while(1)
    {
        /* Get a new char */
        currchar = fgetc(input);
        char_counter++;

        if(feof(input))
            break;
        
        while((currchar == '\n' && lastchar == '\n') && (Options & REMOVE_BLANK_LINES))
            currchar = fgetc(input);

        if((char_counter == 0 && (Options & LINE_NUMBERS)) ||
               (lastchar == '\n' && (Options & LINE_NUMBERS)))
        {
            printf("%i ", line_counter);
            line_counter++;
        }

        if((currchar != del_arg && (Options & DELETE_X)) &&
        ((Options & DELETE_X) || (Options & REPLACE_XY) || (Options & TO_UPPER) ||
        (Options & TO_LOWER) || (Options & EXPAND_TABS)))
        {
            if((Options & REPLACE_XY) && currchar == *Rep_xy)
            {
                currchar = *(Rep_xy + 1);
            }

            if((currchar >= 'a' && 'z' >= currchar) && (Options & TO_UPPER)) 
            {
                currchar -= CONVERT_CASE;
            }

            if((currchar >= 'A' && 'Z' >= currchar) && (Options & TO_LOWER))
            {
                currchar += CONVERT_CASE;
            }

            if((currchar == TAB) && (Options & EXPAND_TABS))
            {
                for(i = 0; i < num_spaces; i++)
                fputc(' ', stdout);
            }
            else
            {
                fputc(currchar, stdout);
            }

        }
        else if(!(Options & DELETE_X))
            fputc(currchar, stdout);

        lastchar = currchar;


    }
}

int main(int argc, char **argv)
{
    FILE* input;
    int i;
    
    file_count(argc, argv);
    
    Filenames = malloc(sizeof(char*) * Num_Files);
    if(Filenames == NULL)
    {
        printf("Error: Failed To Allocate %lu Bytes\n", (sizeof(char*) * Num_Files));
        printf("Terminating Program . . .\n");
        return 1; 
    }
    
    command_parser(argc, argv);
    
    if(!(Options & HELP))
    {
        /* If no files are supplied, Num_Files is set to 1for stdin */ 
        if(*Filenames == NULL)
            Num_Files++;
        
        for(i = 0; i < Num_Files; i++)
        {
            /* Determine the input */
            if(*Filenames)
            {
                input = fopen(Filenames[i], "rt");
                if(input == NULL)
                {
                    printf("Error: Failed To Open %s\n", Filenames[i]);
                    printf("Terminating Program . . .\n");
                    return 1;
                }
            }
            else
              input = stdin;

            
            filter(input, space_per_tab, replace_arg, delete_arg);
            
            if(*Filenames)
                free(input);
        }        
    }
    else
        help();

    free(Filenames);
    return 0;
}

