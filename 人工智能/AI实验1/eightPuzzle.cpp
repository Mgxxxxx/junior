#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <memory.h>

using namespace std;

struct Status
{
	int status[3][3];
	Status *parent;
	Status *next;
};

class EightPuzzle
{
public:
	Status root;
	int goal[9] = {1, 2, 3, 8, 0, 4, 7, 6, 5};
	int pre[9];
	int nextNum = 0;
	int step = 0;
	Status next[4];
	//初始化根节点
	EightPuzzle(int *init)
	{
		memcpy(root.status, init, sizeof(int) * 9);
		memcpy(pre, init, sizeof(int) * 9);
		root.parent = NULL;
		root.next = NULL;
	}
	//打印路径
	void PrintPath(Status *t)
	{
		if (t)
		{
			PrintPath(t->parent);
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
					cout << t->status[i][j];
				cout << endl;
			}
			cout << endl;
		}
	}
	//释放closeTable
	void FreeCloseTable(Status *t)
	{
		Status *p;
		while (t)
		{
			p = t;
			t = t->next;
			free(p);
		}
	}
	//是否与上一步状态一样，是返回0
	int EqualPre(Status *t)
	{
		return memcmp(t->status, pre, sizeof(int) * 9);
	}
	//判断是否是目标状态，若是返回0
	int IsGoal(Status *t)
	{
		return memcmp(t->status, goal, sizeof(int) * 9);
	}
	//下一步
	void Next(Status *t)
	{
		nextNum = 0;
		int posi, posj;
		Status s;
		for (int i = 0; i < 9; i++)
		{
			posi = i / 3;
			posj = i - i / 3 * 3;
			if (t->status[posi][posj] == 0)
				break;
		}
		//up
		if (posi > 0)
		{
			s = *t;
			s.status[posi][posj] = s.status[posi - 1][posj];
			s.status[posi - 1][posj] = 0;
			if (EqualPre(&s) != 0)
			{
				next[nextNum] = s;
				next[nextNum].parent = t;
				nextNum++;
			}
		}
		//down
		if (posi < 2)
		{
			s = *t;
			s.status[posi][posj] = s.status[posi + 1][posj];
			s.status[posi + 1][posj] = 0;
			if (EqualPre(&s) != 0)
			{
				next[nextNum] = s;
				next[nextNum].parent = t;
				nextNum++;
			}
		}
		//left
		if (posj > 0)
		{
			s = *t;
			s.status[posi][posj] = s.status[posi][posj - 1];
			s.status[posi][posj - 1] = 0;
			if (EqualPre(&s) != 0)
			{
				next[nextNum] = s;
				next[nextNum].parent = t;
				nextNum++;
			}
		}
		//right
		if (posj < 2)
		{
			s = *t;
			s.status[posi][posj] = s.status[posi][posj + 1];
			s.status[posi][posj + 1] = 0;
			if (EqualPre(&s) != 0)
			{
				next[nextNum] = s;
				next[nextNum].parent = t;
				nextNum++;
			}
		}
	}
	//广度优先
	void BFS()
	{
		step = 0;
		queue<Status> openTable;
		Status *closeTable = new Status;
		Status *cur = closeTable;
		Status *t;
		openTable.push(root);
		while (!openTable.empty())
		{
			memcpy(pre, cur->status, sizeof(int) * 9);
			t = new Status;
			*t = openTable.front();
			openTable.pop();
			step++;
			cur->next = t;
			cur = cur->next;
			if (IsGoal(cur) == 0)
			{
				PrintPath(cur);
				FreeCloseTable(closeTable);
				cout << "step:" << step << endl;
			}
			Next(cur);
			for (int i = 0; i < nextNum; i++)
			{
				openTable.push(next[i]);
			}
		}
		cout << "BFS failed.\n";
		FreeCloseTable(closeTable);
	}
};

int main()
{
	// int init[9] = {2, 8, 3, 1, 0, 4, 7, 6, 5};
	// EightPuzzle ep(init);
	// ep.BFS();
}