#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void upcase(char *inputstring,char **newstring)
{
	int counter;
	if(!*newstring)	//若未给newstring为NULL
	{
		//为newstring分配空间
		if(!(*newstring=realloc(NULL,strlen(inputstring)+1)))
		{
			printf("ERROR ALLOCATING MEMORY!\n");
			exit(255);
		}
	}
	else	//newstring不为NULL
	{
		//为newstring修改空间大小
		if(!(*newstring=realloc(*newstring,strlen(inputstring)+1)))
		{
			printf("ERROR ALLOCATING MEMORY!\n");
			exit(255);
		}
	}
	//把inputstring的内容复制到*newstring中
	strcpy(*newstring,inputstring);
	//遍历字符串，把小写改为大写
	for(counter=0;counter<strlen(*newstring);counter++)
	{
		if((*newstring)[counter]>97&&(*newstring)[counter]<=122)
				(*newstring)[counter]-=32;
	}
}

int main(void)
{
	char *string=NULL;
	upcase("Hello",&string);
	printf("str1=%s\n",string);
	upcase("Goodbye",&string);
	printf("str2=%s\n",string);
	free(string);
	return 0;
}



