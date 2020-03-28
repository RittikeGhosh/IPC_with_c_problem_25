#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main()
{
	char str[100];
	printf("Enter the string [MAX = 100] : ");
	scanf("%[^\n]%*c", str);
	printf("%s\n", str);

	return 0;
}
