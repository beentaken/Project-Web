// HalfHalfPlaneSimpleTest.cpp
// -- simple test of the plane utility functions
// cs200 10/15

#include <iostream>
#include <iomanip>
#include "HalfPlane.h"
using namespace std;


int main(void) {

  {
    Hcoords h(2,-1,4);
    Point P(-5,-8);
    cout << dot(h,P) << endl;
  }

  {
    Vector n(8,-3);
    Point C(2,9);
    Hcoords h = HalfPlane(n,C);
    cout << boolalpha;
    cout << Hcoords::Near(dot(h,C),0)
         << ' ' << (dot(h,C+n) > 0)
         << ' ' << (dot(h,C-n) < 0) << endl;
  }

  {
    Point A(28,44),
          B(61,53),
          P(54,21);
    Hcoords h = HalfPlane(A,B,P);
    cout << boolalpha;
    cout << Hcoords::Near(dot(h,A),0)
         << ' ' << Hcoords::Near(dot(h,B),0)
         << ' ' << (dot(h,P) < 0) << endl;
  }

  {
    Hcoords h(-3,1,7);
    Point P(11,5),
          Q(1,10);
    Interval I = ClipSegment(h,P,Q);
    cout << '[' << I.bgn << ',' << I.end << ']' << endl;
  }

  return 0;
}

