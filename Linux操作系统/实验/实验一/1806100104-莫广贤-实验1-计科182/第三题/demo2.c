#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void thread1(void)	//子线程1调用的函数
{
	int i;
	for(i=0;i<3;i++)	printf("This is pthread1.\n");
}

void thread2(void)	//子线程2调用的函数
{
	int i;
	for(i=0;i<3;i++)	printf("This is pthread2.\n");
	pthread_exit(0);	//手动退出
	printf("left...\n");
}

int main(void)
{
	pthread_t id1,id2;
	int i,ret1,ret2;
	ret1=pthread_create(&id1,NULL,(void*)thread1,NULL);	//创建两个子线程，并开启对应的函数
	ret2=pthread_create(&id2,NULL,(void*)thread2,NULL);
	if(ret1!=0)	//若子线程1创建失败
	{
		printf("Create pthread1 error!\n");
		exit(1);
	}
	if(ret2!=0)	//若子线程1创建失败
	{
		printf("Create pthread2 error!\n");
		exit(1);
	}
	for(i=0;i<3;i++)	printf("This is the main process.\n");
	pthread_join(id1,NULL);	//当子线程运行完毕，将子线程加入到主线程
	pthread_join(id2,NULL);
	return (0);
}
