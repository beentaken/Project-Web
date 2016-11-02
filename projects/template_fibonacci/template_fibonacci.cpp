// Fibonacci.cpp
//
// Fibonacci numbers using templates.
//
// 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610,
// 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025,
// 121393, ...[1]
//
// DJD
#include <iostream>

using namespace std;

template <unsigned N>
struct Fibonacci
{
enum { value = Fibonacci<N-1>::value + Fibonacci<N-2>::value};
};
template <>
struct Fibonacci<0>
{
enum { value = 0 };
};
template <>
struct Fibonacci<1>
{
enum { value = 1 };
};
int main()
{
	cout << Fibonacci<1>::value << endl;
	cout << Fibonacci<2>::value << endl;
	cout << Fibonacci<3>::value << endl;
	cout << Fibonacci<4>::value << endl;
	cout << Fibonacci<5>::value << endl;
	cout << Fibonacci<6>::value << endl;
	cout << Fibonacci<7>::value << endl;
	cout << Fibonacci<8>::value << endl;
	cout << Fibonacci<9>::value << endl;
	cout << Fibonacci<10>::value << endl;
	cout << Fibonacci<11>::value << endl;
	cout << Fibonacci<12>::value << endl;
	cout << Fibonacci<13>::value << endl;
	cout << Fibonacci<14>::value << endl;
	cout << Fibonacci<15>::value << endl;
	cout << Fibonacci<16>::value << endl;
	cout << Fibonacci<17>::value << endl;
	cout << Fibonacci<18>::value << endl;
	cout << Fibonacci<19>::value << endl;
	cout << Fibonacci<20>::value << endl;

	return 0;
}