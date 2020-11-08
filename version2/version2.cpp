
#include <iostream>
#include <fstream>   //c++操作文件所需的头文件
#include <cstring>
#include <ctype.h>	//判断字符是否是字母 
#include<cmath>		//数学方法
#include<string>
using namespace std;
#define MAXSIZE 10000   //顺序表的最大长度

int counter = 1;	//既是单词个数又是线性表下标，一会儿输出单词个数记得count-1
int words = 0;	//统计欲查找单词频数 
int words_num[MAXSIZE];		//存放所有单词词频 
typedef struct {
	string key;		//存放单词
	bool flag;		//设置标志，若查找单词在后面出现过，就把该单词所在位置下的flag置为false
	int words;		//统计词频,若查询单词在后面再次出现，words++ 
}ElemType;
typedef struct {					//线性表结构
	ElemType elem[MAXSIZE];
	int length;
}SqList;

typedef struct LNode {			//单链表结构 
	string data;
	struct LNode *next;
}LNode, *LinkList;

typedef struct BSTNode {			//二叉链表结构 
	string data;
	struct BSTNode *Lchild, *Rchild;
}BSTNode, *BSTree;

void Init_SqList(SqList &L)			//初始化线性表 
{
	L.length = 0;
	for (int i = 0; i < MAXSIZE; i++)		//所有单词初始化为true(未发现相同词)
	{
		L.elem[i].flag = true;
		L.elem[i].words = 1;
	}
}

void Init_LinkList(LinkList &L)		//初始化单链表 
{
	L = new LNode;
	L->next = NULL;
}
bool judge_alphabet(char ch)		//判断是否为英文字母 
{
	if (isalpha(ch))
		return true;
	else
		return false;
}
int detect(const char *p, SqList &L, LinkList &LL)
{
	int i = 0, m;
	char temp[20];
	int start, end;
	while (p[i] <127&&p[i]>0 )		//当非空行  不超过ASCILL范围 防止越界
	{
		while (!judge_alphabet(p[i]) )//不是字母
		{
			i++;
		}
		/*		
		设置一个start变量，将这个单词的首字母所在位置记录下来，然后一直向后找，
		直到再次找到非英文字母，证明一个单词结束，再设置一个end变量记录下结束位置。
		这样就截取到了一个单词的起始位置。
		*/
		if (judge_alphabet(p[i]))
		{
			start = i++;//确定起点，向后遍历一位
			//i++;
			while (judge_alphabet(p[i]))
			{
				i++;
			}
		}
		end = i;//确定终点
		for (int j = start, k = 0; j <= end; k++, j++)
		{
			temp[k] = p[j];
			m = k;
		}
		temp[m] = '\0';
		L.elem[counter].key = temp;			//counter为单词个数，也可作为线性表存储单词的下标	 
		counter++;
		i++;//向后遍历，跳过标点
		LNode *s = new LNode;
		s->data = temp;
		s->next = LL->next;
		LL->next = s;
	}
}

//将文件C:\book.txt中的数据逐条读入顺序表L中
void ReadData(SqList &L, LinkList &LL, BSTree &T)
{
	int i = 0;
	string temp;
	ifstream infile("InFile.txt");  //以输入的方式（读文件的方式）打开文件c:\book.txt
	while (!infile.eof())   //只要文件没读完，就读一行数据到顺序表L中的第i个分量中
	{
		getline(infile, temp);
		const char *p = temp.c_str();
		int len = strlen(p);
		if (len != 0)
		{
			detect(p, L, LL);
		}
	}
	L.length = counter - 1;
	infile.close();
}

void WriteData(SqList L)
{
	int q;
	ofstream outfile("words.txt");		//将单词总数及词频信息保存在新的txt中 
	outfile << "单词总数为:" << L.length << endl;
	for (q = 1; q < L.length; q++)
	{
		if (words_num[q] != 0)
			outfile << L.elem[q].key << "	" << words_num[q] << endl;
	}
	outfile.close();
}

void PrintList(SqList L, LinkList LL)
{
	cout << "\n\n全文共有" << L.length << "个单词" << endl;
	LNode *p;
	p = LL->next;
	/*while(p!=NULL)		//以单链表形式输出所有单词
	{
		cout<<p->data;
		p=p->next;
	}
	cout<<endl;*/
	for (int p = 0; p < counter; p++)		//以线性表形式输出所有单词 
	{
		cout << L.elem[p].key;
	}
}

void Getnum(SqList &L)		//获取文章中所有单词词频 
{
	int j = 1;
	for (int a = 1; a <= L.length; a++)	//外层循环，拿第a个单词和线性表所有单词进行比较 
	{
		int nums = 0;
		if (L.elem[a].flag == true)	//true证明还没有相同单词，执行比较操作 
		{
			for (int i = 1; i <= L.length; i++)		//内层循环，依次遍历整个线性表 
			{
				if (L.elem[a].key == L.elem[i].key)
				{
					L.elem[i].flag = false;
					nums++;
				}
				else	continue;
			}
			words_num[j] = nums;		//将所有单词词频存放在word_num这个数组中 
			L.elem[j].key = L.elem[a].key;
			j++;
		}
	}
}

int Sq_Search(SqList L, string key)
{
	int i;
	L.elem[0].key = key;
	for (i = 1; i <= L.length; i++)
	{
		if (L.elem[i].key == key)
		{
			words++;
		}
	}
	return words;
}

int LL_Search(LinkList L, string key)
{
	LNode *p;
	p = L->next;
	while (p != NULL)
	{
		if (p->data == key)
			words++;
		p = p->next;
	}
	return words;

}

void InsertSort(SqList &L)		//使用直接插入法对线性表L进行排序 
{
	int i, j;
	for (i = 2; i < L.length; i++)
	{
		if (L.elem[i].key < L.elem[i - 1].key)
		{
			L.elem[0].key = L.elem[i].key;
			for (j = i - 1; L.elem[0].key < L.elem[j].key; j--)
				L.elem[j + 1].key = L.elem[j].key;
			L.elem[j + 1].key = L.elem[0].key;
		}
	}
}
int Search_Bin(SqList L, string key)		//线性表折半查找 
{
	int low = 1, high = L.length, mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (key == L.elem[mid].key)
		{
			return mid;
		}
		else if (key < L.elem[mid].key)
			high = mid - 1;
		else	low = mid + 1;
	}
	return 0;
}

void Insert_BST(BSTree &T, string e)		//二叉排序树中插入结点 
{
	BSTNode *s;
	if (!T)
	{
		s = new BSTNode;
		s->data = e;
		s->Lchild = s->Rchild = NULL;
		T = s;
	}
	else if (e < T->data)
		Insert_BST(T->Lchild, e);
	else if (e > T->data)
		Insert_BST(T->Rchild, e);
}
void Create_BST(BSTree &T, SqList L)		//创建二叉排序树 
{
	int i = 1;
	string e;
	T = NULL;
	e = L.elem[1].key;
	while (i <= L.length)
	{
		Insert_BST(T, e);
		i++;
		e = L.elem[i].key;
	}
}
void InorderBST(BSTree T)		//中序遍历并输出二叉排序树 
{
	if (T)
	{
		InorderBST(T->Lchild);
		cout << T->data << "  ";
		InorderBST(T->Rchild);
	}
}

BSTree SearchBST(BSTree T, string key)		//二叉排序树的查找 
{
	if (!T || key == T->data)	return T;
	else if (key < T->data)	return SearchBST(T->Lchild, key);
	else return SearchBST(T->Rchild, key);
}

void Delete(BSTree &T, BSTNode *p, BSTNode *f)		//二叉排序树中p所指结点的删除 
{
	BSTNode *q, *s;
	q = p;
	if (p->Rchild  && p->Lchild)   //p所指结点的左右子树均非空
	{
		s = p->Lchild;
		while (s->Rchild)
		{
			q = s;
			s = s->Rchild;
		}
		p->data = s->data;
		if (q == p)	q->Lchild = s->Lchild;
		else q->Rchild = s->Lchild;
		delete s;
	}
	else if (p->Rchild == NULL)   //p所指结点只有左子树
	{
		p = p->Lchild;
	}
	else if (p->Lchild == NULL)   //p所指结点只有右子树
	{
		p = p->Rchild;
	}
	if (f == NULL) T = NULL;    //T只有一个根结点，且被删除的是根结点
	else if (q == f->Lchild)  f->Lchild = p;   //被删结点是双亲f的左孩子
	else   f->Rchild = p;    //被删结点是双亲f的右孩子
	delete q;    //释放被删结点
}

bool DeleteBST(BSTree &T, string key)		//二叉排序树的删除 
{
	BSTNode *p = T, *f = NULL;
	while (p)     //从二叉排序树的根开始，查找关键字为key的结点，并用p指向它，f指向p的双亲
	{
		if (p->data == key)     //找到关键字为key的结点，且p指向它
			break;
		f = p;                       //f为p的双亲
		if (p->data > key)      //在p的左子树中继续查找
			p = p->Lchild;
		else                       //在p的右子树中继续查找
			p = p->Rchild;
	}
	if (p == NULL)            //T中不存在关键字等于key的结点，返回false
		return false;
	Delete(T, p, f);         //在二叉排序树T中删除p所指结点（关键字为key的结点），f指向p的双亲
	return true;           //删除成功，返回true
}

void show_main_menu()
{
	cout << "			-----------------------------------------" << endl;
	cout << "				**基于不同策略的单词检索系统**\n" << endl;
	cout << "				1.基于顺序表的顺序查找" << endl;
	cout << "				2.基于链表的顺序查找" << endl;
	cout << "				3.基于顺序表的折半查找" << endl;
	cout << "				4.基于二叉排序树的查找" << endl;
	cout << "				5.退出系统" << endl;
	cout << "			-----------------------------------------" << endl;
}
void show_sec_menu()
{
	cout << "			****************************" << endl;
	cout << "				1.词频统计" << endl;
	cout << "				2.单词查找" << endl;
	cout << "				3.返回上一级" << endl;
	cout << "			****************************" << endl;
}
void show_BiTree_menu()						//二叉排序树多一个删除功能，把菜单单独拉出来处理 
{
	cout << "			****************************" << endl;
	cout << "				1.词频统计" << endl;
	cout << "				2.单词查找" << endl;
	cout << "				3.删除单词" << endl;
	cout << "				4.返回上一级" << endl;
	cout << "			****************************" << endl;
}


int main()
{
	int start_time, end_time;
	SqList LSq;
	LinkList LL;
	BSTree T;
	string word;
	int num;	//单词个数
	BSTree num1;
	int n;
	Init_SqList(LSq);		//初始化线性表 
	Init_LinkList(LL);		//初始化单链表 
	ReadData(LSq, LL, T);
	Create_BST(T, LSq);
	//InorderBST(T);			//测试 中序输出二叉排序树 
	do
	{
		show_main_menu();
		cout << "请选择一个你想实现的功能:";
		cin >> n;
		cout << endl;
		switch (n)
		{
		case 1:show_sec_menu();
			cout << "请输入你想具体实现内容的序号:";
			cin >> n;
			cout << endl;
			switch (n)
			{
			case 1:Getnum(LSq);
				WriteData(LSq);
				cout << "\n写入数据成功！！\n请在D盘打开words.txt文件查看\n" << endl; break;
			case 2:cout << "请输入要查询的单词:";
				cin >> word;
				num = Sq_Search(LSq, word);
				if (num > 0)
					cout << "查找成功！" << "\n该词词频为:" << num << "\n查找ASL为:" << (LSq.length + 1) / 2 << endl << endl;
				else
					cout << "很抱歉，查找失败！" << endl; break;
			case 3:break;
			default:cout << "请输入正确的序号！！！\n";
			}break;
		case 2:show_sec_menu();
			cout << "请输入你想具体实现内容的序号:";
			cin >> n;
			cout << endl;
			switch (n)
			{
			case 1:Getnum(LSq);
				WriteData(LSq);
				cout << "\n写入数据成功！！\n请在D盘打开words.txt文件查看\n" << endl; break;
			case 2:cout << "请输入要查询的单词:";
				cin >> word;
				num = LL_Search(LL, word);
				if (num > 0)
					cout << "查找成功！" << "\n该词词频为:" << num << "\n查找ASL为:" << (LSq.length + 1) / 2 << endl << endl;
				else
					cout << "很抱歉，查找失败！" << endl; break;
			case 3:break;
			default:cout << "请输入正确的序号！！！\n";
			}break;
		case 3:show_sec_menu();
			cout << "请输入你想具体实现内容的序号:";
			cin >> n;
			cout << endl;
			switch (n)
			{
			case 1:Getnum(LSq);
				WriteData(LSq);
				cout << "\n写入数据成功！！\n请在D盘打开words.txt文件查看\n" << endl; break;
			case 2:cout << "请输入要查询的单词:";
				cin >> word;
				InsertSort(LSq);
				num = Search_Bin(LSq, word);
				if (num > 0)
					cout << endl << word << "查询成功！！\n" << "\n查找ASL为:" << log(LSq.length + 1) / log(2) - 1 << endl << endl;
				else cout << "\n很抱歉，该文章没有你想查询的词(\n"; break;
			case 3:break;
			default:cout << "请输入正确的序号！！！\n";
			}break;
		case 4:show_BiTree_menu();
			cout << "请输入你想具体实现内容的序号:";
			cin >> n;
			cout << endl;
			switch (n)
			{
			case 1:Getnum(LSq);
				WriteData(LSq);
				cout << "\n写入数据成功！！\n请在D盘打开words.txt文件查看\n" << endl; break;
			case 2:Create_BST(T, LSq);
				cout << "请输入要查询的单词:";
				cin >> word;
				num1 = SearchBST(T, word);
				if (num1 > 0)
					cout << endl << word << "查询成功！！！\n" << "\n查找ASL为:" << log(LSq.length) / log(2) << endl;
				else    cout << "\n很抱歉，该文章没有你想查询的单词(\n";
				break;
			case 3:Create_BST(T, LSq);
				cout << "请输入要删除的关键字:";
				cin >> word;
				if (DeleteBST(T, word) == false)
					cout << "\n删除失败！！ 二叉排序树中不存在单词 " << word << "！！\n";
				else
				{
					cout << "\n删除成功！！ 二叉排序树现在的中序遍历结果为：\n";
					InorderBST(T);
				}
			case 4:break;
			default:cout << "请输入正确的序号！！！\n";
			}break;
		case 5:break;
		default:cout << "请输入正确的序号！！！\n"; break;
		}
	} while (n != 5);
	//PrintList(LSq,LL);    输出文章所有单词（测试用）
}