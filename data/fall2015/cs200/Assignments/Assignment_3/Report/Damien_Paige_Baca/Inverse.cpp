#include "Affine.h"

Affine Inverse(const Affine& A)
{
	Affine Result;
	Affine L;
	Affine T;
	float LCoeff = 0;
	Vector vec(A[0][2], A[1][2]);
  
	// Computing Inverse of L
	LCoeff = 1/((A[0][0] * A[1][1]) - (A[0][1] * A[1][0]));
	L[1][1] = LCoeff *  A[0][0];
	L[0][0] = LCoeff *  A[1][1];
	L[0][1] = LCoeff * -A[0][1];
	L[1][0] = LCoeff * -A[1][0];
	
	Result = L * Trans(-vec);
	return Result;
}