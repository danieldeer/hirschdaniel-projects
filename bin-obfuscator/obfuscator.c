#include <stdio.h>

char invert(char c)
{
	return ~c;
}

int main()
{
	int c;
	long counter = 0;
	while((c = getchar()) != EOF)
	{
		putchar(invert(c));
		counter++;
	}
	putchar(EOF);
	printf("size %ld bytes\n", counter);
}
