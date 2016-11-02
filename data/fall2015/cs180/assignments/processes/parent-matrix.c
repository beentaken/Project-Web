/******************************************************************************/
/*!
  \file   parent-matrix.c
  \author Damien Baca
  \par    email: damien.baca\@digipen.edu
  \par    DigiPen login: damien.baca
  \par    Course: CS180
  \par    Assignment #3
  \date   Due - 11/01/2015
  \brief  Calculates the square of a matrix using child process's
          - get_matrix
          - print_matrix
          - main
*/
/******************************************************************************/
#include <stdio.h>   /* fopen, fscanf, fclose, printf, sprintf */
#include <stdlib.h>  /* exit, malloc                           */
#include <sys/shm.h> /* shmget, shmat, shmdt, shmctl           */
#include <sys/wait.h>/* waitpid                                */
#include <unistd.h>  /* sleep, fork, execv                     */
#define MAX_ARG_SIZE 20

/* NOTE: 
   I used one as a general error code since we has no guidelines
   on what they should be for particular errors. All errors print
   the cause of the error before returning.
*/
  
/******************************************************************************/
/*!
  \brief Reads in a matrix from a text file and returns it as an int pointer
    
  \param filename  - The name of the text file that contains the matrix

  \param width     - Allows function to set width through the pointer

  \return The matrix via a int pointer
    
*/
/******************************************************************************/
int *get_matrix(const char *filename, int *width)
{
  int value, *matrix;
  FILE *fp;
  
    /* Open the file in text/translated mode */  
  fp = fopen(filename, "rt");
  if (!fp)
  {
    printf("Can't open file: %s\n", filename);
    exit(-3);
  }
  
    /* Read the width and allocate the matrix */
  fscanf(fp, "%d", width);
  matrix = malloc(*width * *width * sizeof(int));
  if (!matrix)
  {
    printf("Can't malloc matrix\n");
    fclose(fp);
    exit (-2);
  }
  
    /* Read the vaules and put in the matrix */
  while (!feof(fp))
  {
    int result = fscanf(fp, "%d", &value);
    if (result == -1)
      break;
    *matrix++ = value;
  }
  fclose(fp);
  
    /* Return the address of the matrix */
  return matrix - (*width * *width);
}

/******************************************************************************/
/*!
  \brief Prints out a matrix given a width
    
  \param matrix  - The matrix to print out

  \param width   - the width of the matrix
    
*/
/******************************************************************************/
void print_matrix(int *matrix, int width)
{
  int i, size = width * width;
  for (i = 0; i < size; i++)
  {
    printf("%8i", matrix[i]);
    if ( (i + 1) % width == 0)
      printf("\n");
  }
  printf("\n");
}

/******************************************************************************/
/*!
  \brief Calculates the square of a matrix using child process's 
    
  \param argc  - The number of arguments provided

  \param argv  - An array of char pointers that holds all the arguments 

  \return 1 if there was an error and 0 if it was successful.
    
*/
/******************************************************************************/
int main(int argc, char **argv) 
{
  int width,       /* width of the matrix            */
      shmid,       /* Shared Memory ID               */
      exec_retval, /* return value of execv(failure) */
      i;           /* loop counter                   */
  int col = 1;     /* col# used in setting up args[] */
  int row = 1;     /* row# used in setting up args[] */
  int *chpid;      /* array of child pids            */
  int *buffer;     /* shared memory area             */
  char *args[6];   /* child arguments                */
  char *child_exe; /* name of the child exe          */
  key_t key = 123; /* key for shmget                 */ 

  /* Matrix read in from file        */
  /* get_matrix() allocates memory!! */
  int *matrix = get_matrix(argv[1], &width); 
  /* The size to make buffer  */ 
  int buffer_size = 2 * width * width * sizeof(int) + sizeof(int);
  
    /* Setting up shared memory */
  shmid = shmget(key, buffer_size, 0600 | IPC_CREAT);
  buffer = (int *)shmat(shmid, NULL, 0);

  if (argc < 3)
  {
    printf("Insufficient parameters supplied\n");
    return 1;
  }
    
    /* Allocating argumets for children */
  for(i = 0; i < 5; i++)
    args[i] = malloc(sizeof(char) * MAX_ARG_SIZE);
  args[5] = NULL;
  
    /* The user-supplied the name of the child exe */
  child_exe = argv[2];

    /* Allocating array of child process IDs */
  chpid = malloc(width * width * sizeof(int));

  if(chpid == NULL)
  {
    /* Malloc has failed */
    printf("Can't malloc chpid\n");

    /* clean up */
    free(matrix);
    for(i = 0; i < 6; i++)
      free(args[i]);
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, 0);

    return 1;
  }
    /* Print the matrix */
  print_matrix(matrix, width);
  
    /* Copying matrix data into shared memory */
  buffer[0] = width;
  for(i = 1; i <= (width * width); i++)
    buffer[i] = matrix[i - 1];
   
    /* Setting up child args */
  sprintf(args[0], "%s", child_exe);
  sprintf(args[1], "%i", shmid);

    /* Fork a child for each matrix entry     */
  for (i = 0; i < width * width; i++)
  {
    /* Child Number */
    sprintf(args[2], "%i", i + 1);
    
    /* Assigning rows and columns to children */
    if(col - 1 == width)
    {
      row++;
      col = 1;
    }
    sprintf(args[3], "%i", row);
    sprintf(args[4], "%i", col);
    col++;
    
    /* Forking children to run child_exe */
    chpid[i] = fork();
    if(chpid[i] == 0)
      exec_retval = execv(child_exe, args);
    
    if(chpid[i] == 0 || chpid[i] == -1)
    {
      /* Exec has failed or fork has failed, find out which */
      if(chpid[i] == 0)
        printf("Exec has failed. retval = %i\n", exec_retval);
      if(chpid[i] == -1)
        printf("Fork has failed to create child process\n");

      perror("Message");

      /* Clean up */
      free(matrix);
      free(chpid);
      for(i = 0; i < 6; i++)
        free(args[i]);
      shmdt(buffer);
      shmctl(shmid, IPC_RMID, 0);

      return 1;
    }
  }

  /* Wait for children */
  for(i = 0; i <= width * width; i++)
  {
    int status;
    /* Wait for any child to finish */
    waitpid(-1, &status, 0);

    /* Child returns any value to indicate success */
    /* No value returned means there was an error  */
    if(!WIFEXITED(status))
    {
      printf("Child ended abnormally\n");

      /* clean up */
      free(matrix);
      free(chpid);
      for(i = 0; i < 6; i++)
        free(args[i]);
      shmdt(buffer);
      shmctl(shmid, IPC_RMID, 0);

      /* One child failure results in an incorrect calculation */
      /* So we abort                                           */
      return 0;
    }
  }
    /* Print matrix from shared buffer */
  print_matrix(buffer + 1 + width * width, width);
  
  /* Cleanup */
  free(matrix);
  free(chpid);
  for(i = 0; i < 6; i++)
    free(args[i]);
  shmdt(buffer);
  shmctl(shmid, IPC_RMID, 0);
  
  return 0;
}

