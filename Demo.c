#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
int Info(void);
void Decide(FILE* TempCopy);
FILE* Device = NULL;
typedef struct Profile
{
	int priority;
	char Username[7];
	char Key[21];
	struct Profile* Link;
}Creat, * Edit;
int main(void)
{
	char Str[255];
	FILE* Temp = NULL;
	FILE* Annount = NULL;
	Device = fopen("Device.txt", "a+");
	Info();
	while (1)
	{
		Temp = fopen("Temp.txt", "w+");
		printf("\n$");
		gets(Str);
		fputs(Str, Temp);
		Decide(Temp);
		break;
	}
	fclose(Temp);
	fclose(Device);
	return 0;
}

void Decide(FILE* TempCopy)
{
	char Order[255], Doc[255];
	while (1)
	{
		fscanf(TempCopy, Order);

		break;
	}
}

int Info(void)
{
	printf("The Light Of Photoelectric\n");
	printf("版权所有--GUET  保留所有权利\n\n");
	printf("代码详情  https://github.com/Micro-LHH/The-Light-Of-Photoelectric");
}