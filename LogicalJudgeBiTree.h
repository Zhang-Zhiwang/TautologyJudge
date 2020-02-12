#ifndef _LogicalJudgeBitree_H_
#define _LogicalJudgeBitree_H_


typedef struct BTNode{
	char data;//����ַ�����ת���������ַ� 
	char value;//��ű�Ԫ��ֵ�Լ������ 
	BTNode *LChild, *RChild;
}BTNode,*BiTree;



typedef struct TrueValueTable{
	int num;//��� 
	char  binary[20];//�������ַ��� 
	char value;//��ֵ 
}TrueValueTable,*Table; 



void InitBiTree(BiTree &T)//��ʼ�����������ÿ� 
{
	T=NULL;
}

void DestroyBiTree(BiTree &T)//���ٶ����� 
{
	if(T)
	{
		DestroyBiTree(T->LChild);
		DestroyBiTree(T->RChild);
		delete T;
	}
	T=NULL;
}

bool JudgeSymbol(char &c)//�ж��Ƿ�Ϊ������ 
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

char Priority(char &t1,char &t2)//�жϲ����������ȼ���t1Ϊջ��Ԫ�أ�t2Ϊ������ջԪ�� 
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
		if(ch[i]>='a'&&ch[i]<='z'||ch[i]>='A'&&ch[i]<='Z')//�������ַ���Ϊ��Ԫ�����ɽڵ㲢��ջ 
		{
			p=new BTNode;
			p->data=ch[i];
			p->value='0';//��Ԫ��ʼ��ֵ��0 
			p->LChild=p->RChild=NULL;
			Push(OPND,p);
			i++;
		}
		else if(JudgeSymbol(ch[i]))//��������ջ�򵯳� 
		{
			if(SqStackEmpty(OPTR))//������ջ����ǰ��������������ջ 
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
					case '<'://ջ�в��������ȼ�С�ڵ�ǰ����������ǰ��������ջ 
						q=new BTNode;
						q->data=q->value=ch[i];
						q->LChild=q->RChild=NULL;
						Push(OPTR,q);
						i++;
						break;
					case '='://ȥ������ 
						Pop(OPTR,s);
						delete s;
						i++;
						break;
					case '>'://ջ�в��������ȼ��ߣ��������������Ӧ��Ԫ�����������Ԫջ 
						Pop(OPTR,s);
						if(symbol=='~')//��Ŀ������������������������������� 
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

void Expression(BiTree &T,void (*visit)(BiTree &T))//������������Ա��ʽ������ 
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

int CountVariate(char ch[],char *var)//������ʽ�в�ͬ�ı�Ԫ���� 
{
	int len=strlen(ch);
	int count=0,i=0,j;
	bool *Judge=new bool [len] {false};
	while(ch[i]!='=')
	{
		if( !JudgeSymbol( ch[i] ) && *(Judge+i) == false)//���ַ�����ĸ��������ǰ����ַ���δ���ֹ� 
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
	*(var+count)='\0';//Ϊ��strlen()�����ܷ�����ȷ��ֵ����Ҫ����\0�� 
	return count;
}

void Change(BiTree &T,char letter,char value)//�����������Զ�Ӧ��Ԫ��ֵ 
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
	int all=strlen(var);//�����ַ�����ñ�Ԫ��������������ֵ�� 
	int space=(int)pow(2,all);
	Ta=new TrueValueTable [space];
	for(int i=0;i<space;i++)
	{
		Ta[i].num=i; //�����ֵ 
		int temp=i,mod;//Ϊ�ַ����鸳ֵ�ĸ������� 
		for(int k=0;k<all;k++)
		{
			mod=temp%2;
			temp/=2;
			Ta[i].binary[all-k-1]=mod+'0';
		}//ģ2ȡ�����򸳸��ַ����� 
		Ta[i].binary[all]='\0';//��������\0 
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
