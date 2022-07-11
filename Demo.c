#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<malloc.h>
#define Version "0.4"

FILE* File = NULL;

enum identifier
{
	NOTHING = 0, YORN , ADMNAME, PASSWORD, LOG, COMMAND, CHECK
};

typedef struct profile     //Administrator Info
{
	char AdmName[11];
	char Password[13];
	bool Permission;
	struct profile* AdmLink;
}logup;

int Index = 0;
logup* AdmHLink = NULL; 
logup* Annount = NULL;

typedef struct equipment   //Device Info
{
	char DevName[11];
	char Model[13];

	logup* borrower;
	logup* master;
}creat;


void logo(void);

void init(void);

void quit(void);

void get_order(void);

char* identify(int VaNum, ...);

logup* init_profile(void);

void edit_profile(int VaNum, ...);

void get_link(void);

void close(void);

void login(void);

logup* read_profile(logup* Profile);

bool check_profile(char* chr);

bool link(logup* Profile, int Ind);

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
	printf("代码详情  https://github.com/Micro-LHH/The-Light-Of-Photoelectric\n\n\n");
}


void init(void)
{
	AdmHLink = (logup*)malloc(sizeof(logup));
	get_link();
	if (Index == 0)
	{
		printf_s("用户数据丢失。。。\n");
		printf_s("是否进入初始化模式(y/n)\n$");
		if (strcmp(identify(2, YORN, "$"), "y") == 0)
		{
			printf_s("初始化中。。。\nSuperadministrator\n");
			AdmHLink = init_profile();
			AdmHLink->Permission = true;
			Annount = AdmHLink;
		}
		else	
			quit();
	}
	else
	{
		while (true)
		{
			printf("Login/Logup\n$");
			if (strcmp(identify(2, LOG, "$"), "login") == 0)
			{
				printf_s("用户名:");
				if (strcmp(identify(3, CHECK, "用户名:", ADMNAME), "y") == 0)
					break;
				else
				{
					system("cls");
					logo();
				}
			}
			else
			{
				Annount = init_profile();
				break;
			}
		}
	}
	system("cls");
	logo();
	login();
}


void get_order(void)
{
	char OrderLine[255] = "";
	File = fopen("Command.txt", "w");
	gets_s(OrderLine, 255);
	fwrite(OrderLine, 1, strlen(OrderLine), File);
	fclose(File);
}


char* identify(int VaNum, ...)
{
	va_list VaList;
	va_start(VaList, VaNum);
	enum identifier Identify = va_arg(VaList, enum identifier);
	char* Command = va_arg(VaList, char*);
	enum identifier Check = va_arg(VaList, enum identifier);
	char ID[255] = "";
	while (true)
	{
		while (true)
		{
			get_order();
			File = fopen("Command.txt", "r");
			if (fscanf_s(File, "%s", ID, 255) == -1)
				strcpy(ID, "");
			fclose(File);
			if (strcmp(ID, "") == 0)
				printf_s("%s", Command);
			else
				break;
		}
		switch (Identify)
		{
		case YORN:
			if ((strcmp(ID, "y") == 0) || (strcmp(ID, "Y") == 0))
			{
				va_end(VaList);
				return "y";
			}
			else if ((strcmp(ID, "n") == 0) || (strcmp(ID, "N") == 0))
			{
				va_end(VaList);
				return "n";
			}
			else
				printf_s("'%s'请输入合法的命令(y/n)\n%s",ID, Command);
			break;

		case LOG:
			if ((strcmp(ID, "Login") == 0) || (strcmp(ID, "login") == 0))
			{
				va_end(VaList);
				return "login";
			}
			else if ((strcmp(ID, "Logup") == 0) || (strcmp(ID, "logup") == 0))
			{
				va_end(VaList);
				return "logup";
			}
			else
				printf_s("'%s'请输入合法的命令(login/logup)\n%s", ID, Command);
			break;

		case ADMNAME:
			if (strlen(ID) <= 10)
			{
				va_end(VaList);
				return ID;
			}
			else
				printf_s("'%s'请输入合法长度的命令(<=10)\n$", ID);
			break;
			
		case PASSWORD:
			if (strlen(ID) <= 12)
			{
				va_end(VaList);
				return ID;
			}
			else
				printf_s("'%s'请输入合法长度的命令(<=12)\n$", ID);
			break;

		case COMMAND:
			if (strcmp(ID, "help") == 0)
				system("HELP");
			va_end(VaList);
			return ID;


		case CHECK:
			switch (Check)
			{
			case ADMNAME:
				if (check_profile(ID))
				{
					printf_s("密码:");
					Check = PASSWORD;
					Command = "密码:";
				}
				else
				{
					printf_s("查无此账号,是否创建(y/n)\n");
					if (strcmp(identify(2, YORN, "是否创建(y/n)\n"), "y") == 0)
					{
						Annount = init_profile();
						va_end(VaList);
						link(Annount, Index);
						return "y";
					}
					else
						return "n";
				}
				break;
			case PASSWORD:
				if (strcmp(ID, Annount->Password) == 0)
					return "y";
				else
				{
					printf_s("密码错误\n%s",Command);
					int err = 0;
					err++;
					if (err == 5)
						return "n";
				}
				break;
			default:
				break;
			}

		default:
			break;
		}
	}
	
}


logup* init_profile(void)
{
	logup* Profile;
	if (Index == 0)
		Profile = AdmHLink;
	else
		Profile = (logup*)malloc((sizeof(logup)));
	printf_s("请输入用户名(<=10):");
	edit_profile(2, ADMNAME, Profile);
	printf_s("请输入密码(<=12):");
	edit_profile(2, PASSWORD, Profile);
	Index++;
	if (Profile != NULL)
	{
		Profile->Permission = false;
		link(Profile, Index);
		return Profile;
	}
	return NULL;
}


void edit_profile(int VaNum, ...)
{
	va_list VaList;
	va_start(VaList, VaNum);
	enum identifer Identify = va_arg(VaList, enum identifier);
	char* Edit = NULL;
	switch (Identify)
	{
	case ADMNAME:
		Edit = identify(3, ADMNAME, "请输入用户名(<=10):");
		strcpy_s(va_arg(VaList, logup*)->AdmName, strlen(Edit) + 1, Edit);
		break;
	case PASSWORD:
		Edit = identify(3, PASSWORD, "请输入密码(<=12):");
		strcpy_s(va_arg(VaList, logup*)->Password, strlen(Edit) + 1, Edit);
		break;
	default:
		break;
	}
}


void get_link(void)
{
	logup* fp = NULL;
	File = fopen("Annount.txt", "r");
	fp = read_profile(AdmHLink);
	while (fp != AdmHLink)
		fp = read_profile(fp);
	fclose(File);
}


void close(void)
{
	logup* fp = AdmHLink;
	char License[6];
	File = fopen("Annount.txt", "w");
	fclose(File);
	for (int i = 0; i < Index; i++ )
	{
		if (fp->Permission)
			strcpy_s(License, 5, "true");
		else
			strcpy_s(License, 6, "false");
		File = fopen("Annount.txt", "a");
		fprintf_s(File, "%s\t%s\t%s\n\n\n", fp->AdmName, fp->Password, License);
		fclose(File);
		fp = fp->AdmLink;
	}
}


void login(void)
{
	bool License = Annount->Permission;
	printf_s("%s\n", Annount->AdmName);
	if (License)
	{
		while (true)
		{
			printf_s("$");
			if (strcmp(identify(2, COMMAND, "$"), "quit") == 0)
				break;
		}
	}
}


void quit(void)
{
	close();
	printf("正在退出。。。\n");
	exit(0);
}

logup* read_profile(logup* Profile)
{
	char Name[11];
	char Pass[13];
	char License[6];
	if (Index == 0)
		Profile = AdmHLink;
	else
		Profile = (logup*)malloc(sizeof(logup));
	if (Profile != NULL)
	{
		if (fscanf_s(File, "%s\t%s\t%s\n\n\n", Name, 11, Pass, 13, License, 6) == -1)
		{
			free(Profile);
			return AdmHLink;
		}
		Name[10] = '\0';
		strcpy_s(Profile->AdmName, 11, Name);
		Pass[12] = '\0';
		strcpy_s(Profile->Password, 13, Pass);
		License[5] = '\0';
		if (strcmp(License, "true") == 0)
			Profile->Permission = true;
		else
			Profile->Permission = false;
		Profile->AdmLink = NULL;
		Index++;
		return Profile->AdmLink;
	}
	return AdmHLink;
}


bool check_profile(char* chr)
{
	logup* fp = AdmHLink;
	do
	{
		if (strcmp(fp->AdmName, chr) == 0)
		{
			Annount = fp;
			return true;
		}
		fp = fp->AdmLink;
	} while (fp != NULL);
	return false;
}


bool link(logup* Profile, int Ind)
{
	logup* fp = AdmHLink;
	for (int i = 0; i < Ind-2; i++)
		fp = fp->AdmLink;
	if (Ind < Index)
		Profile->AdmLink = fp->AdmLink;
	fp->AdmLink = Profile;
	Index++;
}