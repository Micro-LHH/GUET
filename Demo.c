#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
void decide(FILE* TempCopy);
FILE* Device = NULL;
typedef struct Profile
{
	int priority;
	char Username[7];
	char Key[21];
	Creat* Link;
}Creat, * Edit;
int main(void)
{
	char Str[255];
	FILE* Temp = NULL;
	Device = fopen("Device.txt", "a+");
	printf("光电之光 0.1\n");
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
	char order[255], Doc[255];
	while (1)
	{
		fscanf(TempCopy, order);

		break;
	}
}
