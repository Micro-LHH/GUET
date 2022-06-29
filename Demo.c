#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#define Version "0.2"
void identify(void);

void judge(enum judment Ord);

void init(void);

void get_order(void);

FILE* Order = NULL;
FILE* Annount = NULL;
FILE* Device = NULL;

enum library
{
	Temp = 0,Dev
};

enum judment
{
	yORn = 0,
};

typedef struct info     //Device Info
{
	char Username[7];
	char Key[21];
	bool Adm;
	struct Profile* Link;
}creat, * edit;


int main(void)
{
	identify();
	init();
	
	fclose(Order);
	fclose(Annount);
	fclose(Device);
	return 0;
}


void judge(enum judment Jud)
{
	char Judge[255] = { 0 };
	fscanf_s(Order, Judge);
	switch (Jud)
	{
	case yORn:
		break;
	default:
		break;
	}
}


void identify(void)        //System Info
{
	printf("The Light Of Photoelectric %s\n", Version);
	printf("版权所有--GUET  保留所有权利\n\n");
	printf("代码详情  https://github.com/Micro-LHH/The-Light-Of-Photoelectric");
}

void init(void)
{
	Order = fopen("Temp.txt", "w+");
	Annount = fopen("Annount.txt", "r");
	if (Annount == NULL)
	{
		printf("用户数据丢失。。。\n");
		printf("是否进入初始化模式(y/n)\n$");
		get_order();
		judge(yORn);
	}
	
}


void get_order(void)
{
	char Command[255] = { 0 };
	gets(Command);
	if (strlen(Command) == 255)
		Command[254] = '\0';
	fputs(Command, Order);
}