// timing.cpp
// -- elementary operation execution times
// cs200 6/15

#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cfloat>
#include <climits>
using namespace std;


int main(void) {
  const int VALUE_COUNT = 10000,
            COUNT = 10000;

  // some random integers
  srand(unsigned(time(0)));
  int *ivalues1 = new int[VALUE_COUNT],
      *ivalues2 = new int[VALUE_COUNT];
  for (int n=0; n < 2; ++n) {
    int *ivalues = (n == 0) ? ivalues1 : ivalues2;
    for (int i=0; i < VALUE_COUNT; ++i) {
      int x;
      unsigned char *bytes = reinterpret_cast<unsigned char*>(&x);
      for (int k=0; k < sizeof(int); ++k)
        bytes[k] = rand()%256;
      ivalues[i] = x;
    }
  }

  // get some random floating point numbers
  float *fvalues1 = new float[VALUE_COUNT],
        *fvalues2 = new float[VALUE_COUNT];
  for (int n=0; n < 2; ++n) {
    float *fvalues = (n == 0) ? fvalues1 : fvalues2;
    int i=0;
    while (i < VALUE_COUNT) {
      float x;
      unsigned char *bytes = reinterpret_cast<unsigned char*>(&x);
      for (int k=0; k < sizeof(float); ++k)
        bytes[k] = rand()%256;
      if (FLT_MIN <= x && x <= FLT_MAX) {
        fvalues[i] = x;
        ++i;
      }
    }
  }

  //cout << CLOCKS_PER_SEC << endl;

  // baseline time: integer addition
  clock_t t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      int x = ivalues1[j] + ivalues2[j];
  }
  clock_t t_end = clock();
  double time_iadd = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);

  cout << "*** All timing values are relative to integer addition ***" << endl;
  cout << fixed << setprecision(2);
  cout << left;

  // integer increment
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      int x = ++(ivalues1[j]);
  }
  t_end = clock();
  double time_inc = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "integer increment: "
       << (time_inc/time_iadd) << endl;

  // integer decrement
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      int x = --(ivalues1[j]);
  }
  t_end = clock();
  double time_dec = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "integer decrement: "
       << (time_dec/time_iadd) << endl;

  // integer subtraction
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      int x = ivalues1[j] - ivalues2[j];
  }
  t_end = clock();
  double time_isub = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "integer subtraction: "
       << (time_isub/time_iadd) << endl;

  // integer multiplication
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      int x = ivalues1[j] * ivalues2[j];
  }
  t_end = clock();
  double time_imul = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "integer multiplication: "
       << (time_imul/time_iadd) << endl;

  // integer division
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      int x = ivalues1[j] / ivalues2[j];
  }
  t_end = clock();
  double time_idiv = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "integer division: "
       << (time_idiv/time_iadd) << endl;

  // integer modulus
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      int x = ivalues1[j] % ivalues2[j];
  }
  t_end = clock();
  double time_imod = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "integer modulus: "
       << (time_imod/time_iadd) << endl;

  // floating point addition
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      float x = fvalues1[j] + fvalues2[j];
  }
  t_end = clock();
  double time_add = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "floating point addition: "
       << (time_add/time_iadd) << endl;

  // floating point multiplication
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      float x = fvalues1[j] * fvalues2[j];
  }
  t_end = clock();
  double time_mul = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "floating point multiplication: "
       << (time_mul/time_iadd) << endl;

  // floating point subtraction
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      float x = fvalues1[j] - fvalues2[j];
  }
  t_end = clock();
  double time_sub = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "floating point subtraction: "
       << (time_sub/time_iadd) << endl;

  // floating point division
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      float x = fvalues1[j] / fvalues2[j];
  }
  t_end = clock();
  double time_div = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "floating point division: "
       << (time_div/time_iadd) << endl;

  // floating point square root
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      float x = sqrt(fvalues1[j]);
  }
  t_end = clock();
  double time_sqrt = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "floating point sqrt: "
       << (time_sqrt/time_iadd) << endl;

  // floating point sine function
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      float x = sin(fvalues1[j]);
  }
  t_end = clock();
  double time_sin = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "floating point sine: "
       << (time_sin/time_iadd) << endl;

  // floating point exponential function
  t_bgn = clock();
  for (int i=0; i < COUNT; ++i) {
    for (int j=0; j < VALUE_COUNT; ++j)
      float x = exp(fvalues1[j]);
  }
  t_end = clock();
  double time_exp = double(t_end-t_bgn)/double(CLOCKS_PER_SEC);
  cout << setw(31) << "floating point exp: "
       << (time_exp/time_iadd) << endl;

  delete[] ivalues1;
  delete[] ivalues2;
  delete[] fvalues1;
  delete[] fvalues2;
  return 0;
}
