#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main(int argc,char **argv){
         long int a=syscall(321);
	printf("System call sys_helloworld return %ld",a);
	return 0;
}

