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

void Display_CC(int key)
{
	if(key == 0)
	{
		cout << "Unencrypted Msg:" << endl;
		cout << "	" << msg << endl << endl;
	}
	else
	{
		cout << "Encrypted Msg (Key = " << key << "):" << endl;
		cout << "	" << msg << endl << endl;
	}
}

void CC_Encrypt(int key)
{
	for (size_t i = 0; i < msg.size(); ++i) {
		int newchar = msg[i] + key;
		if(msg[i] == ' ')
			continue;

		if(newchar > 'z') {
			newchar -= 'z';
			newchar += 'a' - 1;
		}

		msg[i] = (char)newchar;
	}
}

void CC_Decrypt(int key)
{
	for (size_t i = 0; i < msg.size(); ++i) {
		int newchar = msg[i] - key;
		if(msg[i] == ' ')
			continue;

		if(newchar < 'a') {
        	newchar = 'a' - newchar;
        	newchar = 'z' - newchar + 1;
		}

		msg[i] = (char)newchar;
	}
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
	Accquire_Msg("Test.txt");
	cout << endl;
	Display_CC(0);

	for(int i = 1; i <= 26; i++)
	{
		cout << "-------------------------------------" << endl;
		cout << "Encrypting ...." << endl << endl;
		CC_Encrypt(i);
		Display_CC(i);

		cout << "Dencrypting ...." << endl << endl;
		CC_Decrypt(i);
		Display_CC(0);

	}
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
			CC_Encrypt(atoi(argv[2]));
			Display_textonly();
			return 0;
		}

		if(!strcmp(argv[1], "Decrypt") && argc > 3) 
		{
			Accquire_Msg(argv[3]);
			CC_Decrypt(atoi(argv[2]));
			Display_textonly();
			return 0;
		}
	}

	usage(argv[0]);
	return 0;
}