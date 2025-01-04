#include <stdio.h>
#include <stdlib.h>


void encrypt(FILE* secret_file, FILE* encrypted_file, FILE* key_file)
{
	int c;
	while((c=fgetc(secret_file)) != EOF)
	{
		int key = rand();
		int encrypted_c = c ^ key;

		fputc(key, key_file);
		fputc(encrypted_c, encrypted_file);
	}
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Provide the to-be encrypted file\n");
	}
	else
	{
		char* secret_file_name = argv[1];
		FILE* secret_file = fopen(secret_file_name, "r");
		FILE* encrypted_file = fopen("crypt.out", "w");
		FILE* key_file = fopen("key.out", "w");

		encrypt(secret_file, encrypted_file, key_file);

		fclose(secret_file);
		fclose(encrypted_file);
		fclose(key_file);
	}
}
