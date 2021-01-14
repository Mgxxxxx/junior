#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
int appleNum=5;	//苹果的总个数
int count=0;	//当前盘子里苹果个数
sem_t sem1,sem2;	//sem1为盘子使用权，sem2为苹果的个数

void put()
{
	while(appleNum>0)	//有苹果时
	{
		sem_wait(&sem1);	//sem1减一，得到盘子使用权
		printf("put a apple on the plate.\n");
		sem_post(&sem2);	//sem2加一，苹果数量加一
		appleNum--;
		count++;
		printf("There are %d apples on the plate.\n",count);
		sem_post(&sem1);	//让出盘子使用权
	}
}

void get()
{
	while(1)	//一直观察是否有苹果
	{
			sem_wait(&sem2);	//先看有无苹果
			sem_wait(&sem1);	//sem1减一，得到盘子使用权
			printf("get a apple from the plate.\n");
			count--;
			printf("There are %d apples on the plate.\n",count);
			sem_post(&sem1);	//让出盘子使用权
	}
}

int main()
{
	pthread_t t1,t2;
	sem_init(&sem1,0,1);	//初始化信号量sem1为1
	sem_init(&sem2,0,0);	//初始化信号量sem2为0
	pthread_create(&t1,NULL,(void*)put,NULL);
	pthread_create(&t2,NULL,(void*)get,NULL);
	pthread_join(t2,NULL);
}
