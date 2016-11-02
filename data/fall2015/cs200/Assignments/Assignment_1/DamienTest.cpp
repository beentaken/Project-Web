// AffineTest.cpp
// cs200 1/14

#include <iostream>
#include <iomanip>
#include <cmath>
#include "Affine.h"

#define PI 3.14159265358979323846264338327950
using namespace std;


ostream& operator<<(ostream& s, const Hcoords& v) {
  return s << '[' << v.x << ',' << v.y << ',' << v.w << ']';
}


ostream& operator<<(ostream& s, const Affine& A) {
  s << '{';
  for (int i=0; i < 3; ++i) {
    s << '{';
    for (int j=0; j < 3; ++j)
      s << A[i][j] << ((j < 2) ? ',' : '}');
    s << ((i < 2) ? ',' : '}');
  }
  return s;
}


int main(void)
{
  //Constructor Test
  //////////////////////////////////////////////////////////////////////////////
  cout << "--Constructor Test-------------------------" << endl;
  Hcoords A;
  cout << "Hcoords Default Constructor" << endl; 
  cout << A << endl;
  
  Point B;
  cout << "Point Default Constructor" << endl; 
  cout << B << endl;
  
  Vector C;
  cout << "Vector Default Constructor" << endl; 
  cout << C << endl;
  
  Affine AA;
  cout << "Affine Default Constructor" << endl; 
  cout << AA << endl;
  
  Hcoords D(1,2,3);
  cout << "Hcoords Constructor" << endl; 
  cout << D << endl;
  
  Point E(1,2);
  cout << "Point Constructor" << endl;
  cout << E << endl;
  
  Vector F(1,2);
  cout << "Vector Constructor" << endl; 
  cout << F << endl;
  
  Vector G(1,2),H(3,4);
  Point I(5,6);
  Affine AB(G,H,I);
  cout << "Affine Constructor" << endl; 
  cout << AB << endl;
  cout << "-------------------------------------------" << endl << endl; 
  //////////////////////////////////////////////////////////////////////////////
  
  
  //Normalize Test
  //////////////////////////////////////////////////////////////////////////////
  cout << "--Normalize Test---------------------------" << endl;
  Vector z(-4,5); 
  cout << "Vector" << endl; 
  cout << z << endl;        
  z.Normalize(); 
  cout << "Normalized Vector" << endl; 
  cout << z << endl;     
  cout << "-------------------------------------------" << endl << endl;  
  //////////////////////////////////////////////////////////////////////////////
  
  
  //Operators
  //////////////////////////////////////////////////////////////////////////////
  cout << "--Operators--------------------------------" << endl;
  cout<< "Operator+(u,v)" << endl;
  Hcoords J(2,3,0),K(2,2,0);
  Hcoords L = J + K;
  cout << L << endl;
  
  cout<< "Operator-(u,v)" << endl;
  L = J - K;
  cout << L << endl;
  
  cout<< "Operator-(v)" << endl;
  L = -J;
  cout << L << endl;
  
  cout<< "Operator*(r,v)" << endl;
  float r = 3;
  L = 3 * J;
  cout << L << endl;
  
  cout<< "Operator*(A,v)" << endl;
  Vector X(3,0),Y(0,3);
  Point W(0,0);
  Affine Scaler(X,Y,W);
  L = Scaler * J;
  cout << L << endl;
  
  cout<< "Operator*(A,B)" << endl;
  Vector XX(1,0),YY(0,1);
  Point WW(4,-5);
  Affine Tran(XX,YY,WW);
  Affine Result;
  Result = Tran * Scaler;
  cout << Result << endl;
  cout << "-------------------------------------------" << endl << endl;  
  //////////////////////////////////////////////////////////////////////////////
  
  
  //Others
  //////////////////////////////////////////////////////////////////////////////
  cout << "--Others-----------------------------------" << endl;
  cout << "dot(u,v)" << endl;
  float dotres = dot(J,K);
  cout << dotres << endl;
  
  cout << "abs(v)" << endl;
  Vector absvec(3,4);
  float absres = abs(absvec);
  cout << absres << endl;
  absvec.Normalize();
  absres = abs(absvec);
  cout << absres << endl;
  
  cout << "Rot(t)" << endl;
  float Deg = PI/4; //45 Degrees
  Affine RotRes = Rot(Deg);
  cout << RotRes << endl;
  
  cout << "Trans(v)" << endl;
  Vector Transvec(2,-6);
  Affine TransTrans = Trans(Transvec); //Translation Transform in case you wondered.
  cout << TransTrans << endl;
  
  cout << "Scale(r)" << endl;
  float ScaleFactor = 5;
  Affine ScaleTrans = Scale(ScaleFactor);
  cout << ScaleTrans << endl;
  
  cout << "Scale(rx, ry)" << endl;
  float ScaleFactor2 = 10;
  ScaleTrans = Scale(ScaleFactor, ScaleFactor2);
  cout << ScaleTrans << endl;
  cout << "-------------------------------------------" << endl << endl;
  //////////////////////////////////////////////////////////////////////////////
  return 0;
  
  
}

