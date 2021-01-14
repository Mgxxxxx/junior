#include <iostream>
#include <vector>

using namespace std;

struct Rule
{
    int missionary;
    int savage;
};
struct Node
{
    int missionary;
    int savage;
    int boat;
    int direct;
};

int num = 0;
vector<Node> v;

bool reach(Node &node)
{
    if (node.missionary == 0 && node.savage == 0 && node.boat == 0)
        return true;
    return false;
}
bool equal(Node &n1, Node &n2)
{
    if (n1.missionary == n2.missionary &&
        n1.savage == n2.savage &&
        n1.boat == n2.boat)
        return true;
    return false;
}
bool visited(vector<Node> &v, Node &n)
{
    for (int i = 0; i < v.size(); ++i)
        if (equal(v[i], n))
            return true;
    return false;
}
void print_rule(int n, Rule *r)
{
    for (int i = 0; i < n; ++i)
        cout << "(" << r[i].missionary << "," << r[i].savage << ")\n";
    cout << endl;
}
void print_v(vector<Node> &v)
{
    cout << "the " << ++num << " st method is:\n";
    for (int i = 0; i < v.size(); ++i)
        cout << "(" << v[i].missionary << "," << v[i].savage << "," << v[i].boat << endl;
    cout << endl;
}
void ruleSet(int &sample, int &capacity, Rule *r)
{
    int i, j, k = 0;
    for (i = 0; i < sample; ++i)
        for (j = 0; j < sample; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            if (i + j <= capacity)
            {
                r[k].missionary = i;
                r[k++].savage = j;
            }
        }
}
int getNum_ruleSet(int &sample, int &capacity)
{
    int i, j, k = 0;
    for (i = 0; i < sample; ++i)
        for (j = 0; j < sample; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            if (i + j <= capacity)
                ++k;
        }
    return k;
}
void init(vector<Node> &v, int &ruleset_num, int &sample, int &capacity)
{
    cout << "input the initial number of missionaries:\n";
    cin >> sample;
    cout << "input the capacity of boat\n";
    cin >> capacity;
    ruleset_num = getNum_ruleSet(sample, capacity);
    cout << "\nthere is a total of " << ruleset_num << " sets of rules:\n";
    Node n;
    n.missionary = n.savage = sample;
    n.boat = 1;
    n.direct = 0;
    v.push_back(n);
}
void processing(vector<Node> &v, Rule *rule, int &ruleset_num, int &sample)
{
    Node node;
    if (reach(v.back()))
        print_v(v);
    else
    {
        while (v.back().direct < ruleset_num)
        {
            if (v.back().boat == 1)
            {
                node.missionary = v.back().missionary - rule[v.back().direct].missionary;
                node.savage = v.back().savage - rule[v.back().direct].savage;
                node.boat = 0;
                node.direct = 0;
                if (visited(v, node) == false &&
                    (node.missionary == node.savage ||
                     node.missionary == node.missionary == sample) &&
                    (node.missionary >= 0) && (node.missionary <= sample) &&
                    (node.savage >= 0) && (node.savage <= sample))
                {
                    v.push_back(node);
                    processing(v, rule, ruleset_num, sample);
                    v.pop_back();
                }
            }
            else
            {
                node.missionary = v.back().missionary + rule[v.back().direct].missionary;
                node.savage = v.back().savage + rule[v.back().direct].savage;
                node.boat = 1;
                node.direct = 0;
                if (visited(v, node) == false &&
                    (node.missionary == node.savage || node.missionary == node.missionary == sample) &&
                    (node.missionary >= 0) && (node.missionary <= sample) &&
                    (node.savage >= 0) && (node.savage <= sample))
                {
                    v.push_back(node);
                    processing(v, rule, ruleset_num, sample);
                    v.pop_back();
                }
            }
            v.back().direct++;
        }
    }
}
int main()
{
    vector<Node> v;
    Rule *rule;
    int ruleset_num, sample, capacity;
    init(v, ruleset_num, sample, capacity);
    rule = new Rule[ruleset_num];
    ruleSet(sample, capacity, rule);
    print_rule(ruleset_num, rule);
    processing(v, rule, ruleset_num, sample);
}