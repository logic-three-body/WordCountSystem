#define _CRT_SECURE_NO_WARNINGS //put in first line
//https://blog.csdn.net/qq_34778691/article/details/68951235?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-4.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-4.channel_param
//文件名:英文单词词频统计.cpp
//作者:Vector_山水之间
//学号:1508010333
//版本:1.3
//完成日期:2017.3.31
//主要功能:在文本文件中输入一篇英文文章，统计文章中每个英文单词出现的次数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 27	                 //26个字母和其他字符
typedef struct Word              //字典树的结构体定义
{
	Word *next[MAX];	         //数组下标0-25代表小写字母，26代表其他字符
	int num;
};
typedef struct tlist             //结构体定义：单词和对应频率
{
	char word[200];
	int time;
};
struct tlist list[3000000];      //定义结构体数组
Word *root;
char str[200] = "";                //字符数据初始化为空
char tempword[1000];
int size = 0;                      //size为一篇文章中字符串的个数
void output();
void display();
int main();
void createWord(char *str)       //新建单词的函数(形参为字符串指针) 
{
	int len = strlen(str), id=0;   //strlen函数是测量字符串长度的函数
	Word *p = root, *q;
	for (int i = 0; i < len; i++)//遍历单词判断当前字符是否为字母或其他字符
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			id = str[i] - 'a';   //字符相减为对应ASCII值相减
		if (str[i] >= 'A' && str[i] <= 'Z')
			id = str[i] - 'A';
		if (str[i] == '\'')
			id = 26;          //当读取到的字符为26个英文字母除外的其他字符，id置为26
		if (p->next[id] == NULL)  //若已到达链表结尾，开辟新的结构体存入字母
		{
			q = (Word *)malloc(sizeof(Word));//初始化
			q->num = 0;
			for (int j = 0; j < MAX; j++)
			{				
				q->next[j] = NULL;
			}
			p->next[id] = q;
			p = p->next[id];
		}
		else                       //若未到达链表结尾，指针指向下一个
		{
			p = p->next[id];
		}
	}
	p->num++;                   //重复单词数加1
}
void readWord(Word *p, int len)     //读单词的函数    
{
	int i;
	for (i = 0; i < 27; i++)
	{
		if (p->next[i] != NULL)
		{
			if (i == 26)
			{
				str[len + 1] = '\0';
				str[len] = '\'';
				len++;
			}
			else
			{
				str[len] = 'a' + i;
				len++;
			}
			readWord((Word*)p->next[i], len);
			len--;
		}
	}
	if (p->num != 0)
	{
		str[len] = '\0';
		strcpy(list[size].word, str);    //如果遇到单词结束标志，将str存入list[size].word
		list[size].time = p->num;
		size++;
	}
}
void output()                           //输出函数
{
	int i;
	FILE *fpx;
	fpx = fopen("result.txt", "w");        //将统计结果写入到result.txt文件中
	for (i = 1; i < size; i++)
		fprintf(fpx, "%s   %d\n", list[i].word, list[i].time);
	for (i = 1; i < size; i++)                //输出统计结果
		printf("%s   %d\n", list[i].word, list[i].time);

	fclose(fpx);
}
void menu()                             //菜单函数
{
	int choose;
	printf("----------------英文单词词频统计-----------------------------\n\n");
	printf("使用前请认真阅读使用帮助!\n\n");
	printf("使用帮助:\n");
	printf("请在文本文档data.txt中输入英文单词或者英文文章\n");
	printf("若要修改需要统计的英文单词或者英文文章，请在data.txt文件中进行\n\n");
	printf("若以确保在data.txt文档中输入数据,请按数字键1查看词频统计结果!\n");
	scanf("%d", &choose);
	if (choose == 1)
		display();
}
void display() //展示统计结果函数
{
	int i, j;
	char letter;
	int len = 0; //初始时字符串长度为0 
	root = (Word*)malloc(sizeof(Word)); //为结构体指针root动态开辟存储空间 
	for (i = 0; i < 27; i++) 
		root->next[i] = NULL; 
	FILE *fp; fp = fopen("data.txt", "r"); //文件打开方式为只读 
	if (!fp) {
		printf("\n打开文件data.txt失败!");
	}
	while ((letter = fgetc(fp)) != EOF) //从txt文档中读入 读入失败时，函数值为EOF(-1) fgetc为从文件中读取一个字符 
	{
		if ((letter >= 'A'&&letter <= 'Z') || (letter >= 'a'&&letter <= 'z') || (letter == '\''&&len != 0))
		{
			tempword[len] = letter; 
			len++;
		}
		else
		{
			tempword[len] = '\0';//遇到标点，切分这段单词
			createWord(tempword); 
			len = 0;
		}
	}
	tempword[len] = '\0';
	createWord(tempword);
	//创建对文本最后一个英文字符串的读取
	len = 0;
	fclose(fp); 
	readWord(root, 0);
	struct tlist temp;
	for (i = 0; i < size - 1; i++) //比较频率大小冒泡排序
		for (j = i + 1; j < size; j++)
		{
			if (list[i].time > list[j].time ||
				(list[i].time == list[j].time&&
					strcmp(list[i].word, list[j].word) > 0))
			{
				temp.time = list[i].time;
				list[i].time = list[j].time;
				list[j].time = temp.time;
				strcpy(temp.word, list[i].word);
				strcpy(list[i].word, list[j].word);
				strcpy(list[j].word, temp.word);
			}
		}

	printf("英文单词及词频统计结果如下:\n");
	output();
	printf("共%d单词\n",size);
	int sum=0;
	for (int i = 1; i < size; i++)
	{
		sum += list[i].time;
	}
	printf("总数是%d\n", sum);
}
int main() //主函数 
{
	menu(); 
	system("pause");
	return 0;
}