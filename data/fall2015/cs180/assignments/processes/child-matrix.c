/******************************************************************************/
/*
  \file   child-matrix.c
  \author Damien Baca
  \par    email: damien.baca\@digipen.edu
  \par    DigiPen login: damien.baca
  \par    Course: CS180
  \par    Assignment #3
  \date   Due - 11/01/2015
  \brief  Calclates part of a square matrix for parent-matrix.c
          - main
*/
/******************************************************************************/
#include <sys/shm.h> /* shmat, shmdt */
#include <stdlib.h>  /* exit         */
#include <stdio.h>   /* sscanf       */

/*
  Arguments
  ---------------------------------------------------------
  [0] - The name of the program on the disk
  [1] - The id of the shared memory
  [2] - The child number
  [3] - The row from the original matrix to work with
  [4] - The column from the original matrix to work with
  [5] - NULL
  ---------------------------------------------------------
*/

/******************************************************************************/
/*!
  \brief Calculates the square of a matrix using child process's 
    
  \param argc  - The number of arguments provided

  \param argv  - An array of char pointers that holds all the arguments 

  \return 1 if calculation was succesfull and nothing if there is an error
    
*/
/******************************************************************************/
int main(int , char const *argv[])
{
  int shmid;      /* Shared Memory ID               */
  int i;          /* loop counter                   */
  int child_row;  /* child's assigned row#          */
  int child_col;  /* child's assigned column#       */
  int child_num;  /* child's number                 */
  int result = 0; /* result of child's calculation  */
  int width;      /* width of the matrix            */
  int *row;       /* child's assigned row           */
  int *col;       /* child's assigned column        */
  int *buffer;    /* shared memory                  */

  sscanf(argv[1], "%i", &shmid);
  buffer = shmat(shmid, NULL, 0);
  width = buffer[0];
  row = malloc(sizeof(int) * width);
  col = malloc(sizeof(int) * width);

  if(argc < 4)
    return 0;
  
  /* Get assigned row#, column#, and child# */
  sscanf(argv[3], "%i", &child_row);
  sscanf(argv[4], "%i", &child_col);
  sscanf(argv[2], "%i", &child_num);

  /* Read in the row    */
  for(i = 1; i <= width; i++)
    row[i - 1] = buffer[i + ((child_row - 1 ) * width)];
  /* Read in the column */
  for (i = 1; i <= width ; i++)
    col[i - 1] = buffer[child_col + ((i - 1) * width)];

  /* Child's calculation */
  for(i = 0; i < width; i++)
      result += row[i] * col[i];
  
  /* Use child_num to write the result to the proper place */
  /* in shared memory                                      */ 
  buffer[width * width + child_num] = result;

  /* Clean up */
  free(col);
  free(row);
  shmdt(buffer);


  /* Exit with non-zero value to indicate success */
  exit(1);
}
