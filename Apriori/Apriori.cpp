/* Cpp version Apriori
Oct. 14, 2021
Author: Dr. Fox */


#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
#include<map>

using namespace std;

// 使用Map 进行快速计数
typedef map<vector<int>,int> Counter;
typedef vector<int> list;

// 输入文件路径
string Datapath = "input.txt";

int minsup;
Counter C,L;

int idx=0; // 记录当前层数


// 集合运算

// 求并集 且保证从小到大
list U(list s1, list s2){
    list ans;
    int i=0,j=0;
    for(;(i<s1.size())&&(j<s2.size());)
    {
        if(s1[i]<s2[j])
        {
            ans.push_back(s1[i]);
            i++;
        }
        else if(s1[i]>s2[j])
        {
            ans.push_back(s2[j]);
            j++;
        }
        else{
            ans.push_back(s1[i]);
            i++;
            j++;
        }
    }

    for(;i != s1.size();){
        ans.push_back(s1[i]);
        i++;
    }
    for(;j != s2.size();){
        ans.push_back(s2[j]);
        j++;
    }

    return ans;
}


// 求交集的大小
int num_inter(list s1,list s2)
{
    int ans = 0;
    int i=0,j=0;
    for(;(i<s1.size())&&(j<s2.size());)
    {
        if(s1[i]<s2[j]){
            i++;
        }
        else if(s1[i]>s2[j]){
            j++;
        }
        else{
            ans ++;
            i++;
            j++;
        }
    }

    return ans;
}


// 数据量过大，不直接装载数据，使用流数据
/* void load() */

void Init_load_C()
{
    ifstream inflow(Datapath);

    if(!inflow){
        perror("Failed to Scan Data!\n");
        exit(0);
    }
    else{
        int tpe;
        vector<int> tpl;

        while(! inflow.eof())
        {
            inflow >> tpe;
            if(tpe == -1)
                continue;

            tpl.push_back(tpe);

            if (C.count(tpl) == 0) // 之前并未出现
                C[tpl] = 1;
            else                    //之前出现过就次数加1
                C[tpl] ++;

            tpl.clear();
        }
    }

    inflow.close();
}

// 根据上一个C生成下一个L
void toNextL()
{
    L.clear();
    for(Counter::iterator i=C.begin() ; i!=C.end() ; i++)
        if(i->second >= minsup){
            L[i->first] = i->second;
        }
    return ;
}

// 根据上层L上成可能的候选集
void generate_CandidateSet()
{
    C.clear();
    list tp;
    for(Counter::iterator i=L.begin() ; i!=L.end() ; i++)
        for(Counter::iterator j=i; j!=L.end() ; j++)
        {
            if(j == i)
                continue;

            // 自己体会（懒得打字
            if(num_inter(i->first,j->first) == idx -1)
            {
                tp = U(i->first,j->first);
                if (C.count(tp) == 0)
                    C[tp] = 1;
                else
                    C[tp] ++;
            }
        }

    vector<list> tpfirst;
    for(Counter::iterator i=C.begin() ; i!=C.end() ; i++)
        if (i->second == (idx + 1)*idx/2)
        {
            tpfirst.push_back(i->first);
        }

    C.clear();
    for(int i=0;i<tpfirst.size();i++)
        C[tpfirst[i]] = 0;
}


void recount()
{
    ifstream inflow(Datapath);

    if(!inflow){
        perror("Failed to Scan Data!\n");
        exit(0);
    }

    int tpe;
    list tpl;

    while(! inflow.eof())
    {
        inflow >> tpe;
        if(tpe != -1)
            tpl.push_back(tpe);
        else{
            sort(tpl.begin(),tpl.end());
            for(Counter::iterator i=C.begin() ; i!=C.end() ; i++)
                if(num_inter(tpl,i->first) == idx+1)
                    C[i->first] ++;

            tpl.clear();
        }
    }

}

void printlist(list s)
{
    for(int i=0;i<s.size();i++)
        cout << s[i] << ' ';
    return ;
}

void Print(Counter counter)
{
    cout <<"No."<< idx << ": \n";
    for(Counter::iterator i=counter.begin() ; i!=counter.end() ; i++)
    {
        printlist(i->first);
        cout << ": " << i->second << endl;
    }
    cout << endl;
}

int main(){

    cout << "Please enter minsup: ";
    cin >> minsup;
    C.clear();
    L.clear();

    Init_load_C();
    toNextL();
    idx = 1;
    Print(L);

    while(true){
        generate_CandidateSet();
        recount();
        toNextL();
        if(L.size() == 0) // 结束
            return 0;
        idx ++;
        Print(L);
    }

    return 0;
}
