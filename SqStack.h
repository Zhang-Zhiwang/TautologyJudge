#ifndef _SqStack_H_
#define _SqStack_H_
#define  StackSize 50
typedef struct{
	SElemType *base;
	int size;
	int top;
}SqStack;

void InitSqStack(SqStack &S)
{
	S.base=new SElemType[StackSize];
	S.top=0;
	S.size=StackSize;
}

void DestroySqStack(SqStack &S)
{
	delete[] S.base;
	S.top=0;
	S.size=0;
}

void ClearSqStack(SqStack &S)
{
	S.top=0;
}

bool SqStackEmpty(SqStack &S)
{
	return S.top==0;
}

int SqStackLength(SqStack &S)
{
	return S.top;
}

bool GetTop(SqStack &S,SElemType &e)
{
	if(S.top!=0)
	{
		e=S.base[S.top-1];
		return true;
	}
	else return false;
}

void Push(SqStack &S,SElemType &e)
{
	if(S.top>=S.size)
	{
		SElemType *newbase;
		newbase = new SElemType [S.size+10];
		for(int i=0;i<S.top;i++)
			newbase[i]=S.base[i];
		delete[] S.base;
		S.base=newbase;
		S.size+=10;
	}
	S.base[S.top++]=e;
}

bool Pop(SqStack &S,SElemType &e)
{
	if(S.top!=0)
	{
		e=S.base[--S.top];
		return true;
	}
	else return false;
}
#endif
