#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main()
{
	int pid,pid1,pid2;
	printf("parent pid is %d \n",getpid());	//getpid()获取当前进程的pid
	pid=fork();	//fork()创建一个新进程
	switch(pid)
	{
		case -1:	//创建进程失败
			printf("fork fail!\n");
			exit(1);
		case 0:	//对子进程进行操作
			execl("/bin/ls","ls","-1","-color",NULL);	//一个子进程执行ls命令
			printf("execl ls fail!\n");
			exit(1);
		default:	//父进程的操作
			printf("ls process pid is %d\n",pid);
			pid1=fork();
			switch(pid1)
			{
				case -1:
					printf("fork1 fail!\n");
					exit(1);
				case 0:
					printf("I am subprocess1!\n");
					exit(0);
				default:
					printf("subprocess1 pid is %d\n",pid1);	//一个子进程打印语句
					pid2=fork();
					switch(pid2)
					{
						case -1:
							printf("fork2 fail!\n");
							exit(1);
						case 0:
							execl("/bin/ps","ps","-l",NULL);	//一个子进程执行ps命令
							exit(0);
							printf("execl ps fail!\n");
						default:
							printf("ps process pid is %d\n",pid2);
							wait(NULL);	//这三个wait等待3个子进程结束
							wait(NULL);
							wait(NULL);
							printf("All subprocess completed!\n");
							exit(0);

					}
			}
	}
}
