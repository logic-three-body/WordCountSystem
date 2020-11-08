
#include<iostream>
#include<fstream>
#include<cstring>
#include<cmath>
#include <windows.h>
#include <time.h>
#include <iomanip> //要用到格式控制符
#include<string>
#define MAXSIZE 5000 
#define m 5000                        			//哈希表的表长
#define NULLKEY " "                 			//单元为空的标记

using namespace std;

int totalWordNum;//单词总数 
int NonReaptedWords;//去重后单词数 
string WordTank[MAXSIZE];//存放单词 
int frequency[MAXSIZE];//存放词频
int flag_First_occur[MAXSIZE];//标记第一次出现的位置 
double rate[MAXSIZE];
int setrate_Million_Smaller = 100;//万分之100以下的单词过滤
int setrate2;

typedef struct {
	string key;
	int Time;
}Word;

typedef struct {
	Word *R;
	int length;
}SSTable;

typedef struct LNode
{
	Word data;
	struct LNode *next;
}LNode, *LinkList;

typedef struct BSTNode
{
	Word data;
	struct BSTNode *lchild, *rchild;
}BSTNode, *BSTree;

BSTree T1;//

struct HashTable {
	string  key;
	int  Time;
};

typedef struct WordNode//单词结点 
{
	Word data;
	struct WordNode *nextarc;
}WordNode;
typedef struct LetterNode//首字母表头结点 
{
	int letter_no;//ch
	WordNode *firstarc;
}LetterNode, AdjList[38];//?

typedef struct
{
	AdjList Letter;//Letter
}ALGraph;//邻接表 

int Hash_int(int a)
{
	int p;
	p = WordTank[a][0];
	p = p - 96;
	return p;
}

void InitTree(BSTree &T)
{
	T = new BSTNode;
	T = NULL;
	return;
}

void Insert(BSTree &T, Word e)
{
	if (!T)
	{
		//建立一个根节点指针S并开辟存储空间 
		BSTree S;
		S = new BSTNode;
		//为新结点S赋值 
		S->data = e;
		//初始化孩子左右结点为空 
		S->lchild = S->rchild = NULL;
		T = S; //S赋值给T 
	}
	else if (e.key < T->data.key)//如果待插入元素小于根节点，插入左边 
		Insert(T->lchild, e);
	else if (e.key > T->data.key)//大于，右边 
		Insert(T->rchild, e);
}

int Input3(BSTree &T, string key) {
	int n = 0;
	if (T == NULL)return -1;
	while (T != NULL && key != T->data.key)//非递归遍历 
	{
		n++;
		if (key < T->data.key)
			T = T->lchild;
		else
			T = T->rchild;
	}
	if (T == NULL)return -1;
	if (key == T->data.key)
	{
		T->data.Time++;
		return n;
	}
}

int Search5(BSTree &T, string key) {
	int n = 0;
	if (T == NULL)return -1;
	while (T != NULL && key != T->data.key)
	{
		n++;
		if (key < T->data.key)
			T = T->lchild;
		else
			T = T->rchild;
	}
	if (T == NULL)return -1;
	if (key == T->data.key)
	{
		return n;
	}
}

void InputTree(BSTree &T)
{
	int i = 1;
	//建立一颗空树 
	T = NULL;
	//建立一个word结点，先赋值，便于录入 
	Word e;
	e.key = WordTank[1];
	e.Time = 1;
	while (i < totalWordNum)//单词数组未输入完 
	{
		//建立一个临时的树，用来遍历查重，这个遍历将改变树的根结点，是毁灭性的 
		BSTree temp;
		InitTree(temp);
		temp = T;
		//判断是否重复 
		int res = Input3(temp, WordTank[i]); //在此统计词频，若重复频次就加1。

		if (res == -1)//如果不重复，就录入.
		{
			Insert(T, e);
		}
		//else T->data.Time++;//不行 

		//下一次的输入 
		i++;
		e.key = WordTank[i];
		e.Time = 1;//初次输入，词频都为1
	}
}

int TraverseOut(BSTree T)
{

	fstream out;
	out.open("OutFile3.txt", ios::app);
	if (T)
	{
		TraverseOut(T->lchild);
		//cout<<T->data.key<<" "<<T->data.Time<<endl;
		out << T->data.key << " " << T->data.Time << endl;
		TraverseOut(T->rchild);
	}
	return 0;
}

int TraverseOut2(BSTree T)
{
	if (T)
	{
		TraverseOut2(T->lchild);
		cout << T->data.key << " " << T->data.Time << endl;
		TraverseOut2(T->rchild);
	}
	return 0;
}

void Output3(BSTree T)
{
	int i;
	fstream out;
	out.open("OutFile3.txt", fstream::out | ios_base::trunc);
	out << "单词 词频" << endl;
	TraverseOut(T);//遍历树的结点，依次输出 
	return;
}

void Search3(BSTree T)
{
	Output3(T);
	//TraverseOut2(T); 
	cout << "基于二叉排序树，请输入你要查找的单词：" << endl;
	string ch;
	cin >> ch;

	BSTree temp;
	InitTree(temp);
	temp = T;

	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);
	int res = Search5(temp, ch);
	QueryPerformanceCounter(&ed);
	time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;

	if (res != -1)
	{
		cout << "此单词的词频为：" << temp->data.Time << endl;
		cout << "查找所花费的次数：" << res << endl;
		cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
		cout << "平均查找长度：" << ((NonReaptedWords + 1) / NonReaptedWords) * log2(NonReaptedWords + 1) - 1 << endl;
	}
	else cout << "查找失败" << endl;
}

void Search3_1(BSTree T, string ch)
{
	Output3(T);
	cout << "基于二叉排序树:" << endl;

	BSTree temp;
	InitTree(temp);
	temp = T;

	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);
	int res = Search5(temp, ch);
	QueryPerformanceCounter(&ed);
	time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;

	if (res != -1)
	{
		cout << "此单词的词频为：" << temp->data.Time << endl;
		cout << "查找所花费的次数：" << res << endl;
		cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
		cout << "平均查找长度：" << ((NonReaptedWords + 1) / NonReaptedWords) * log2(NonReaptedWords + 1) - 1 << endl;
	}
	else cout << "查找失败" << endl;
}

void InitList(SSTable &L)
{
	L.R = new Word[MAXSIZE];
	//if(!L.elem) return;
	L.length = 0;
	return;
}

int CheckSqList(SSTable L, string a)
{
	for (int i = 1; i <= L.length; i++)
		if (a == L.R[i].key) return i;//返回重复单词在数组中的位置 
	return 0;
}

void Input1(SSTable &L)
{
	//词频统计&存入内存 
	int i = 1;
	while (i <= totalWordNum)
	{
		if ((WordTank[i][0] < '0' && !CheckSqList(L, WordTank[i])) || (WordTank[i][0] > '9' && !CheckSqList(L, WordTank[i])))//不重复录入 
		{
			L.length++;
			L.R[L.length].key = WordTank[i];
			L.R[L.length].Time = 1;//不能++ 
			//标记位置
			flag_First_occur[i] = 1;
		}
		else if ((WordTank[i][0] < '0'&&CheckSqList(L, WordTank[i])) || (WordTank[i][0] > '9'&&CheckSqList(L, WordTank[i])))//重复就只统计 
		{
			L.R[CheckSqList(L, WordTank[i])].Time++;//词频统计  
		}
		i++;
	}
}

void Rate1(SSTable L)
{
	fstream out;
	out.open("OutFile1.txt", fstream::out | ios_base::trunc);
	int i = 0;
	cout << "单词" << " " << "频次" << " " << "概率(万分之一)" << endl;
	while (i <= NonReaptedWords)
	{
		if (flag_First_occur[i] == 1)//如果是第一个元素，就输出 
		{
			frequency[i] = L.R[i].Time;
			rate[i] = (double)frequency[i] / totalWordNum;
			rate[i] = rate[i] * 10000;
			if (rate[i] > setrate_Million_Smaller)//&&rate[i]<100000
			{
				out << L.R[i].key << " " << frequency[i] << endl;
				cout << "i:" << i << " " << L.R[i].key << " " << frequency[i] << "/" << totalWordNum << " " << rate[i] << endl;
			}
		}
		i++;
	}
}

int Locate(LinkList L, string a)//定位重复结点 
{

	LinkList p;
	p = L;  //p指向第一个结点
	int i = 0;
	while (p->next) {//遍历单链表
		i++;
		p = p->next;
		if (p->data.key == a)
		{
			p->data.Time++;//有重复，频率加1
			// cout<<"有重复，位置为"<<i<<endl; 
			return i;
		}

	}
	return 0;
}

void Input2(LinkList &L)
{
	LinkList r, p; int i = 0;
	L = new LNode;
	L->next = NULL;
	r = L;
	for (i = 1; i <= totalWordNum; i++)
	{
		if (Locate(L, WordTank[i]) == 0)
		{
			//未重复的部分，也就是第一次录入的部分 
			p = new LNode;
			p->data.Time = 1;	//结点初始化频次为1 
			p->data.key = WordTank[i];
			p->next = NULL;
			r->next = p;
			r = p;
		}

	}
	return;
}

void Rate2(LinkList L)
{
	fstream out;
	out.open("OutFile1.txt", fstream::out | ios_base::trunc);
	LinkList p = L->next;
	//利用顺序表计算单词概率 
	int i = 1;
	cout << "单词" << " " << "频次" << " " << "概率" << endl;
	while (i <= totalWordNum)
	{
		if (flag_First_occur[i] == 1)//如果是第一个元素，就输出 
		{
			frequency[i] = p->data.Time;
			rate[i] = (double)frequency[i] / totalWordNum;
			rate[i] = rate[i] * 10000;
			if (rate[i] > setrate_Million_Smaller)
			{
				out << p->data.key << " " << frequency[i] << endl;
				cout << "i:" << i << " " << p->data.key << " " << frequency[i] << "/" << totalWordNum << " " << rate[i] << endl;
			}
			if (p->next) p = p->next;
			else break;
		}
		i++;
	}
}

int InitList(LinkList &L)
{
	LinkList p = L;
	while (p->next) {
		p = p->next;
		p->data.Time = 1;
	}
	return 0;
}

int ListLength(LinkList L)
{
	LinkList p;
	p = L->next;  //p指向第一个结点
	int i = 0;
	while (p) {//遍历单链表,统计结点数
		i++;
		p = p->next;
	}
	return i;
}

void Sort2(LinkList &L)
{
	int n = ListLength(L);
	int i; //用i记录已经比好，置于末尾的元素 
	int j;//用j记录已比较的轮次 
	LinkList p;
	for (i = 0; i < n - 1; i++)//n个元素要比较n-1轮 
	{
		p = L->next;
		j = 0;//比较完一轮后，j归零 
		while (p&&j < n - 1 - i)//和2轮for循环的本质一样 
		{
			if (p->data.key > p->next->data.key)
			{
				Word t = p->data;
				p->data = p->next->data;
				p->next->data = t;
			}
			p = p->next;
			j++;
		}
	}
	return;
}

void Output2(LinkList &L)
{
	int i;
	fstream out;
	out.open("OutFile2.txt", fstream::out | ios_base::trunc);
	out << "单词 词频" << endl;
	Sort2(L);
	LinkList p = L->next;
	while (p)
	{
		out << p->data.key << " " << p->data.Time << endl;
		p = p->next;
	}
	return;
}

void Search2(LinkList L)
{
	Output2(L);
	cout << "基于链式存储结构，请输入你要查找的单词：" << endl;
	string ch;
	cin >> ch;

	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);

	LinkList p;
	p = L;
	int i = 0;
	while (p->next)
	{
		p = p->next;
		if (p->data.key == ch)
		{
			QueryPerformanceCounter(&ed);
			time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;
			cout << "此单词的词频为：" << p->data.Time << endl;
			cout << "查找所花费的次数：" << Locate(L, ch) << endl;
			cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
			cout << "平均查找长度：" << (NonReaptedWords + 1) / 2 << endl;
			return;
		}
	}
	cout << "查找失败！" << endl;
}

void Search2_1(LinkList L, string ch)
{
	Output2(L);
	cout << "基于链式存储结构：" << endl;
	LinkList p;
	p = L;
	int i = 0;

	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);
	while (p->next)
	{
		p = p->next;
		if (p->data.key == ch)
		{
			QueryPerformanceCounter(&ed);
			time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;
			cout << "此单词的词频为：" << p->data.Time << endl;
			cout << "查找所花费的次数：" << Locate(L, ch) << endl;
			cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
			cout << "平均查找长度：" << (NonReaptedWords + 1) / 2 << endl;
			return;
		}
	}
	cout << "查找失败！" << endl;
}

void PrintText()
{
	int i = 1;
	while (i <= totalWordNum)
	{
		cout << WordTank[i] << " ";
		i++;
	}
	cout << endl;
}

void PrintWord()
{
	int i = 1;
	while (i <= totalWordNum)
	{

		cout << "单词序号：" << i << "   /" << WordTank[i] << "/" << " ";
		cout << WordTank[i].length() << endl;
		i++;
	}
	if (i == totalWordNum)cout << "单词数组已全部打印" << endl;
	cout << "除去数字外，本文一共" << i - 1 << "个英文单词" << endl;
}

void Output1(SSTable &L)
{
	int i;
	fstream out;
	out.open("OutFile1.txt", fstream::out | ios_base::trunc);
	out << "单词 词频" << endl;
	//out<<"单词 词频 概率（万分之一）"<<endl; 
	for (i = 1; i <= L.length; i++)
	{
		//cout<<L.R[i].key<<" "<<L.R[i].Time<<"/"<<t<<" "<<setprecision(10)<<10000*L.R[i].Time/t<<endl;
		//out<<L.R[i].key<<" "<<L.R[i].Time<<"/"<<t<<" "<<setprecision(10)<<10000*L.R[i].Time/t<<endl;
		out << L.R[i].key << " " << L.R[i].Time << endl;
		//cout<<L.R[i].key<<" "<<L.R[i].Time<<endl;
	}
	return;
}

void Search1_Order(SSTable &L, string a)
{

	int flag = 0; int i;
	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);
	for (i = 1; i < i <= L.length; i++)//1000000
	{

		if (a == L.R[i].key)
		{
			flag = 1;
			break;
		}
	}
	QueryPerformanceCounter(&ed);
	time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;

	if (flag == 1)
	{
		cout << "基于顺序表的查找：" << endl;
		cout << "此单词的词频为：" << L.R[i].Time << endl;
		cout << "查找所花费的次数：" << i << endl;
		cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
		cout << "平均查找长度：" << (L.length + 1) / 2 << endl;
	}
	else cout << "查找失败" << endl;
	return;
}

void Search1_Half(SSTable &ST, string key)
{

	cout << "基于折半查找：" << endl;
	int low = 1;
	int high = ST.length;
	int k = 0;
	int flag = 0;
	int mid = 0;

	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);
	while (low <= high)
	{
		k++;
		mid = (low + high) / 2;
		if (key == ST.R[mid].key)
		{
			flag = 1;
			break;
		}
		else if (key < ST.R[mid].key) high = mid - 1;//前一子表查找
		else low = mid + 1;                       //后一子表查找
	}
	QueryPerformanceCounter(&ed);
	time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;
	if (flag == 1)
	{
		cout << "此单词的词频为：" << ST.R[mid].Time << endl;
		cout << "查找所花费的次数：" << k << endl;
		cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
		cout << "平均查找长度：" << log(ST.length) / log(2) << endl;
	}
	else
	{
		cout << "查找失败" << flag << endl;
	}
	return;
}

void Preprocessing()
{
	//读入 
	fstream in;
	in.open("InFile.txt", ios::in);
	if (!in)
	{
		cout << "Can not open the file!" << endl;
		return;
	}
	//存入内存 
	int i = 1;//从第一个单词开始，str[0]空置不用 
	while (!in.eof())
	{
		in >> WordTank[i];
		int j = WordTank[i].length();//单词字母数 
		bool jud_Comma = false;
		bool EndOfArticle = false;
		//首字母大写转为小写 
		for (int n = 0; n < j; n++)
		{
			if (WordTank[i][n] >= 'A'&&WordTank[i][n] <= 'Z')
				WordTank[i][n] = WordTank[i][n] + 32;
		}

		//非字母字符标记统计
		//TODO soultion 1 : 
		/*
		当找到字符的不是字母，有两种情况：
		1.到达文章末尾  -> stranger!
		2.衔接多个单词  -> tigers,two / Hello!Thanks,boy! 
		
		第一种情况  s t r ... e r ! '\0'  这时候只需去除标点即可
		第二种情况 将单词切片 substr() -> tigers two :
		 LetterTank[i+1]=LetterTank[i+1].substr(0,i-1)
		 ++i
		*/
		int num = 0;
		for (int n = 0; n < j; n++)
		{
			if (WordTank[i][n]<'a' || WordTank[i][n]>'z')
			{
				//cout<<"非字母："<<WordTank[i][n]<<endl;
				WordTank[i][n] = 35;
				//cout<<"转化为："<< WordTank[i][n]<<endl;

				string tmp = WordTank[i].substr(0, i - 1);



				WordTank[i] = tmp;
				cout << "转化为：" << WordTank[i+1][n] << endl;

				num++;
				jud_Comma = true;//有标点
			}
		}
		//TODO  可优化 : 设置标志如果为 非单词则进入 

		//非字母字符移动至字符串最后 
		if (jud_Comma)
		{
			for (int k = 0; k < j; k++)
				if (WordTank[i][k] == 35)//交换，利用Letter[0][0]当temp变量
				{
					WordTank[0][0] = WordTank[i][k];
					WordTank[i][k] = WordTank[i][k + 1];
					WordTank[i][k + 1] = WordTank[0][0];
				}
		}


		WordTank[i] = WordTank[i].substr(0, j - num);//去除标点符号 substr(0, j - num) -> tigers... => tigers(substr)
		if (WordTank[i][0] == '\0')
		{
			//cout<<"空格位置："<<i<<endl;//system("pause");
			i--;//如果当前接收的是一个空串，则接受下一个字符串 
		}
		//统计单词数 
		if (jud_Comma)
		{
			if (!EndOfArticle)
			{
				i += (num + 1);//有几个标点符号说明有几个单词
			}		
		}
		else
		{
			i++;
		}
	}
	//TODO 有问题,比如 tigers,two会被当成一个单词
	totalWordNum = i - 1;//t为常量 
	//cout<<"单词总数："<<t<<endl;

	//频率数组初始化 
	for (i = 1; i < totalWordNum; i++)
	{
		frequency[i] = 1;
	}

	return;
}



void PrintMenu()
{
	cout << "基于不同策略的英文单词的词频统计和检索系统" << endl;
	//cout<<"文本位置 ";system("cd"); 
	cout << "单词总数：" << totalWordNum << endl;
	if (totalWordNum > 800)cout << "注意:当前单词数量过多，可能导致程序崩溃" << endl;
	cout << endl;
	cout << "---菜单---" << endl;
	cout << "1.基于顺序表的查找" << endl;
	cout << "2.基于链表的查找" << endl;
	cout << "3.基于二叉排序树的查找" << endl;
	cout << "4.哈希表" << endl;
	cout << "5.单词总数" << endl;
	cout << "6.单词数组打印" << endl;
	cout << "7.高频词输出" << endl;
	cout << "8.文章打印" << endl;
	cout << "9.比较" << endl;
	cout << "0.链表:删除低频词" << endl;
	cout << "a.二叉排序树：删除低频词" << endl;
	cout << "b.以二叉排序树结构输出" << endl;
	cout << "----------" << endl;
	cout << "请按相应的数字键进行选择：" << endl;
}

void PrintMenu1()
{
	cout << "基于不同策略的英文单词的词频统计和检索系统" << endl;
	cout << "单词总数：" << totalWordNum << endl;
	if (totalWordNum > 800)cout << "注意:当前单词数量过多，可能导致程序崩溃" << endl;
	cout << endl;
	cout << "---菜单---" << endl;
	cout << "1.顺序查找" << endl;
	cout << "2.折半查找" << endl;
	cout << "3.删除低频词" << endl;
	cout << "4.返回上一级" << endl;
	cout << "----------" << endl;

	cout << "请按相应的数字键进行选择：" << endl;
}

void PrintMenu4()
{
	cout << "基于不同策略的英文单词的词频统计和检索系统" << endl;
	cout << "单词总数：" << totalWordNum << endl;
	if (totalWordNum > 800)cout << "注意:当前单词数量过多，可能导致程序崩溃" << endl;
	cout << endl;
	cout << "---菜单---" << endl;
	cout << "1.开放地址法" << endl;
	cout << "2.链地址法" << endl;
	cout << "3.返回上一级" << endl;
	cout << "----------" << endl;
	cout << "请按相应的数字键进行选择：" << endl;
}

void QuickSort1(SSTable &L, int l, int r)//l=1,r=L.length
{
	if (l < r)
	{
		int i = l, j = r; Word x = L.R[l];
		while (i < j)
		{
			while (i < j && L.R[j].key >= x.key) // 从右向左找第一个小于x的数
				j--;
			if (i < j)
				L.R[i++] = L.R[j];

			while (i < j && L.R[i].key < x.key) // 从左向右找第一个大于等于x的数
				i++;
			if (i < j)
				L.R[j--] = L.R[i];
		}
		L.R[i] = x;
		QuickSort1(L, l, i - 1); // 递归调用 
		QuickSort1(L, i + 1, r);
	}
}

void Sort1(SSTable &L)
{
	int i = 0, j = 0;//外i内j 
	for (i = 1; i <= L.length; i++)//外层循环n-1 
		for (j = 1; j <= L.length - i; j++)//内层循环n-1-i 
		{
			if (L.R[j].key > L.R[j + 1].key)
			{
				Word temp;
				temp = L.R[j];
				L.R[j] = L.R[j + 1];
				L.R[j + 1] = temp;
			}
		}
}

void Del1(SSTable &L, int no)
{
	no = no + 1;
	int i;
	if (no > L.length || no < 1)
	{
		cout << "Sorry，the position to be deleted is invalid!" << endl;
		return;
	}
	for (i = no; i < L.length; i++)
	{
		L.R[i - 1] = L.R[i];
	}
	L.length--;
	return;
}

void DelLowRate1(SSTable &L)
{
	cout << "删除前表长:" << L.length << endl;
	for (int i = 1; i < L.length; i++)
	{
		if (10000 * (double)L.R[i].Time / totalWordNum < 100)
		{
			Del1(L, i);
			i--;//key	
		}
	}
	cout << "删除后表长:" << L.length << endl;
	for (int i = 1; i < L.length; i++)
	{
		if (10000 * (double)L.R[i].Time / totalWordNum)
		{
			cout << "剩余" << L.R[i].key << " " << L.R[i].Time << " " << 10000 * (double)L.R[i].Time / totalWordNum << endl;
		}
	}
}

void Del2(LinkList &L, int no)
{
	int i; int j;
	if (no > ListLength(L) || no < 1)
	{
		cout << "Sorry，the position to be deleted is invalid!" << endl;
		return;
	}
	LinkList p, q;
	p = L; j = 0;
	while (p && (j < no - 1))
	{
		p = p->next;
		j++;
	}
	q = p->next;
	p->next = q->next;
	delete q;
	return;
}

void DelLowRate2(LinkList &L)
{
	cout << "删除前表长:" << ListLength(L) << endl;
	LinkList p;
	p = L;
	int i = 0;
	while (p->next)
	{
		i++;
		p = p->next;
		if (10000 * (double)p->data.Time / totalWordNum < 100)
		{
			Del2(L, i);
			i--;//key	
		}
	}
	cout << "删除后表长:" << ListLength(L) << endl;
	p = L;
	while (p->next)
	{
		p = p->next;
		if (10000 * (double)p->data.Time / totalWordNum)
		{
			cout << "剩余" << p->data.key << " " << p->data.Time << " " << 10000 * (double)p->data.Time / totalWordNum << endl;
		}
	}
}

//根据节点的三种情况来删除节点
int Delete(BSTree	& p)
{
	BSTree q, s;
	if (p->rchild == NULL)
	{
		q = p; p = p->lchild; free(q); cout << "删除完成1" << endl;
	}
	else if ((p)->lchild == NULL)
	{
		q = p; p = p->rchild; free(q); cout << "删除完成2" << endl;
	}
	else
	{
		q = p; s = p->lchild;
		while (s->rchild)
		{
			q = s; s = s->rchild;
		}
		p->data = s->data;
		if (q != p)
			q->rchild = s->lchild;
		else
			q->lchild = s->lchild;
		free(s);
		cout << "删除完成3" << endl;
	}
	return true;
}

int DeleteBST(BSTree& T, string key)
{
	if (!T)
		return false;
	else
	{
		if (key == T->data.key)
		{
			return Delete(T);
		}
		else if (key < T->data.key)
		{
			return DeleteBST(T->lchild, key);
		}
		else
		{
			return DeleteBST(T->rchild, key);
		}
	}
}

void DeleteBST2(BSTree &T, string key) {
	//从二叉排序树T中删除关键字等于key的结点
	BSTree p = T; BSTree f = NULL;                     			//初始化
	BSTree q = nullptr;
	BSTree s = nullptr;
	while (p)
	{
		if (p->data.key == key)
		{
			break;
		}
		f = p;
		if (p->data.key > key)
		{
			p = p->lchild;
		}
		else
		{
			p = p->rchild;
		}
	}
	if (!p)
	{
		return;
	}
	if ((p->lchild) && (p->rchild))
	{
		q = p;
		s = p->lchild;
		while (s->rchild)
		{
			q = s; s = s->rchild;
		}
		p->data = s->data;
		if (q != p)
		{
			q->rchild = s->lchild;
		}
		else q->lchild = s->lchild;
		delete s;
	}//if
	else
	{
		if (!p->rchild)
		{

			q = p;
			p = p->lchild;

		}//else if
		else if (!p->lchild)
		{
			q = p;	  p = p->rchild;
		}//else if
		if (!f)
		{
			T = p;
		}
		else
		{
			if (q == f->lchild)
			{
				f->lchild = p;
			}
			else
			{

				f->rchild = p;
			}
		}
		delete q;
	}
}//DeleteBST

int DelLowRate3(BSTree &T)
{
	if (T)
	{
		DelLowRate3(T->lchild);
		if (10000 * T->data.Time / totalWordNum > 100)
		{
			string key1 = T->data.key;
			//DeleteBST(T,key1);
			Insert(T1, T->data);//Word e
			NonReaptedWords++;
		}
		else
		{
			//	cout<<"保留:"<<T->data.key<<" "<<10000*(double)T->data.Time/t<<endl;
		}
		DelLowRate3(T->rchild);
	}
	return 0;
}

void MenuLinearList(SSTable &L)
{
	//菜单 
	system("cls");
	PrintMenu1();
	char ch;
	cin >> ch;
	//Sort1(L);
	QuickSort1(L, 1, L.length);
	Output1(L);//输出 
	string sk;
	switch (ch)
	{
	case '1':	cout << "顺序查找，输入你要查找的单词：" << endl;
		cin >> sk;
		Search1_Order(L, sk); system("pause"); system("cls"); break;

	case '2':	cout << "折半查找，输入你要查找的单词：" << endl;
		cin >> sk;
		Search1_Half(L, sk); system("pause"); system("cls");
		break;

	case '3':   DelLowRate1(L); system("pause"); system("cls"); break;

	case '4':	return;
	}
}

int Hash(string key)
{
	int n;
	n = key[0] - 'a';//用ASCII码作差，计算首字母的序号 
	int result = 100 * n;//每一个字母分配100个数组空间，a序号就是0，空间为0-100. 
	return result;
}

int Search4(HashTable HT[], string key) {
	//在哈希表中查找,若查找成功，返回序号，否则返回-1 
	int H0 = Hash(key);     	                   			//计算哈希地址
	int Hi;
	if (HT[H0].key == NULLKEY) return -1;		//若单元H0为空，则所查元素不存在
	else if (HT[H0].key == key) return H0;		//若单元H0中元素的关键字为key，则查找成功
	else {
		for (int i = 1; i < m; ++i)
		{
			Hi = (H0 + i) % m;     		//按照线性探测法计算下一个哈希地址Hi
			if (HT[Hi].key == NULLKEY) return -1;	//若单元Hi为空，则所查元素不存在
			else if (HT[Hi].key == key) return Hi; 	//若单元Hi中元素的关键字为key，则查找成功
		}//for
		return -1;
	}//else
}

int CheckHash(HashTable HT[], string key) {
	int H0 = Hash(key);
	int Hi;
	if (HT[H0].key == NULLKEY) return -1;
	else if (HT[H0].key == key) return H0;
	else {
		for (int i = 1; i < m; ++i) {
			Hi = (H0 + i) % m;
			if (HT[Hi].key == NULLKEY) return -1;
			else if (HT[Hi].key == key)
			{
				HT[Hi].Time++;
				return Hi;
			}
		}//for
		return -1;
	}//else
}

void QuickSort(HashTable HT[], int l, int r)
{
	if (l < r)
	{
		int i = l, j = r; HashTable x = HT[l];
		while (i < j)
		{
			while (i < j && HT[j].key >= x.key) // 从右向左找第一个小于x的数
				j--;
			if (i < j)
				HT[i++] = HT[j];

			while (i < j && HT[i].key < x.key) // 从左向右找第一个大于等于x的数
				i++;
			if (i < j)
				HT[j--] = HT[i];
		}
		HT[i] = x;
		QuickSort(HT, l, i - 1); // 递归调用 
		QuickSort(HT, i + 1, r);
	}
}

void Sort4(HashTable HT[])
{
	int i = 0; int j; int res = 0;
	for (i = 0; i <= 26; i++)
	{
		for (j = i * 100 + 1; j < (i + 1) * 100; j++)
		{
			if (HT[j].key == "\0")
			{
				res = j;
				break;
			}
		}
		QuickSort(HT, i * 100 + 1, res);//不能为(i+1)*100，因为后面都是空，只能到res				
	}
	return;
}

void Output4(HashTable HT[])
{
	int i;
	fstream out;
	out.open("OutFile4.txt", fstream::out | ios_base::trunc);
	out << "单词 词频" << endl;
	for (int i = 1; i < m; i++)
		if (HT[i].key != "\0")
		{
			out << HT[i].key << " " << HT[i].Time << endl;
		}
	return;
}


void Input4(HashTable HT[])
{
	//哈希表初始化 
	for (int i = 1; i < MAXSIZE; i++)//不能有等于，越界就崩溃 
	{
		HT[i].key = "\0";
		//cout<<i<<endl;
	}
	//构建 
	for (int i = 1; i <= 5000; i++)//300-5000
	{
		if (WordTank[i][0] == '\0')break; //或者i==t 
		while (1)
		{
			int res = CheckHash(HT, WordTank[i]);//在此实现 
			if (res == -1)break;
			else
			{
				i++;
				//Time++;无法在此实现 
			}
		}
		int n;
		n = WordTank[i][0] - 'a';
		int max = 100 * n + 100;
		int p = max - 100 + 1;//从1开始,0开始会有错误 
		while (p < max)
		{
			if (HT[p].key == "\0")//用checkhash记录频次 &&CheckHash(HT,str[i])==-1
			{
				HT[p].key = WordTank[i];
				HT[p].Time = 1;
				//cout<<HT[p].key<<"已录入第"<<p<<"个位置"<<endl;
				break;
			}
			else	p++;
		}
	}
}

void HashOpenAddress(HashTable HT[])
{
	Input4(HT);
	Sort4(HT);
	Output4(HT);
	string sk;
	cout << "开放地址法，输入你要查找的单词：" << endl;
	cin >> sk;

	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);
	int result = Search4(HT, sk);
	QueryPerformanceCounter(&ed);
	time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;

	if (result != -1)
	{
		cout << "此单词的词频为：" << HT[result].Time << endl;
		cout << "查找所花费的次数：" << result % 100 << endl;
		cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
		cout << "平均查找长度：" << NonReaptedWords / 26 << endl;
		cout << "在第" << result << "位置找到" << endl;
	}
	else
	{
		cout << "查找失败" << endl;
	}
	return;
}

void HashOpenAddress_1(HashTable HT[], string sk)
{

	Input4(HT);
	Sort4(HT);
	Output4(HT);

	cout << "开放地址法：" << endl;

	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);
	int result = Search4(HT, sk);
	QueryPerformanceCounter(&ed);
	time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;

	if (result != -1)
	{
		cout << "此单词的词频为：" << HT[result].Time << endl;
		cout << "查找所花费的次数：" << result % 100 << endl;
		cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
		cout << "平均查找长度：" << NonReaptedWords / 26 << endl;
		cout << "在第" << result << "位置找到" << endl;
	}
	else
	{
		cout << "查找失败" << endl;
	}
	return;
}

void Output5(ALGraph G)
{
	fstream out;
	out.open("OutFile5.txt", fstream::out | ios_base::trunc);
	int i;
	//HCSort(G);
	out << "单词" << " " << "词频" << endl;
	for (i = 1; i <= 26; i++)
	{
		WordNode *h;
		h = G.Letter[i].firstarc;
		while (h != NULL)
		{
			out << h->data.key << " " << h->data.Time << endl;
			h = h->nextarc;
		}
	}
	out.close();
	return;
}

void Sort5(ALGraph &G)
{
	int i;
	for (i = 1; i <= 26; i++)
	{
		WordNode *p, *q;
		Word t;
		for (p = G.Letter[i].firstarc; p != NULL; p = p->nextarc)
		{
			for (q = p->nextarc; q != NULL; q = q->nextarc)
			{
				if (p->data.key > q->data.key)
				{
					t = q->data;
					q->data = p->data;
					p->data = t;
				}
			}
		}
	}
	return;
}

void HashChainAddress(ALGraph G, string sk)
{
	cout << "基于链地址法：" << endl;
	Sort5(G);
	Output5(G);
	int k;
	int i;
	k = sk[0];
	k = k - 96;
	WordNode *h;
	int flag = 0;
	h = G.Letter[k].firstarc;
	int j = 0;
	int r;

	LARGE_INTEGER fr;
	LARGE_INTEGER st;
	LARGE_INTEGER ed;
	double time;
	QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&st);

	while (h != NULL)
	{
		j++;
		if (h->data.key == sk)
		{
			flag = j;
			r = h->data.Time;
		}
		h = h->nextarc;
	}

	QueryPerformanceCounter(&ed);
	time = (double)(ed.QuadPart - st.QuadPart) / (double)fr.QuadPart;

	if (flag > 0)
	{

		cout << "此单词的词频为：" << r << endl;
		cout << "查找该单词所花的次数：" << flag + 1 << endl; flag = (totalWordNum + 1) / 2;
		cout << "查找所花费的时间：" << setprecision(6) << time * 1000000 << "ns" << endl;
		cout << "平均查找长度：" << flag << endl;
	}
	else
	{
		cout << "查找失败" << endl;
	}
	return;
}

void Input5(ALGraph &G)
{
	int i;
	//初始化顶点表 
	for (i = 1; i <= 26; i++)
	{
		G.Letter[i].letter_no = i;
		G.Letter[i].firstarc = NULL;
	}
	int num = 0;//单词计数 
	for (i = 0; i < totalWordNum; i++)
	{
		int k;
		k = Hash_int(i);//判断单词首字母序号 
		WordNode *h = nullptr, *w = nullptr, *e = nullptr, *p = nullptr, *r = nullptr;
		h = G.Letter[k].firstarc;//h是单词的首字母表头结点 
		if (h == NULL)//表头结点为空，录入 
		{
			p = new WordNode;
			p->data.key = WordTank[i];
			p->data.Time = frequency[i];
			p->nextarc = NULL;
			G.Letter[k].firstarc = p;
			num++;
		}
		else
		{
			int flag = 0;
			while (h != NULL)
			{
				r = h;
				if (h->data.key == WordTank[i])//遍历一个首字母的链表，如果相同，频次自加 
				{
					h->data.Time++;
					flag = 1;
				}
				h = h->nextarc;// 
			}
			if (flag == 0)//如果没有相同的，就录入 
			{
				p = new WordNode;
				p->data.key = WordTank[i];
				p->data.Time = frequency[i];
				p->nextarc = NULL;
				r->nextarc = p;
				num++;
			}
		}
	}
	NonReaptedWords = num;//有效单词数 
	return;
}

void MenuHash(HashTable HT[], ALGraph G)
{
	//菜单 
	system("cls");
	PrintMenu4();
	char ch;
	cin >> ch;
	string sk;
	switch (ch)
	{
	case '1':	HashOpenAddress(HT); system("pause"); system("cls"); break;

	case '2':	cout << "链地址法，输入你要查找的单词：" << endl;
		cin >> sk;
		HashChainAddress(G, sk); system("pause"); system("cls");
		break;

	case '3':   return;
	}
}

void PrintMenu7()
{
	cout << "基于不同策略的英文单词的词频统计和检索系统" << endl;
	cout << "单词总数：" << totalWordNum << endl;
	if (totalWordNum > 800)cout << "注意:当前单词数量过多，可能导致程序崩溃" << endl;
	cout << endl;
	cout << "---菜单---" << endl;
	cout << "1.顺序表" << endl;
	cout << "2.链表" << endl;
	cout << "3.二叉排序树" << endl;
	cout << "4.开放地址法" << endl;
	cout << "5.链地址法" << endl;
	cout << "6.概率区间筛选" << endl;
	cout << "7.返回上一级" << endl;
	cout << "----------" << endl;
	cout << "请按相应的数字键进行选择：" << endl;
}

int TraverseOut1(BSTree T, int &i)
{
	if (T)
	{
		fstream out;
		out.open("OutFile1.txt", fstream::out | ios_base::trunc);
		TraverseOut1(T->lchild, i);
		i++;
		if (10000 * (double)T->data.Time / totalWordNum < setrate_Million_Smaller)//
		{
			out << T->data.key << " " << T->data.Time << endl;
			cout << "i:" << i << " " << T->data.key << " " << T->data.Time << "/" << totalWordNum << " " << 10000 * (double)T->data.Time / totalWordNum << endl;
		}
		TraverseOut1(T->rchild, i);
	}
	return 0;
}

void Rate3(BSTree T)
{
	int i = 0;
	cout << "单词" << " " << "频次" << " " << "概率(万分之一)" << endl;
	TraverseOut1(T, i);
}

void Rate4(HashTable HT[])
{
	fstream out;
	out.open("OutFile1.txt", fstream::out | ios_base::trunc);
	cout << "单词" << " " << "频次" << " " << "概率(万分之一)" << endl;
	for (int i = 1; i < m; i++)
		if (HT[i].key != "\0")
		{
			if (10000 * (double)HT[i].Time / totalWordNum > setrate_Million_Smaller)
			{
				out << HT[i].key << " " << HT[i].Time << endl;
				cout << "i:" << i << " " << HT[i].key << " " << HT[i].Time << "/" << totalWordNum << " " << 10000 * (double)HT[i].Time / totalWordNum << endl;
			}

		}
}

void Rate5(ALGraph G)
{
	fstream out;
	out.open("OutFile1.txt", fstream::out | ios_base::trunc);
	int k = 1;
	//sort
	Sort5(G);
	cout << "单词" << " " << "频次" << " " << "概率(万分之一)" << endl;
	for (int i = 1; i <= 26; i++)
	{
		WordNode *h;
		h = G.Letter[i].firstarc;
		while (h != NULL)
		{
			if (10000 * (double)h->data.Time / totalWordNum > setrate_Million_Smaller)
			{
				out << h->data.key << " " << h->data.Time << endl;
				cout << "i:" << k << " " << h->data.key << " " << h->data.Time << "/" << totalWordNum << " " << 10000 * (double)h->data.Time / totalWordNum << endl;
			}
			h = h->nextarc;
			k++;
		}
	}
	return;
}


void Rate6(SSTable L)
{

	cout << "请输入筛选的概率区间：" << endl;
	cin >> setrate_Million_Smaller >> setrate2;
	int i = 0;//fre[5]=1;
	cout << "单词" << " " << "频次" << " " << "概率(万分之一)" << endl;
	while (i <= NonReaptedWords)
	{
		if (flag_First_occur[i] == 1)//如果是第一个元素，就输出 
		{
			frequency[i] = L.R[i].Time;
			rate[i] = (double)frequency[i] / totalWordNum;
			rate[i] = rate[i] * 10000;
			if (rate[i] > setrate_Million_Smaller&&rate[i] < setrate2)//&&rate[i]<100000
			{
				cout << "i:" << i << " " << L.R[i].key << " " << frequency[i] << "/" << totalWordNum << " " << rate[i] << endl;
			}
		}
		i++;
	}
}


void MenuHighRate(SSTable L, LinkList L1, BSTree T, HashTable HT[], ALGraph G)
{
	//频率初始化 
	for (int i = 1; i < totalWordNum; i++)
	{
		frequency[i] = 1;
		rate[i] = 0;
	}

	//菜单 
	system("cls");
	PrintMenu7();
	char ch;
	cin >> ch;
	string sk;
	switch (ch)
	{
	case '1':	Rate1(L); system("pause"); system("cls"); break;

	case '2':	Rate2(L1); system("pause"); system("cls"); break;

	case '3':   Rate3(T); system("pause"); system("cls"); break;

	case '4':	Input4(HT); system("pause"); system("cls"); Sort4(HT); Rate4(HT); break;

	case '5':	Rate5(G); system("pause"); system("cls"); break;

	case '6':	Rate6(L); system("pause"); system("cls"); break;

	}
}

void Compare(SSTable L, LinkList L1, BSTree T, HashTable HT[], ALGraph G)
{
	cout << "请输入您要查找的单词：" << endl;
	string sk;
	cin >> sk;

	Sort1(L);
	Sort2(L1);
	Sort4(HT);
	Sort5(G);

	Search1_Order(L, sk); cout << endl;
	Search1_Half(L, sk); cout << endl;
	Search2_1(L1, sk); cout << endl;
	Search3_1(T, sk); cout << endl;
	HashOpenAddress_1(HT, sk); cout << endl;
	HashChainAddress(G, sk); cout << endl;
	//若查找的是文章中最后一个词，会出现查找失败的情况。 

	Output1(L);
	Output2(L1);
	Output3(T);
	Output4(HT);
	Output5(G);
	cout << "统计数据已存入文件夹!" << endl;

	return;
}



int main()
{
	//system("color 0B");
	Preprocessing();//得到单词总数t与单词数组str[t] 

	//顺序存储结构 
	SSTable L; InitList(L);
	Input1(L);//录入单词，词频统计 


	//链式存储结构
	LinkList L1;
	Input2(L1);//录入单词


	//二叉排序树
	BSTree T;
	InputTree(T);
	//InputTree(T);

	//开放地址法 
	HashTable HT[m];
	cout << "1" << endl;//不输出这个1程序会崩溃，暂时未查明原因

	//链地址法
	ALGraph G;
	//Input5(G);


	G.Letter->firstarc = nullptr;
	//菜单 
	while (1)
	{

		system("cls");
		PrintMenu();
		//system("ver");
		char ch;
		cin >> ch;
		switch (ch)
		{
		case '1':MenuLinearList(L); break;
		case '2':Search2(L1); system("pause"); system("cls"); break; break;
		case '3':Search3(T); system("pause"); system("cls"); break; break;
		case '4':MenuHash(HT, G); break;
		case '5':cout << "单词总数:" << totalWordNum << endl; system("pause"); system("cls"); break;
		case '6':PrintWord(); system("pause"); system("cls"); break;
		case '7':MenuHighRate(L, L1, T, HT, G); break;
		case '8':PrintText(); system("pause"); system("cls"); break;
		case '9':Compare(L, L1, T, HT, G); system("pause"); system("cls"); break;
		case '0':DelLowRate2(L1); system("pause"); system("cls"); break;
		case 'a':NonReaptedWords = 1; DelLowRate3(T); T = T1; system("pause"); system("cls"); break;
		case 'b':TraverseOut2(T); system("pause"); system("cls"); break;
		}
		system("pause");
		system("cls");
	}
	return 0;
}