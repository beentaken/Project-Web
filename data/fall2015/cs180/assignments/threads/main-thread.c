/******************************************************************************/
/*!
  \file   main-thread.c
  \author Damien Baca
  \par    email: damien.baca\@digipen.edu
  \par    DigiPen login: damien.baca
  \par    Course: CS180
  \par    Assignment #4
  \date   Due - 11/09/2015
  \brief  Calculates the square of a matrix using threads
          - get_matrix
          - print_matrix
          - main
*/
/******************************************************************************/
#include "matrix-thread.h" /* thread data struct   */

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
  int value, *matrix, result;
  FILE *fp;
  
  fp = fopen(filename, "rt");
  if (!fp)
  {
    printf("Can't open file: %s\n", filename);
    exit(-1);
  }
  
  result = fscanf(fp, "%d", width);
  if (result == -1)
  {
    printf("Can't read from file: %s\n", filename);
    fclose(fp);
    exit(-1);
  }

  matrix = malloc(*width * *width * sizeof(int));
  if (!matrix)
  {
    printf("Can't malloc matrix\n");
    fclose(fp);
    exit (-2);
  }
  
  while (!feof(fp))
  {
    result = fscanf(fp, "%d", &value);
    if (result == -1)
      break;
    *matrix++ = value;
  }
  fclose(fp);
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
  \brief Calculates the square of a matrix using threads
    
  \param argc  - The number of arguments provided

  \param argv  - An array of char pointers that holds all the arguments 

  \return 1 if there was an error and 0 if it was successful.
    
*/
/******************************************************************************/
int main(int argc, char **argv) 
{
  int i;                /* Loop Counter                             */
  int col = 1;          /* col # used in initializing data_struct   */
  int row = 1;          /* row # used in initializing data_struct   */
  int rowflag = 0;      /* flag used in initializing data_struct    */
  int ERR = 0;          /* Flag for checking function return values */
  int width;            /* width of the matrix                      */
  int matsize;          /* total matrix values                      */
  int *input_matrix;    /* the matrix read in                       */
  int *result_matrix;   /* threads will put results here            */
  pthread_t *thread_id;
  data_struct *ds;
  
  /* If we don't have at least two arguments then Err */
  if (argc < 2)
  {
    printf("ERROR: Insufficient parameters supplied\n");
    return 1;
  }
  
  /* Reading the input matrix from a file into it's memory. */
  input_matrix = get_matrix(argv[1], &width);
  matsize = width * width;
  
  /* Allocating array of thread id's */
  thread_id = malloc(sizeof(pthread_t) * matsize);
  if(thread_id == NULL)
  {
    printf("ERROR: Memory Allocation Failure (thread_id)\n");
    /* Clean Up */
    free(input_matrix);
    return 1;
  }
  
  /* Allocating array of data_structs */
  ds = malloc(sizeof(data_struct) * matsize);
  if(ds == NULL)
  {
    printf("ERROR: Memory Allocation Failure (ds)\n");
    /* Clean Up */
    free(thread_id);
    free(input_matrix);
    return 1;
  }
  
  /* Allocating result matrix */
  result_matrix = malloc(sizeof(int) * matsize);
  if(result_matrix == NULL)
  {
    printf("ERROR: Memory Allocation Failure (result_matrix)\n");
    /* Clean Up */
    free(thread_id);
    free(input_matrix);
    free(ds);
    return 1;
  }

  /* Printing the input matrix. */
  print_matrix(input_matrix, width);
  
  /* Creating all of the other threads and supplying them with */
  /* their parameters                                          */ 
  for(i = 0; i < matsize; i++)
  {
    ds[i].result_matrix_ = &result_matrix;
    ds[i].input_matrix_  = &input_matrix;
    ds[i].threadnum_     = i;
    ds[i].width_         = width;

    if(rowflag == width)
    {
      row++;
      col = 1;
      rowflag = 0;
    }
    ds[i].row_ = row;
    ds[i].col_ = col;
    rowflag++;
    col++;

    ERR = pthread_create(&thread_id[i], NULL, threadcalc, &ds[i]);
    if(ERR)
    {
      printf("ERROR: Failed to create new thread\n");
      /* Clean Up */
      free(result_matrix);
      free(thread_id);
      free(input_matrix);
      free(ds);
      return 1;
    }
  }

  /* Waiting for all of the threads to finish. */
  for(i = 0; i < matsize && !ERR; i++)
    ERR = pthread_join(thread_id[i], NULL);
  if(ERR)
  {
    /* None of the errors I saw for this function seemed applicable */
    /* Just in case                                                 */
    printf("ERROR: Failed to join terminated thread\n");
    /* Clean Up */
    free(result_matrix);
    free(thread_id);
    free(input_matrix);
    free(ds);
    return 1;
  }
    
  /* Printing the resulting squared matrix. */
  print_matrix(result_matrix, width);
  
  /* Cleaning up any memory or resources the main thread created. */
  free(result_matrix);
  free(thread_id);
  free(input_matrix);
  free(ds);

  return 0;
}