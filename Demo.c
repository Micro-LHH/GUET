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
	NOTHING = 0, YORN , LENGTH, ADMNAME, PASSWORD, LOG, COMMAND
};

typedef struct profile     //Administrator Info
{
	char AdmName[11];
	char Password[13];
	bool Permission;
	struct profile* AdmLink;
}logup;

int index = 0;
logup* AdmHLink = NULL;

//typedef struct equipment   //Device Info
//{
//	char DevName[11];
//	char Model[13];
//
//};


void logo(void);

void init(void);

void quit(void);

void get_order(void);

char* identify(int VaNum, ...);

logup* init_profile(void);

void edit_profile(int VaNum, ...);

void get_link(void);

void close(void);

void login(logup* AnnCopy);

logup* read_profile(logup* Profile);

void write(char* fp, char* chr);

int main(void)
{
	logo();
	init();
	close();
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
	logup* Ann = NULL;
	AdmHLink = (logup*)malloc(sizeof(logup));
	File = fopen("Annount.txt", "r");
	if (File == NULL)
	{
		printf_s("用户数据丢失。。。\n");
		printf_s("是否进入初始化模式(y/n)\n$");
		if (strcmp(identify(2, YORN, "$"), "y") == 0)
		{
			printf_s("初始化中。。。\nSuperadministrator\n");
			AdmHLink = init_profile();
			AdmHLink->Permission = true;
			Ann = AdmHLink;
			index = 1;
		}
		else	
			quit();
	}
	else
	{
		get_link();
		printf("Login/Logup\n$");
		if (identify(2, LOG, "$") == "login")
			;
		else
			Ann = init_profile();
	}
	system("cls");
	logo();
	login(Ann);
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
	enum identifer Identify = va_arg(VaList, enum identifier);
	int Length = va_arg(VaList, int);
	char* Command = va_arg(VaList, char*);
	static char ID[255] = "";
	while (true)
	{
		while (true)
		{
			get_order();
			File = fopen("Command.txt", "r");
			fscanf_s(File, "%s", ID, 255);
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
				printf_s("请输入合法的命令(y/n)\n$");
			break;

		case LENGTH:
			if (strlen(ID) <= Length)
			{
				va_end(VaList);
				return ID;
			}
			else
				printf_s("请输入合法长度的命令(<=%d)\n$", Length);
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
				printf_s("请输入和法的命令(login/logup)\n");
			break;

		case COMMAND:
			if (strcmp(ID, "help") == 0)
				system("HELP");
			va_end(VaList);
			return ID;
		default:
			printf("请输入合法的命令\n$");
			break;
		}
	}
	
}


logup* init_profile()
{
	logup* Profile;
	Profile = (logup*)malloc((sizeof(logup)));
	printf_s("请输入用户名(<=10):");
	edit_profile(3, ADMNAME, Profile);
	printf_s("请输入密码(<=12):");
	edit_profile(3, PASSWORD, Profile);
	if (Profile != NULL)
	{
		Profile->Permission = false;
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
		Edit = identify(3, LENGTH, 10, "请输入用户名(<=10):");
		strcpy_s(va_arg(VaList, logup*)->AdmName, strlen(Edit) + 1, Edit);
		break;
	case PASSWORD:
		Edit = identify(3, LENGTH, 12, "请输入密码(<=12):");
		strcpy_s(va_arg(VaList, logup*)->Password, strlen(Edit) + 1, Edit);
		break;
	default:
		break;
	}
}


void get_link(void)
{
	logup* fp = NULL;
	char* Null = "";
	fgets(Null, 255, File);
	fp = read_profile(AdmHLink);
	do
	{
		fp = read_profile(fp);
	} while (fp == AdmHLink);
	fclose(File);
}


void close(void)
{
	int i = 0;
	logup* fp = AdmHLink;
	File = fopen("Annount.txt", "w");
	fputs("AdmName\tPassword\tPermission\n", File);
	fclose(File);
	for (int i = 0; i < index; i++ )
	{
		write("Annount.txt", fp->AdmName);
		write("Annount.txt", "\t");
		write("Annount.txt", fp->Password);
		write("Annount.txt", "\t");
		if (fp->Permission)
			write("Annount.txt", "true");
		else
			write("Annount.txt", "false");
		write("Annount.txt", "\n\n\n");
		fp = fp->AdmLink;
	}
}


void login(logup* AnnCopy)
{
	bool License = AnnCopy->Permission;
	/*for (int i = 0; i <= strlen(AnnCopy->AdmName); i++)
	{
		printf_s("%c", AnnCopy->AdmName[i]);
	}*/
	printf_s("%s\n", AnnCopy->AdmName);
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
	printf("正在退出。。。\n");
	exit(0);
}

logup* read_profile(logup* Profile)
{
	char Information[255] = "";
	fscanf_s(File, "%s", Information, 1);
	if (strcmp(Information, "") == 0)
		return AdmHLink;
	strcpy_s(Profile->AdmName, 11, Information);
	fscanf_s(File, "%s", Information, 1);
	strcpy_s(Profile->Password, 13, Information);
	fscanf_s(File, "%s", Information, 1);
	if (Information == "true")
		Profile->Permission = true;
	else
		Profile->Permission = false;
	index++;
	Profile->AdmLink = NULL;
	return Profile->AdmLink;
}


void write(char* fp, char* chr)
{
	FILE* File = fopen(fp, "a");
	fprintf_s(File, "%s", chr);
	fclose(File);
}