#include <iostream>
#include <cstdlib>
#include <string.h>
#include <math.h>
using namespace std;
#include "LogicalJudgeBiTree.h"

void Print(BiTree &T)
{
	cout<<T->data<<" ";
}

int main()
{
	BiTree BT;
	InitBiTree(BT);
	Table Ta; 
	char ch[256],var[20];//ch为输入字符数组，var为变元字符数组 
	cout<<"请正确输入逻辑表达式并以'='结束。"<<endl;
	cin>>ch;
	cout<<endl;
	if(CreateBiTree(BT,ch))
	{
		cout<<"表达式中变元个数为:"<<endl;
		cout<<CountVariate(ch,var)<<endl;
		cout<<"变元分别为："; 
		cout<<var<<endl;
		int all=strlen(var);//变元个数 
		cout<<"表达式前缀表示为："<<endl;
		PreorderTraverse(BT,Print);
		cout<<endl;
		cout<<"表达式中缀表示为："<<endl;
		MidorderTraverse(BT,Print);
		cout<<endl;
		cout<<"表达式后缀表示为："<<endl;
		LastorderTraverse(BT,Print);
		cout<<endl;
		cout<<"表达式为："<<endl;
		Expression(BT,Print);
		cout<<endl;
		cout<<"序号\t\t"<<var<<"\t\t"<<"表达式值"<<endl;
		CreateTable(Ta,var,BT);
		DisplayTable(Ta,all);
		JudgeTable(Ta,all);
	}
	else cout<<"输入结果有误"<<endl;
	system("pause");
}
