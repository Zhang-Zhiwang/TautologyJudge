#ifndef _LogicalJudgeBitree_H_
#define _LogicalJudgeBitree_H_


typedef struct BTNode{
	char data;//存放字符数组转化得来的字符 
	char value;//存放变元的值以及运算符 
	BTNode *LChild, *RChild;
}BTNode,*BiTree;



typedef struct TrueValueTable{
	int num;//序号 
	char  binary[20];//二进制字符串 
	char value;//真值 
}TrueValueTable,*Table; 



void InitBiTree(BiTree &T)//初始化二叉树并置空 
{
	T=NULL;
}

void DestroyBiTree(BiTree &T)//销毁二叉树 
{
	if(T)
	{
		DestroyBiTree(T->LChild);
		DestroyBiTree(T->RChild);
		delete T;
	}
	T=NULL;
}

bool JudgeSymbol(char &c)//判断是否为操作符 
{
	switch(c)
	{
		case'~':
		case'&':
		case'|':
		case'(':
		case')':
		case'=':return true;
		default :return false;
	}
}

char Priority(char &t1,char &t2)//判断操作符的优先级，t1为栈中元素，t2为即将入栈元素 
{
	char f;
	switch(t2)
	{
		case '|':
			if(t1=='(') f='<';
			else f='>';
			break;
		case '&':
			if(t1=='('||t1=='|') f='<';
			else f='>';
			break;
		case '~':
			if(t1=='|'||t1=='&'||t1=='(') f= '<';
			else f='>';
			break;
		case '(':
			f='<';
			break;
		case ')':
			if(t1=='(') f='=';
			else f='>';
			break;
		case '=':
			f='>';
			break;
	}
	return f;
}

typedef BTNode* SElemType;
#include "SqStack.h"
bool CreateBiTree(BiTree &T,char ch[])
{
	SqStack OPND,OPTR;
	InitSqStack(OPND);
	InitSqStack(OPTR);
	BTNode *p,*q,*s;
	int i=0;
	char symbol;
	
	while(!SqStackEmpty(OPTR)||ch[i]!='=')
	{
		if(ch[i]>='a'&&ch[i]<='z'||ch[i]>='A'&&ch[i]<='Z')//将单个字符作为变元，生成节点并入栈 
		{
			p=new BTNode;
			p->data=ch[i];
			p->value='0';//变元初始真值赋0 
			p->LChild=p->RChild=NULL;
			Push(OPND,p);
			i++;
		}
		else if(JudgeSymbol(ch[i]))//操作符入栈或弹出 
		{
			if(SqStackEmpty(OPTR))//操作符栈空则当前操作符进操作符栈 
			{
				q=new BTNode;
				q->data=q->value=ch[i];
				q->LChild=q->RChild=NULL;
				Push(OPTR,q);
				i++;
			}
			else if(GetTop(OPTR,p))
			{
				symbol=p->data;
				switch(Priority(symbol,ch[i]))
				{
					case '<'://栈中操作符优先级小于当前操作符，当前操作符入栈 
						q=new BTNode;
						q->data=q->value=ch[i];
						q->LChild=q->RChild=NULL;
						Push(OPTR,q);
						i++;
						break;
					case '='://去除括号 
						Pop(OPTR,s);
						delete s;
						i++;
						break;
					case '>'://栈中操作符优先级高，弹出操作符与对应变元生成字数入变元栈 
						Pop(OPTR,s);
						if(symbol=='~')//单目操作符，生成右子树，便于中序遍历 
						{
							if(SqStackEmpty(OPND)) return false;
							Pop(OPND,p);
							s->RChild=p;
							s->LChild=NULL;
						}
						else
						{
							if(SqStackEmpty(OPND)) return false;
							Pop(OPND,p);
							if(SqStackEmpty(OPND)) return false;
							Pop(OPND,q);
							s->LChild=q;
							s->RChild=p;
						}
						Push(OPND,s);
				}
			}
		}
	}
	if(Pop(OPND,T)) return true;
	else return false;
}

char Operate(char a,char symbol,char b)
{
	switch(symbol)
	{
		case '&':
			if(a=='1'&&b=='1') return '1';
			else return '0';
			break;
		case '|':
			if(a=='1'||b=='1') return '1';
			else return '0';
			break;
		case '~':
			if(b=='0') return '1';
			else return '0';
			break;
	}
}


char Calculate(BiTree &T)
{
	if(T)
	{
		if(!T->LChild&&!T->RChild) return T->value;
		else return Operate(Calculate(T->LChild),T->data,Calculate(T->RChild));
	}
}

void PreorderTraverse(BiTree &T,void (*visit)(BiTree &T))
{
	if(T)
	{
		visit(T);
		PreorderTraverse(T->LChild,visit);
		PreorderTraverse(T->RChild,visit);
	}
}

void MidorderTraverse(BiTree &T,void (*visit)(BiTree &T))
{
	if(T)
	{
		MidorderTraverse(T->LChild,visit);
		visit(T);
		MidorderTraverse(T->RChild,visit);
	}
}

void LastorderTraverse(BiTree &T,void(*visit)(BiTree &T))
{
	if(T)
	{
		LastorderTraverse(T->LChild,visit);
		LastorderTraverse(T->RChild,visit);
		visit(T);
	}
}

void Expression(BiTree &T,void (*visit)(BiTree &T))//利用中序遍历对表达式加括号 
{
	char a,b;
	if(T)
	{
		if(T->data=='~')
		{
			visit(T);
			if(JudgeSymbol(T->RChild->data))
			{
				cout<<"(";
				Expression(T->RChild,visit);
				cout<<")";
			}
			else Expression(T->RChild,visit);
		}
		else
		{
			if(T->LChild&&T->LChild->LChild&&(Priority(a=T->LChild->data,b=T->data)=='<'))
			{
				cout<<"(";
				Expression(T->LChild,visit);
				cout<<")";
			}
			else Expression(T->LChild,visit);
			visit(T);
			if(T->RChild&&T->RChild->LChild&&(Priority(a=T->RChild->data,b=T->data)=='<'))
			{
				cout<<"(";
				Expression(T->RChild,visit);
				cout<<")";
			}
			else Expression(T->RChild,visit);
		}
	}
}

int CountVariate(char ch[],char *var)//计算表达式中不同的变元个数 
{
	int len=strlen(ch);
	int count=0,i=0,j;
	bool *Judge=new bool [len] {false};
	while(ch[i]!='=')
	{
		if( !JudgeSymbol( ch[i] ) && *(Judge+i) == false)//该字符是字母，并且在前面的字符中未出现过 
		{
			for(j=i;j<len;j++)
			{
				if(ch[j]==ch[i]) *(Judge+j)=true;
			}
			*(var+count)=ch[i];
			count++;
		}
		i++;
	}
	*(var+count)='\0';//为了strlen()函数能返回正确的值，需要加上\0； 
	return count;
}

void Change(BiTree &T,char letter,char value)//遍历二叉树对对应变元赋值 
{
	if(T)
	{
		if(T->data==letter) T->value=value;
		Change(T->LChild,letter,value);
		Change(T->RChild,letter,value);
	}
}

void CreateTable(Table &Ta,char var[],BiTree &T)
{
	int all=strlen(var);//根据字符串获得变元个数，并创建真值表 
	int space=(int)pow(2,all);
	Ta=new TrueValueTable [space];
	for(int i=0;i<space;i++)
	{
		Ta[i].num=i; //赋序号值 
		int temp=i,mod;//为字符数组赋值的辅助变量 
		for(int k=0;k<all;k++)
		{
			mod=temp%2;
			temp/=2;
			Ta[i].binary[all-k-1]=mod+'0';
		}//模2取余逆序赋给字符数组 
		Ta[i].binary[all]='\0';//在最后加上\0 
		for(int j=0;j<all;j++)
		{
			Change(T,var[j],Ta[i].binary[j]);
		}
		Ta[i].value=Calculate(T);
	}
}

void DisplayTable(Table &Ta,int all)
{
	int space=(int)pow(2,all);
	for(int i=0;i<space;i++){
		cout<<Ta[i].num<<"\t\t"<<Ta[i].binary<<"\t\t"<<Ta[i].value<<endl;
	}
}

void JudgeTable(Table &Ta,int all)
{
	int space =(int)pow(2,all);
	int count=0;
	for(int i=0;i<space;i++)
	{
		if(Ta[i].value=='0') count++;
	}
	if(count==0) 
		cout<<"TrueForever"<<endl;
	else if(count==space)
		cout<<"FalseForever"<<endl;
		else
		cout<<"Satisfiablity"<<endl;
}
#endif
