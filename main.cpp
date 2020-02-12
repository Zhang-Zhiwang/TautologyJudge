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
	char ch[256],var[20];//chΪ�����ַ����飬varΪ��Ԫ�ַ����� 
	cout<<"����ȷ�����߼����ʽ����'='������"<<endl;
	cin>>ch;
	cout<<endl;
	if(CreateBiTree(BT,ch))
	{
		cout<<"���ʽ�б�Ԫ����Ϊ:"<<endl;
		cout<<CountVariate(ch,var)<<endl;
		cout<<"��Ԫ�ֱ�Ϊ��"; 
		cout<<var<<endl;
		int all=strlen(var);//��Ԫ���� 
		cout<<"���ʽǰ׺��ʾΪ��"<<endl;
		PreorderTraverse(BT,Print);
		cout<<endl;
		cout<<"���ʽ��׺��ʾΪ��"<<endl;
		MidorderTraverse(BT,Print);
		cout<<endl;
		cout<<"���ʽ��׺��ʾΪ��"<<endl;
		LastorderTraverse(BT,Print);
		cout<<endl;
		cout<<"���ʽΪ��"<<endl;
		Expression(BT,Print);
		cout<<endl;
		cout<<"���\t\t"<<var<<"\t\t"<<"���ʽֵ"<<endl;
		CreateTable(Ta,var,BT);
		DisplayTable(Ta,all);
		JudgeTable(Ta,all);
	}
	else cout<<"����������"<<endl;
	system("pause");
}
