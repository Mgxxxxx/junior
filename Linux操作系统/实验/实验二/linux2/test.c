#include<stdio.h>
#include<stdlib.h>

int main()
{
	char *a;
	printf("%p\n",a);
	char c = 'A';
	a=&c;
	printf("%p\n",a);
	return 0;
}
