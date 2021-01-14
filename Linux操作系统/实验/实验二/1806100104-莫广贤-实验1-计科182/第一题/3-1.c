#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* upcase(char *inputstring)
{
	char* newstring;
	int counter;
	//为newstring分配空间
	if(!(newstring=malloc(strlen(inputstring)+1)))
	{
		printf("ERROR ALLOCATING MEMORY!\n");
		exit(255);
	}
	//把inputstring的内容复制到newstring中
	strcpy(newstring,inputstring);
	//遍历字符串，把小写改为大写
	for(counter=0;counter<strlen(newstring);counter++)
	{
		if(newstring[counter]>97&&newstring[counter]<=122)
				newstring[counter]-=32;
	}
	//返回修改过的字符数组
	return newstring;
}

int main(void)
{
	char *str1,*str2;
	str1=upcase("Hello");
	str2=upcase("Goodbye");
	printf("str1=%s,str2=%s\n",str1,str2);
	free(str1);
	free(str2);
	return 0;
}

