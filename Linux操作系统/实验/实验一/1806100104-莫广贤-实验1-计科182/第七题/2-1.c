#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
int pid1,pid2;

int main()
{
	int fd[2];
	char outpipe[100],inpipe[100];
	pipe(fd);	//创建一个管道
	while((pid1=fork())==-1);
	if(pid1==0)
	{
		lockf(fd[1],1,0);		//互斥，只允许当前进程访问
		//把字符串放入字符数组outpipe中
		sprintf(outpipe,"Child 1 is sending a message!");
		write(fd[1],outpipe,50);	//向管道写入50字节的字符串
		sleep(5);		//阻塞5s
		lockf(fd[1],0,0);
		exit(0);
	}
	else
	{
		while((pid2=fork())==-1);
		if(pid2==0)
		{
			lockf(fd[1],1,0);
			sprintf(outpipe,"Child 2 is sending a message!");
			write(fd[1],outpipe,50);
			sleep(5);
			lockf(fd[1],0,0);
			exit(0);
		}
		else
		{
			wait(0);		//同步处理，等待子进程结束
			read(fd[0],inpipe,50);		//从管道中读出长50字节的串
			printf("%s\n",inpipe);
			wait(0);
			read(fd[0],inpipe,50);
			printf("%s\n",inpipe);
			exit(0);
		}
	}
}
