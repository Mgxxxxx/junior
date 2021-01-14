#include <iostream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<stack>
#include<sstream>

using namespace std;

vector<string> grammar;
set<char> vt, vn;
vector<vector<string>> closure;
map<char, set<char>> first;
map<char, set<char>> follow;
map<int, map<char, int>> nextSG;
map<int, map<char, int>> nextR;
vector<int> statusStack;
vector<char> symbolStack;

int main()
{
	string st;
	vector<string> vs;
	//begin 拓广文法，获取终结符集和非终结符集
	cout << " $代表S`, @代表空字.\n";
	cout << "输入文法，###结束。（如：S->BB|AA）\n";
	int i = 0;
	do {
		cin >> st;
		if (st._Equal("###")) {
			break;
		}
		grammar.push_back(st);
		//终结符集合和非终结符集合
		for (char ch : st) {
			if (isupper(ch)) {
				vn.insert(ch);
			}
			else {
				vt.insert(ch);
			}
		}
		//
		if (i == 0) {
			grammar.push_back(st);
			grammar[0].clear();
			grammar[0].append("$").append("->").append(1, grammar[1][0]);
			++i;
		}
		int k = 3, j;
		st.assign(grammar[i]);
		for (j = 0; j < st.size(); ++j) {
			if (st[j] == '|') {
				grammar[i].assign(st, 0, 3);
				grammar[i].append(st, k, j - k);
				k = j + 1;
				++i;
				grammar.push_back("");
			}
		}
		grammar[i].assign(st, 0, 3);
		grammar[i].append(st, k, j - k);
		++i;
	} while (1);
	vn.insert('$');
	vt.erase('|');
	vt.erase('-');
	vt.erase('>');
	//end
	cout << "\n------拓广后的文法------\n";
	for (int i = 0; i < grammar.size(); i++) {
		cout << "(" << i << ") " << grammar[i] << endl;
	}
	//begin first集
	set<char> sc;
	bool flag = 0;
	bool flag1 = 0;
	//first集和follow集的非终结符
	for (char ch : vn) {
		first[ch];
		follow[ch];
	}
	//first集的终结符
	for (char ch : vt) {
		sc.insert(ch);
		first[ch] = sc;
		sc.clear();
	}
	//保存所有可推出空字的非终结符，用于行200
	for (int i = 0; i < grammar.size(); ++i) {
		if (grammar[i][3] == '@') {
			sc.insert(grammar[i][0]);
		}
	}
	//
	do {
		flag = 0;
		for (int i = 0; i < grammar.size(); ++i) {
			if (!isupper(grammar[i][3])) {
				if (first[grammar[i][0]].insert(grammar[i][3]).second) {
					flag = 1;
				}
			}
			else {
				int j = 3;
				do {
					flag1 = 0;
					for (char ch : first[grammar[i][j]]) {
						if (ch == '@') {
							//if (j < (grammar[i].size() - 1))
								continue;
						}
						if (first[grammar[i][0]].insert(ch).second) {
							flag = 1;
						}
					}
					if (sc.count(grammar[i][j])) {
						++j;
						if (j == grammar[i].size()) {
							break;
						}
						flag1 = 1;
					}
				} while (flag1);
			}
		}
	} while (flag);
	//end
	cout << "\n------FIRST集------\n";
	for (char ch : vn) {
		cout << "FIRST(" << ch << ")={";
		for (char ch2 : first[ch]) {
			cout << ch2 << ", ";
		}
		cout << "\b\b}\n";
	}
	//begin follow集
	follow['$'].insert('#');
	for (int i = 0; i < grammar.size(); i++) {
		for (int j = 3; j < grammar[i].size(); j++) {
			if (isupper(grammar[i][j]) && j + 1 < grammar[i].size()) {
				for (char ch : first[grammar[i][j + 1]]) {
					if (ch != '@')	follow[grammar[i][j]].insert(ch);
				}
			}
		}
	}
	do {
		flag = 0;
		for (int i = 0; i < grammar.size(); i++) {
			for (int j = 3; j < grammar[i].size(); j++) {
				if (isupper(grammar[i][j]) && j + 1 == grammar[i].size()) {
					for (char ch : follow[grammar[i][0]]) {
						follow[grammar[i][j]].insert(ch);
					}
				}
				else if (isupper(grammar[i][j]) && j + 1 != grammar[i].size()) {
					for (int k = j + 1; k < grammar.size(); k++) {
						if (first[grammar[i][k]].count('@')) {
							for (char ch : follow[grammar[i][0]]) {
								follow[grammar[i][j]].insert(ch);
							}
							flag = 1;
							break;
						}
					}
				}
			}
		}
	} while (flag);
	//end
	cout << "\n------FOLLOW------\n";
	for (char ch : vn) {
		cout << "FOLLOW(" << ch << ")={";
		for (char ch2 : follow[ch]) {
			cout << ch2 << ", ";
		}
		cout << "\b\b}\n";

	}
	//begin 项目集规范簇&下一步状态:nextR为归约，nextSG为移进
	//I0
	vector<string> iclosure;
	queue<char> qc;
	st = grammar[0];
	qc.push(st[3]);
	st.insert(3, 1, '%');
	iclosure.push_back(st);
	while (!qc.empty()) {
		for (string st : grammar) {
			if (st[0] == qc.front()) {
				if (isupper(st[3]) && st[0] != st[3]) {
					qc.push(st[3]);
				}
				st.insert(3, 1, '%');
				iclosure.push_back(st);
			}
		}
		qc.pop();
	}
	closure.push_back(iclosure);
	iclosure.clear();
	//Ix
	map<int, bool> cloF;	//一个簇的表达式的标记，1表示已访问
	map<int, bool> icloF;//'%'之后的非终结符的扩展标记，1表示已访问
	for (int i = 0; i < grammar.size(); i++) {
		icloF[i] = 0;
	}
	map<char, int> nsg;
	map<char, int> nr;
	for (int i = 0; i < closure.size(); i++) {
		for (int j = 0; j < closure[i].size(); j++) {
			int pos = closure[i][j].find('%');
			if (pos + 1 == closure[i][j].size()) {
				cloF[j] = 1;
				//保存归约
				if (pos + 1 == closure[i][j].size()) {
					st.assign(closure[i][j], 0, pos);
					int k;
					for (k = 0; k < grammar.size(); k++) {
						if (st._Equal(grammar[k])) {
							break;
						}
					}
					for (char ch : follow[st[0]]) {
						nr[ch] = k;
					}
				}
				//
			}
			else cloF[j] = 0;
		}
		for (int j = 0; j < closure[i].size(); j++) {
			if (cloF[j])
				continue;
			vs.clear();
			vs.push_back(closure[i][j]);
			cloF[j] = 1;
			int pos = closure[i][j].find('%');
			for (int k = j + 1; k < closure[i].size(); k++) {
				if (cloF[k])
					continue;
				if (closure[i][j][pos + 1] == closure[i][k][closure[i][k].find('%') + 1]) {
					vs.push_back(closure[i][k]);
					cloF[k] = 1;
				}
			}
			iclosure.clear();
			char ch;
			for (string st : vs) {
				int pos = st.find('%');
				ch = st[pos + 1];
				st[pos] = ch;
				st[pos + 1] = '%';
				iclosure.push_back(st);
			}
			bool flag = 1;
			for (int i = 0; i < grammar.size(); i++) {
				icloF[i] = 0;
			}
			int icloSize = iclosure.size();
			for (int i = 0; i < icloSize; i++) {
				st = iclosure[i];
				//queue<char> qc;
				int pos = st.find('%');
				if (pos + 1 != st.size()) {
					if (isupper(st[pos + 1])) {
						qc.push(st[pos + 1]);
					}
					while (!qc.empty()) {
						for (int i = 0; i < grammar.size(); i++) {
							st = grammar[i];
							if (st[0] == qc.front() && icloF[i] == 0) {
								icloF[i] = 1;
								if (st[0] != st[3])
									qc.push(st[3]);
								st.insert(3, 1, '%');
								iclosure.push_back(st);
							}
						}
						qc.pop();
					}
				}
			}
			for (int i = 0; i < closure.size(); i++) {
				if (closure[i] == iclosure) {
					flag = 0;
					nsg[ch] = i;
					break;
				}
			}
			if (flag) {
				nsg[ch] = closure.size();
				closure.push_back(iclosure);
			}
		}
		nextR[i] = nr;
		nr.clear();
		nextSG[i] = nsg;
		nsg.clear();
	}
	st = grammar[0];
	st.append("%");
	for (int i = 0; i < closure.size(); i++) {
		if (find(closure[i].begin(), closure[i].end(), st) != closure[i].end()) {
			nextR[i]['#'] = 666;
			break;
		}
	}
	//end
	cout << "\n------项目集规范簇------\n";
	for (int i = 0; i < closure.size(); i++) {
		cout << "I" << i << ": ";
		int k = 0;
		for (string st : closure[i]) {
			if (k++) {
				cout << "    ";
			}
			cout << st << endl;
		}
		cout << endl;
	}

	//begin 构造SLR(1)分析表
	cout << "\n------SLR(1)分析表------\n";
	int colNum = vt.size() + 1 + vn.size() - 1;
	int rowNum = closure.size();
	vector<char> vc;
	for (char ch : vt) {
		vc.push_back(ch);
	}
	vc.push_back('#');
	for (char ch : vn) {
		if (ch == '$')
			continue;
		vc.push_back(ch);
	}
	printf("%-7s", "状态");
	for (char ch : vc) {
		printf("%-4c", ch);
	}
	printf("\n");
	for (int i = 0; i < rowNum; i++) {
		printf("%-7d", i);
		for (char ch : vc) {
			if (nextSG[i].count(ch)) {
				if (isupper(ch))
					printf("%-4d", nextSG[i][ch]);
				else
					printf("%c%-3d", 's', nextSG[i][ch]);
			}
			else if (nextR[i].count(ch)) {
				if (nextR[i][ch] == 666)
					printf("%-4s", "acc");
				else
					printf("%c%-3d", 'r', nextR[i][ch]);
			}
			else {
				printf("%-4s", " ");
			}
		}
		printf("\n");
	}
	//end
	//begin 分析过程
	cout << "\n------输入分析------\n";
	cout << "请输入待分析的字符串：\n";
	string ist;
	cin >> ist;
	if (ist.back() != '#') {
		ist.append("#");
	}
	statusStack.push_back(0);
	symbolStack.push_back('#');
	cout << "\n------分析结果------\n";
	int pos = 0;
	printf("\n%-6s%-20s%-15s%15s  %s\n", "步骤", "状态栈", "符号栈", "输入串", "动作说明");
	stringstream ss;
	string pst;
	int step = 0;
	while (1) {
		if (nextSG[statusStack.back()].count(ist[pos])) {
			//输出
			int seq = nextSG[statusStack.back()][ist[pos]];
			printf("%-6d", ++step);
			for (int i : statusStack) {
				ss << i;
				ss >> st;
				ss.str("");
				ss.clear();
				pst.append(st).append(" ");
			}
			printf("%-20s", pst.c_str());
			st.clear();
			pst.clear();

			for (char ch : symbolStack) {
				st.push_back(ch);
			}
			pst.append(st);
			printf("%-15s", pst.c_str());
			st.clear();
			pst.clear();

			st.assign(ist, pos, ist.size() - pos);
			printf("%15s", st.c_str());
			st.clear();

			printf("  %s%d%s%c%s%d%s%d%s", "ACTION[", statusStack.back(), ",", ist[pos], "]=s", seq, "即状态", seq, "入栈");

			printf("\n");

			//
			statusStack.push_back(seq);
			symbolStack.push_back(ist[pos++]);
		}
		else if (nextR[statusStack.back()].count(ist[pos])) {
			int seq = nextR[statusStack.back()][ist[pos]];
			if (seq == 666) {
				//输出
				printf("%-6d", ++step);
				for (int i : statusStack) {
					ss << i;
					ss >> st;
					ss.str("");
					ss.clear();
					pst.append(st).append(" ");
				}
				printf("%-20s", pst.c_str());
				st.clear();
				pst.clear();

				for (char ch : symbolStack) {
					st.push_back(ch);
				}
				pst.append(st);
				printf("%-15s", pst.c_str());
				st.clear();
				pst.clear();

				st.assign(ist, pos, ist.size() - pos);
				printf("%15s", st.c_str());
				st.clear();

				printf("  %s\n", "acc：分析成功");

				break;
			}

			//输出
			printf("%-6d", ++step);
			for (int i : statusStack) {
				ss << i;
				ss >> st;
				ss.str("");
				ss.clear();
				pst.append(st).append(" ");
			}
			printf("%-20s", pst.c_str());
			st.clear();
			pst.clear();

			for (char ch : symbolStack) {
				st.push_back(ch);
			}
			pst.append(st);
			printf("%-15s", pst.c_str());
			st.clear();
			pst.clear();

			st.assign(ist, pos, ist.size() - pos);
			printf("%15s", st.c_str());
			st.clear();

			printf("  %s%d%s%s%s", "r", seq, ": 用", grammar[seq].c_str(), "归约且GOTO(");

			//
			for (int i = 3; i < grammar[seq].size(); i++) {
				symbolStack.pop_back();
				statusStack.pop_back();
			}

			printf("%d%s%c%s%d%s", statusStack.back(), ",", grammar[seq][0], ")=", nextSG[statusStack.back()][grammar[seq][0]], "入栈");
			printf("\n");

			symbolStack.push_back(grammar[seq][0]);
			statusStack.push_back(nextSG[statusStack.back()][symbolStack.back()]);
		}
		else {
			cout << "\n输入的字符串无法归约,byebye!\n";
			break;
		}
	}
}