#include "Matrix2D.h"
#define PI      3.1415926535897932384626433832795


/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult)
{
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;
	
	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = 0;
	
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}		

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	pResult->m[0][0] = pMtx->m[0][0];
	pResult->m[0][1] = pMtx->m[1][0];
	pResult->m[0][2] = pMtx->m[2][0];

	pResult->m[1][0] = pMtx->m[0][1];
	pResult->m[1][1] = pMtx->m[1][1];
	pResult->m[1][2] = pMtx->m[2][1];
	
	pResult->m[2][0] = pMtx->m[0][2];
	pResult->m[2][1] = pMtx->m[1][2];
	pResult->m[2][2] = pMtx->m[2][2];
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
	int i;
	int j;

	struct Matrix2D Result;

	for (j = 0; j < 3; j++)
	{
		for (i = 0; i < 3; i++)
			Result.m[j][i] = ((pMtx0->m[j][0] * pMtx1->m[0][i])
			                + (pMtx0->m[j][1] * pMtx1->m[1][i])
			                + (pMtx0->m[j][2] * pMtx1->m[2][i]));
	}

	pResult->m[0][0] = Result.m[0][0];
	pResult->m[0][1] = Result.m[0][1];
	pResult->m[0][2] = Result.m[0][2];
					 
	pResult->m[1][0] = Result.m[1][0];
	pResult->m[1][1] = Result.m[1][1];
	pResult->m[1][2] = Result.m[1][2];
					
	pResult->m[2][0] = Result.m[2][0];
	pResult->m[2][1] = Result.m[2][1];
	pResult->m[2][2] = Result.m[2][2];
}

// ---------------------------------------------------------------------------

/*1
This function creates a translation matrix from x *p y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = x;

	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = y;

	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;

}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x *p y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	pResult->m[0][0] = x;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;

	pResult->m[1][0] = 0;
	pResult->m[1][1] = y;
	pResult->m[1][2] = 0;

	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{

	pResult->m[0][0]  =  cosf(Angle * (PI/180));
	pResult->m[0][1]  = sinf(Angle * (PI / 180));
	pResult->m[0][2]  =  0;		  
					  			  
	pResult->m[1][0]  = -sinf(Angle * (PI / 180));
	pResult->m[1][1]  = cosf(Angle * (PI / 180));
	pResult->m[1][2]  =  0;
					  
	pResult->m[2][0]  =  0;
	pResult->m[2][1]  =  0;
	pResult->m[2][2]  =  1;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{

	pResult->m[0][0] =  cosf(Angle);
	pResult->m[0][1] =  sinf(Angle);
	pResult->m[0][2] =  0;

	pResult->m[1][0] =  -sinf(Angle);
	pResult->m[1][1] =  cosf(Angle);
	pResult->m[1][2] =  0;

	pResult->m[2][0] =  0;
	pResult->m[2][1] =  0;
	pResult->m[2][2] =  1;

}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
	struct Vector2D Result;

	Result.x = (pMtx->m[0][0] * pVec->x) + (pMtx->m[0][1] * pVec->y) + (pMtx->m[0][2] * 1);
	Result.y = (pMtx->m[1][0] * pVec->x) + (pMtx->m[1][1] * pVec->y) + (pMtx->m[1][2] * 1);

	pResult->x = Result.x;
	pResult->y = Result.y;
}

// ---------------------------------------------------------------------------
