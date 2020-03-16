#include <stdlib.h>
#include <stdio.h>

int strcompare(char[], char[]);
int strcompare2(char[], char[]);

int main(int argc, char *argv[])
{
	printf("%d", strcompare2("Abd", "Abe"));
	return EXIT_SUCCESS;
}

int strcompare(char *str1, char *str2) 
{	
	int i = 0;
	while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0')
		i++;
	return str1[i] - str2[i];
}

int strcompare2(char *str1, char *str2)
{
	char *sp1 = str1, *sp2 = str2;
	while (*sp1 == *sp2)
	{
		sp1++;
		sp2++;
	}
		
	return *sp1 - *sp2;
}
