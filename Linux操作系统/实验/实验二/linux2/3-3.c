#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define DATASIZE 10

typedef struct stringdata
{
	char *string;	//存储字符串
	int iscontinuing;	//当前字符串是否结束
	struct stringdata *next;	//后继指针
}mydata;

mydata *append(mydata *start,char *input)
{
	mydata *cur=start,*prev=NULL, *new;
	while(cur)
	{
		prev=cur;
		cur=cur->next;
	}
	cur=prev;
	//给new分配新空间，存放字符串
	new=malloc(sizeof(mydata));
	if(!new)
	{
		printf("COULD NOT ALLOCATE MEMORY!\n");
		exit(255);
	}
	//连接两个字符串
	if(cur)	cur->next=new;
	else	start=new;
	cur=new;
	if(!(cur->string=malloc(sizeof(input)+1)))
	{
		printf("ERROR ALLOCATE MEMORY!\n");
		exit(255);
	}
	//把输入的字符串复制到开辟的新空间
	strcpy(cur->string,input);
	//判断该行字符串是否结束，并设置iscontinuing
	cur->iscontinuing=!(input[strlen(input)-1]=='\n'||input[strlen(input)-1]=='\r');
	cur->next=NULL;
	return start;
}

void displaydata(mydata *start)
{
	mydata *cur;
	//记录有多少行和使用了多少个结构体
	int linecounter=0,structcounter=0;
	int newline=1;
	cur=start;
	//遍历链表,输出每行的字符串
	while(cur)
	{
		if(newline)	printf("LINE %d: ",++linecounter);
		structcounter++;
		printf("%s",cur->string);
		newline=!cur->iscontinuing;
		cur=cur->next;
	}
	printf("THIS DATA CONTATINED %d LINES AND WAS STORED IN %d STRUCTS.\n",linecounter,structcounter);
}

void freedata(mydata *start)
{
	mydata *cur,*next=NULL;
	cur=start;
	//遍历每个节点，并释放内存
	while(cur)
	{
		next=cur->next;
		free(cur->string);
		free(cur);
		cur=next;
	}
}

int main(void)
{
	char input[DATASIZE];
	mydata *start=NULL;
	printf("ENTER SOME DATA AND PRESS Ctrl+D WHEN DONE.\n");
	//接受输入的字符串
	while(fgets(input,sizeof(input),stdin))	start=append(start,input);
	displaydata(start);
	freedata(start);
	return 0;
}
