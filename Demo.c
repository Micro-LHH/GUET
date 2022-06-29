#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#define Version "0.2"

int identify(void);

void decide(FILE* TempCopy);

void init(enum library Lib);

enum library
{
	Ann = 0,Dev
};


typedef struct info     //Device Info
{
	int Priority;
	char Username[7];
	char Key[21];
	struct Profile* Link;
}creat, * edit;


int main(void)
{
	char Str[255];
	FILE* Temp = NULL;
	FILE* Annount = NULL;
	FILE* Device = NULL;
	identify();
	Annount = fopen("Annount.txt", "r");
	if (Annount == NULL)
		Init(Ann);
	Device = fopen("Device.txt", "r");
	if (Device == NULL)
		init(Dev);
	while (1)
	{
		Temp = fopen("Temp.txt", "w+");
		printf("\n$");
		gets(Str);
		fputs(Str, Temp);
		decide(Temp);
		break;
	}
	fclose(Temp);
	fclose(Device);
	return 0;
}

void decide(FILE* TempCopy)
{
	char Order[255], Doc[255];
	while (1)
	{
		fscanf(TempCopy, Order);

		break;
	}
}

int	identify(void)        //System Info
{
	printf("The Light Of Photoelectric %s\n", Version);
	printf("版权所有--GUET  保留所有权利\n\n");
	printf("代码详情  https://github.com/Micro-LHH/The-Light-Of-Photoelectric");
	return 0;
}

void init(enum library Lib)
{
	char Judgement[255];
	if (Lib == Ann)
		printf("用户数据丢失。。。\n");
	scanf("是否进入初始化模式(y/n)\n$%s", Judgement);
	/*while (!(strlen(Judgement) == 1 && (Judgement == "y" || Judgement == "n")))
	{
		printf
	}*/
	
}