/******************************************************************************/
/*
  \file   matrix-thread.c
  \author Damien Baca
  \par    email: damien.baca\@digipen.edu
  \par    DigiPen login: damien.baca
  \par    Course: CS180
  \par    Assignment #4
  \date   Due - 11/09/2015
  \brief  Calclates part of a square matrix for main-thread.c
          - threadcalc
*/
/******************************************************************************/
#include "matrix-thread.h"

/******************************************************************************/
/*!
  \brief Calculates the square of a matrix using threads
    
  \param data  - All arguments are passed in the data_struct data

  \return NULL
    
*/
/******************************************************************************/
void *threadcalc(void *data)
{
  /* Converting void pointer to data_struct pointer */
  data_struct *ds     =  (data_struct *)data;

  /* Making Local Copies Of All Data */
  int width           =  ds->width_;
  int threadnum       =  ds->threadnum_;
  int thread_row      =  ds->row_;
  int thread_col      =  ds->col_;
  int *input_matrix   = *(ds->input_matrix_);
  int *result_matrix  = *(ds->result_matrix_);
  
  int i;          /* Loop Counter                   */
  int result = 0; /* Result of child's calculation  */
  int *row;       /* Child's assigned row           */
  int *col;       /* Child's assigned column        */

  row = malloc(sizeof(int) * width);
  if(row == NULL)
  {
    printf("ERROR: Memory Allocation Failure (row)\n");
    return NULL;
  }

  col = malloc(sizeof(int) * width);
  if(col == NULL)
  {
    printf("ERROR: Memory Allocation Failure (col)\n");
    free(row);
    return NULL;
  }
  
  /* Read in the row    */
  for(i = 1; i <= width; i++)
    row[i - 1] = input_matrix[i + ((thread_row - 1 ) * width) - 1];

  /* Read in the column */
  for(i = 1; i <= width; i++)
    col[i - 1] = input_matrix[thread_col + ((i - 1) * width) - 1];
  
  /* Child's calculation */
  for(i = 0; i < width; i++)
    result += row[i] * col[i];

  /* Writing the result to result_matrix */
  result_matrix[threadnum] = result;
  
  free(row);
  free(col);
  return NULL ;
}
