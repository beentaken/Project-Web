/******************************************************************************/
/*
  \file   matrix-thread.h
  \author Damien Baca
  \par    email: damien.baca\@digipen.edu
  \par    DigiPen login: damien.baca
  \par    Course: CS180
  \par    Assignment #4
  \date   Due - 11/09/2015
  \brief  The definition of the data_struct used in main-thread.c
          and matrix-thread.c.
*/
/******************************************************************************/
#include <stdio.h>         /* printf, fopen, fclose, fscanf */
#include <stdlib.h>        /* exit, malloc                  */
#include <pthread.h>       /* thread create/join/exit       */

/*! data_struct */
typedef struct
{
  int **input_matrix_;  /*!< Matrix to be squared   */
  int **result_matrix_; /*!< The resulting matrix   */
  int row_;             /*!< Child's assigned row # */
  int col_;             /*!< Child's assigned col # */
  int width_;           /*!< Width of the matrix    */
  int threadnum_;       /*!< Thread I.D.            */
}data_struct;

void *threadcalc(void *);
