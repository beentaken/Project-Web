
int main(void)
{
	int a = 0;   // int
	auto b = a;  // int

	int* c = &a; // int*
	auto d = c;  // int*
	auto* e = c; // int*

	int& f = a;  // int&
	auto g = f;  // int
	auto& h = f; // int&

	decltype(a) i = 10;

	return 0;
}