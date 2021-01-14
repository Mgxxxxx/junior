#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <map>

using namespace std;

int main()
{
	string s[20];
	string st;
	int gSize = 0;
	int maxSize = 0;
	vector<char> vt, vn;
	vector<vector<string>> closure;
	vector<string> iclosure;
	//输入并拓广文法
	cout << "输入文法，###结束。（如：S->BB|AA, $代表S`）\n";
	int i = -1;
	do
	{
		cin >> s[++i];
		//终结符集合和非终结符集合
		for (char ch : s[i])
		{
			if (isupper(ch))
			{
				if (find(vn.begin(), vn.end(), ch) == vn.end())
				{
					vn.push_back(ch);
				}
			}
			else
			{
				if (find(vt.begin(), vt.end(), ch) == vt.end())
				{
					vt.push_back(ch);
				}
			}
		}
		//end
		if (i == 0)
		{
			s[1] = s[0];
			s[0].clear();
			s[0].append("$").append("->").append(1, s[1][0]);
			++i;
		}
		int k = 3, j;
		st.assign(s[i]);
		for (j = 0; j < st.size(); ++j)
		{
			if (st[j] == '|')
			{
				s[i].assign(st, 0, 3);
				s[i].append(st, k, j - k);
				k = j + 1;
				++i;
			}
		}
		s[i].assign(st, 0, 3);
		s[i].append(st, k, j - k);
	} while (strcmp(s[i].c_str(), "###") == 0);

	for (string st : s)
	{
		if (st.size() > maxSize)
		{
			maxSize = st.size();
		}
	}

	gSize = i;
	//end
	//项目集规范簇
	//I0
	st = s[0];
	st.insert(3, 1, '.');
	iclosure.push_back(st);
	int pos = 0;
	while (pos < iclosure.size())
	{
		if ((i = iclosure[pos].find('.')) != iclosure[pos].npos)
		{
			if ((i != (iclosure[pos].size() - 1)) && (isupper(iclosure[pos][i + 1])))
			{
				for (int j = 0; j < gSize; ++j)
				{
					if (iclosure[pos][i + 1] == s[j][0])
					{
						st = s[j];
						st.insert(3, 1, '.');
						iclosure.push_back(st);
					}
				}
			}
			++pos;
		}
	}
	closure.push_back(iclosure);
	iclosure.clear();
	//end

	for (vector<string> vs : closure)
	{
		int p = 0;
		for (string st : vs)
		{
			int i = st.find('.');
			if ((i + 1) < st.size())
			{
				char ch = st[i + 1];
				st.erase(i, 1);
				st.insert(i + 1, ".");
				iclosure.push_back(st);
				int pos = 0;
				while (pos < iclosure.size())
				{
					if ((i = iclosure[pos].find('.')) != iclosure[pos].npos)
					{
						if ((i != (iclosure[pos].size() - 1)) && (isupper(iclosure[pos][i + 1])))
						{
							for (int j = 0; j < gSize; ++j)
							{
								if (iclosure[pos][i + 1] == s[j][0])
								{
									st = s[j];
									st.insert(3, 1, '.');
									iclosure.push_back(st);
								}
							}
						}
						++pos;
					}
				}
				if ((p < vs.size() - 1) && (ch == vs[p + 1][(vs[p + 1].find('.') + 1)]))
				{
				}
				else
				{
					closure.push_back(iclosure);
					iclosure.clear();
				}
				++p;
			}
		}
	}
	// end
}
