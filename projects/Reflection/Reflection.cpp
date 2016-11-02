
#include "MetaVariable.h"
#include "MetaFunction.h"

int A = 1234;
int B = 0;;

short C = 4321;
short D = 0;

double E = 3.123;
double F = 0.0;

float G = 3.14159f;
float H = 0.0f;

char *String_A = (char*)"Defenestration";
char *String_B = (char*)"Necromimesis";

META_REGISTER_VAR(A);
META_REGISTER_VAR(B);

META_REGISTER_VAR(C);
META_REGISTER_VAR(D);

META_REGISTER_VAR(E);
META_REGISTER_VAR(F);

META_REGISTER_VAR(G);
META_REGISTER_VAR(H);

META_REGISTER_VAR(String_A);
META_REGISTER_VAR(String_B);


void Function_1(int A) {}
void Function_2(int A, float B) {}
void Function_3(int A, float B, double C) {}
void Function_4(int A, float B, double C, short D) {}
void Function_5(int A, float B, double C, short D, const char* E) {}

META_REGISTER_FUN(Function_1);
META_REGISTER_FUN(Function_2);
META_REGISTER_FUN(Function_3);
META_REGISTER_FUN(Function_4);
META_REGISTER_FUN(Function_5);


int main()
{
	std::cout << std::endl;

	GetMetaType(A).Serialize((char*)"Integer_Test", &A);
	GetMetaType(B).Deserialize((char*)"Integer_Test", &B);

	GetMetaType(C).Serialize((char*)"Short_Test", &C);
	GetMetaType(D).Deserialize((char*)"Short_Test", &D);

	GetMetaType(E).Serialize((char*)"Double_Test", &E);
	GetMetaType(F).Deserialize((char*)"Double_Test", &F);

	GetMetaType(G).Serialize((char*)"Float_Test", &G);
	GetMetaType(H).Deserialize((char*)"Float_Test", &H);

	GetMetaType(String_A).Serialize((char*)"String_Test", &String_A);
	GetMetaType(String_A).Deserialize((char*)"String_Test", &String_B);

	MetaPrintVariables(std::cout);
	MetaPrintFunctions(std::cout);
	return 0;
}