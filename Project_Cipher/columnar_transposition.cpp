#include <iostream>
#include <cstdio>  // fopen(), fclose(), fgets(), feof()
#include <cstring> // strcmp(), strlen()
#include <cstdlib> // atoi() 

#define MAX_MSG_LENGTH 1000 

using namespace std;

string msg;
int size = 0;

void usage(char *exe)
{
	cout << "Usage: "<< exe <<" [option]" << endl;
	cout << "Options:" << endl;
	cout << "  'Encrypt'  Usage: "<< exe <<" Encrypt [Encryption Key] [Message File]" << endl;
	cout << "    ---> Encrypts the file using the given encryption key" << endl << endl;

	cout << "  'Decrypt'  Usage: "<< exe <<" Decrypt [Encryption Key] [Message File]" << endl;
	cout << "    ---> Decrypts the file using the given encryption key" << endl << endl;

	cout << "  'Test'     Usage: "<< exe <<" Test" << endl;
	cout << "    ---> Runs test cases" << endl << endl;
}

void stripnl(char *line)
{
  size_t len = strlen(line);
  line[len - 1] = 0;
}

void Display_textonly()
{
	cout << msg << endl;
}

void Display_CT(int key)
{

}

void CT_Encrypt(int key)
{
	
}

void CT_Decrypt(int key)
{
	
}

void Accquire_Msg(const char *filename)
{
	FILE *fp = fopen(filename, "rt");
	if(!fp)
		cout << "Error: Failed to open " << filename << endl;

	char buffer[MAX_MSG_LENGTH];
	while(!feof(fp))
	{
		fgets(buffer, MAX_MSG_LENGTH, fp);
		stripnl(buffer);
		msg = buffer;
		break;
	}
	fclose(fp);
}

void Test(void)
{
	
}

int main(int argc, char *argv[])
{
	if(argv[1] != 0) 
	{
		if(!strcmp(argv[1], "Test"))
		{
			Test();
			return 0;
		}

		if(!strcmp(argv[1], "Encrypt") && argc > 3) 
		{
			Accquire_Msg(argv[3]);
			CT_Encrypt(atoi(argv[2]));
			Display_textonly();
			return 0;
		}

		if(!strcmp(argv[1], "Decrypt") && argc > 3) 
		{
			Accquire_Msg(argv[3]);
			CT_Decrypt(atoi(argv[2]));
			Display_textonly();
			return 0;
		}
	}

	usage(argv[0]);
	return 0;
}