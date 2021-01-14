#include <stdio.h>
struct State
{
    int monkey; /*-1:Monkey at A;0: Monkey at B;1:Monkey at C;*/
    int box;    /*-1:box at A;0:box at B;1:box at C;*/
    int banana; /*Banana at B,Banana=0*/
    int monbox; /*-1: monkey on the box;1: monkey  the box;*/
};
struct State States[150];
char *routesave[150];

int x = 0;

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
    int c;
    int j;
    if (i >= 150)
    {
        printf("%s  \n", "steplength reached 150,have problem ");
        return;
    }
    for (c = 0; c < i; c++) /*if the current state is same to previous,retrospect*/
    {
        if (States[c].monkey == States[i].monkey && States[c].box == States[i].box && States[c].banana == States[i].banana && States[c].monbox == States[i].monbox)
        {
            return;
        }
    }
    if (States[i].monbox == 1 && States[i].monkey == x && States[i].banana == x && States[i].box == x)
    {
        showSolution(i);
        printf("Press any key to continue \n");
        getchar(); /*to save screen for user,press any key to continue*/
        return;
    }
    j = i + 1;
    if (States[i].monkey == 0)
    {
        if (States[i].box == 0)
        {
            if (States[i].monbox == -1)
            {
                climbonto(i);
                reach(i + 1);
                nextStep(j);
            }
            else
            {
                reach(i + 1);
                nextStep(j);
                /*climbdown(i);
				nextStep(j);*/
            }
        }
        else if (States[i].box == 1)
        {
            monkeygoto(1, i);
            nextStep(j);
        }
        else /*box==-1*/
        {
            monkeygoto(-1, i);
            nextStep(j);
        }
    }
    /*end if*/
    else if (States[i].monkey == -1)
    {
        if (States[i].box == -1)
        {
            if (States[i].monbox == -1)
            {
                movebox(0, i);
                nextStep(j);
            }
            else
            {
                climbdown(i);
                nextStep(j);
            }
        }
        else if (States[i].box == 0)
        {
            monkeygoto(0, i);
            nextStep(j);
        }
        else
        {
            monkeygoto(1, i);
            nextStep(j);
        }
    }
    /*end if*/
    else if (States[i].monkey == 1)
    {
        if (States[i].box == 1)
        {
            if (States[i].monbox == -1)
            {
                movebox(0, i);
                nextStep(j);
            }
            else
            {
                climbdown(i);
                nextStep(j);
            }
        }
        else if (States[i].box == -1)
        {
            monkeygoto(-1, i);
            nextStep(j);
        }
        else
        {
            monkeygoto(0, i);
            nextStep(j);
        }
    }
    /*end if*/
} /*end nextStep*/
int main()
{
    States[0].monkey = -1;
    States[0].box = 1;
    States[0].banana = 0;
    States[0].monbox = -1;
    nextStep(0);
}
