// InverseTest.cpp
// jsh 6/14

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Affine.h"
using namespace std;


float frand(void) {
  return float(rand())/float(RAND_MAX);
}


int main(void) {
  srand(unsigned(time(0)));
  const int COUNT = 100;

  bool pass = true;
  for (int n=0; pass && n < COUNT; ++n) {
    Affine A;
    for (int i=0; i < 2; ++i)
      for (int j=0; j < 3; ++j)
        A[i][j] = 1 - 2*frand();
    Affine Z = A * Inverse(A);
    for (int i=0; i < 3; ++i)
      Z[i][i] -= 1;
    float d = 0;
    for (int i=0; i < 2; ++i)
      for (int j=0; j < 3; ++j)
        d += pow(Z[i][j],2);
    pass = (d < 1e-5f);
  }
  if (!pass)
    cout << "     failed test" << endl;
  else
    cout << "     passed test" << endl;

  return 0;
}

