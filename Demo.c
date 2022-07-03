#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdarg.h>
#define Version "0.3"
void logo(void);

void init(void);

void quit(void);

char* get_order(void);

char* identify(int VaNum, ...);

logup* init_profile(void);

void edit_profile(int VaNum, ...);

void open(void);

void close(void);

void login(logup AnnCopy);

FILE* Command = NULL;
FILE* Annount = NULL;
FILE* Device = NULL;
FILE* Info = NULL;

enum identifier
{
	YORN = 0, LENGTH, ADMNAME, PASSWORD, LS, ADD, RM, LOG,
};

typedef struct profile     //Administrator Info
{
	char AdmName[11];
	char Password[13];
	bool Permission;
	struct Profile* Link;
}logup;

typedef struct equipment   //Device Info
{
	char DevName[11];
	char Model[13];
	
};


int main(void)
{
	logo();
	init();
	quit();
	return 0;
}


void logo(void)        //System Info
{
	printf("The Light Of Photoelectric %s\n", Version);
	printf("版权所有--GUET  保留所有权利\n\n");
	printf("代码详情  https://github.com/Micro-LHH/The-Light-Of-Photoelectric");
}


void init(void)
{
	open();
	logup* Ann = NULL;
	if (Annount == NULL)
	{
		printf_s("用户数据丢失。。。\n");
		printf_s("是否进入初始化模式(y/n)\n$");
		if (identify(1, YORN) == "y")
		{
			printf_s("初始化中。。。\nSuperadministrator\n");
			Annount = fopen("Device.txt", "a+");
			fputs("AdmName\tPassword\tPermission\n", Annount);
			Ann = init_profile();
			login(Ann);
		}
		else
		{
			;
		}
	}
	else
	{
		printf("Login/Logup\n");
		if (identify(1, LOG) == "Login")
			;
		else
			login(init_profile);
	}
	
}


char* get_order(void)
{
	char OrderLine[255] = "";
	gets_s(OrderLine, 255);
	if (strlen(OrderLine) == 255)
		OrderLine[254] = '\0';
	fputs(OrderLine, Command);
	return OrderLine;
}


char* identify(int VaNum, ...)
{
	char ID[255] = "";
	va_list VaList;
	va_start(VaList, VaNum);
	get_order();
	fscanf_s(Command, ID);
	switch (VaList[0])
	{
	case YORN:
		if ((ID == "y")||(ID == "Y"))
			return "y";
		else if ((ID == "n")||(ID == "N"))
			return "n";
		else
		{
			printf_s("请输入正确的命令(y/n)\n");
			get_order();
			return identify(1, VaList[0]);
		}


	case LENGTH:
		if (strlen(ID) <= VaList[1])
			return ID;
		else
		{
			printf_s("请输入正确长度的命令(<=%d)\n", VaList[1]);
			return identify(2, VaList[0], VaList[1]);
		}


	case LOG:
		if ((ID == "Login") || (ID == "login"))
			return "Login";
		else if ((ID == "Logup") || (ID == "logup"))
			return "Logup";
	default:
		printf("请输入正确的命令\n");
		return identify(VaNum,VaList[0]);
	}
}


logup* init_profile()
{
	logup Profile = { "", "", false, NULL };
	printf_s("请输入用户名(<=10):");
	edit_profile(2, Profile.AdmName, ADMNAME);
	scanf_s("密码(<=12):");
	edit_profile(2, Profile.Password, PASSWORD);
	return &Profile;
}


void edit_profile(int VaNum, ...)
{
	va_list VaList;
	va_start(VaList, VaNum);
	get_order();
	switch (VaList[1])
	{
	case ADMNAME:
		strcpy_s(VaList[0], 11, identify(2, LENGTH, 10));
		break;
	case PASSWORD:
		strcpy_s(VaList[0], 13, identify(2, LENGTH, 12));
		break;
	default:
		break;
	}
}


void open(void)
{
	Command = fopen("Command.txt", "w+");
	Annount = fopen("Annount.txt", "r");
	if (Annount != NULL)
	{

	}
}


void close(void)
{

}


void login(logup AnnCopy)
{

}


void quit(void)
{

}