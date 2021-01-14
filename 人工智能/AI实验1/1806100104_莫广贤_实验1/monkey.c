#include <stdlib.h>
#include <stdio.h>
#include <time.h>
struct State
{
	int monkey; /*-1:Monkey at A;0: Monkey at B;1:Monkey at C;*/
	int box;	/*-1:box at A;0:box at B;1:box at C;*/
	int monbox; /*-1: monkey on the box;1: monkey  the box;*/
};
struct State States[150];
char *routesave[150];

int banana; //香蕉的位置

/*function monkeygoto,it makes the monkey goto the other place*/
void monkeygoto(int b, int i)
{
	int a;
	a = b;
	if (a == -1)
	{
		routesave[i] = "Monkey go to A";
		States[i + 1] = States[i];
		States[i + 1].monkey = -1;
	}
	else if (a == 0)
	{
		routesave[i] = "Monkey go to B";
		States[i + 1] = States[i];
		States[i + 1].monkey = 0;
	}

	else if (a == 1)
	{
		routesave[i] = "Monkey go to C";
		States[i + 1] = States[i];
		States[i + 1].monkey = 1;
	}
	else
	{
		printf("parameter is wrong");
	}
}
/*end function monkeyygoto*/
/*function movebox,the monkey move the box to the other place*/
void movebox(int a, int i)
{
	int B;
	B = a;
	if (B == -1)
	{
		routesave[i] = "monkey move box to A";
		States[i + 1] = States[i];
		States[i + 1].monkey = -1;
		States[i + 1].box = -1;
	}
	else if (B == 0)
	{
		routesave[i] = "monkey move box to B";
		States[i + 1] = States[i];
		States[i + 1].monkey = 0;
		States[i + 1].box = 0;
	}
	else if (B == 1)
	{
		routesave[i] = "monkey move box to C";
		States[i + 1] = States[i];
		States[i + 1].monkey = 1;
		States[i + 1].box = 1;
	}
	else
	{
		printf("parameter is wrong");
	}
}
/*end function movebox*/
/*function climbonto,the monkey climb onto the box*/
void climbonto(int i)
{
	routesave[i] = "Monkey climb onto the box";
	States[i + 1] = States[i];
	States[i + 1].monbox = 1;
}
/*function climbdown,monkey climb down from the box*/
void climbdown(int i)
{
	routesave[i] = "Monkey climb down from the box";
	States[i + 1] = States[i];
	States[i + 1].monbox = -1;
}
/*function reach,if the monkey,box,and banana are at the same place,the monkey reach banana*/
void reach(int i)
{
	routesave[i] = "Monkey reach the banana";
}
/*output the solution to the problem*/
void showSolution(int i)
{
	int c;
	printf("%s \n", "Result to problem:");
	for (c = 0; c < i + 1; c++)
	{
		printf("Step %d : %s \n", c + 1, routesave[c]);
	}
	printf("\n");
}
/*perform next step*/
void nextStep(int i)
{
	int j;
	if (i >= 150)
	{
		printf("%s  \n", "steplength reached 150,have problem ");
		return;
	}
	if (States[i].monbox == 1 && States[i].monkey == banana && States[i].box == banana) //猴子、箱子、香蕉都在一个点且猴子在箱子上
	{
		showSolution(i);
		return;
	}
	j = i + 1;
	if (States[i].monkey == banana) //猴子在香蕉水平位置上
	{
		if (States[i].box == banana) //箱子在香蕉水平位置上
		{
			if (States[i].monbox == -1) //猴子没上箱子
			{
				climbonto(i);
				reach(i + 1);
				nextStep(j);
			}
			else //猴子在箱子上
			{
				reach(i);
				nextStep(j);
			}
		}
		else //箱子不在香蕉水平位置上
		{
			monkeygoto(States[i].box, i);
			nextStep(j);
		}
	}
	/*end if*/
	else if (States[i].monkey != banana) //猴子不在香蕉水平位置上
	{
		if (States[i].box == States[i].monkey) //箱子和猴子在同一水平位置上
		{
			if (States[i].monbox == -1) //猴子不在箱子上
			{
				movebox(banana, i);
				nextStep(j);
			}
			else //猴子在箱子上
			{
				climbdown(i);
				nextStep(j);
			}
		}
		else if (States[i].box != States[i].monkey) //箱子和猴子不在一个水平位置上
		{
			monkeygoto(States[i].box, i);
			nextStep(j);
		}
	}
} /*end nextStep*/
int main()
{
	srand((unsigned)time(NULL));
	int pos[3] = {-1, 0, 1};
	int onbox[2] = {-1, 1};
	banana = pos[rand() % 3];
	States[0].box = pos[rand() % 3];
	States[0].monbox = onbox[rand() % 2];
	if (States[0].monbox == 1) //若猴子在箱子上，则猴子的位置只能和箱子一样
		States[0].monkey = States[0].box;
	else
		States[0].monkey = pos[rand() % 3];
	printf("monkey:%d\tbox:%d\tmonbox:%d\tbanana:%d\n", States[0].monkey, States[0].box, States[0].monbox, banana);
	nextStep(0);
}
