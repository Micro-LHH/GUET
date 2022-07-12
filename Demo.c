#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
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
	char Model[16];
	time_t PurchaseTime;
	char StorageLocation[16];
	logup* Master;
	bool Situation;
	logup* Borrower;
	time_t BorrowTime;
	time_t ReturnTime;
}creat;


void logo(void);

void init(void);

void quit(void);

void get_order(void);

char* identify(int VaNum, ...);

logup* init_profile(bool License);

void get_profile(void);

void close(void);

void login(void);

bool check_profile(char* chr);

bool link(logup* Profile, int Ind);

logup* creat_profile(void);

logup* new_profile(char* Name);

void get_equipment(void);

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
	get_profile();
	get_equipment();
	if (Index == 0)
	{
		printf_s("用户数据丢失。。。\n");
		printf_s("是否进入初始化模式(y/n)\n$");
		if (strcmp(identify(2, YORN, "$"), "y") == 0)
		{
			printf_s("初始化中。。。\nSuperadministrator\n");
			Annount = AdmHLink = init_profile(true);
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
				Annount = init_profile(false);
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
						Annount = new_profile(ID);
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
					printf_s("密码错误,请重新输入\n%s",Command);
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


logup* init_profile(bool License)
{
	logup* Profile = creat_profile();
	char* Edit = NULL;
	if (Profile != NULL)
	{
		if (Index == 0)
			AdmHLink = Profile;
		printf_s("请输入用户名(<=10):");
		Edit = identify(3, ADMNAME, "请输入用户名(<=10):");
		check_profile(Edit);
		strcpy_s(Profile->AdmName, strlen(Edit) + 1, Edit);
		printf_s("请输入密码(<=12):");
		Edit = identify(3, PASSWORD, "请输入密码(<=12):");
		strcpy_s(Profile->Password, strlen(Edit) + 1, Edit);
		Profile->Permission = License;
		Profile->AdmLink = NULL;
		link(Profile, Index);
		return Profile;
	}
	return NULL;
}


void close(void)
{
	logup* fp = AdmHLink;
	char chr[36] = "";
	File = fopen("Annount.txt", "w");
	fclose(File);
	for (int i = 0; i < Index; i++ )
	{
		strcpy_s(chr, strlen(fp->AdmName)+1, fp->AdmName);
		for (int j = strlen(fp->AdmName); i < 10; i++)
			strcat(chr, " ");
		strcat(chr, "\t");
		strcat(chr, fp->Password);
		for (int j = strlen(fp->Password); j < 12; j++)
			strcat(chr, " ");
		strcat(chr, "\t");
		if (fp->Permission)
			strcat(chr, "true");
		else
			strcat(chr, "false");
		strcat(chr, "\n\n\n");
		File = fopen("Annount.txt", "a");
		fwrite(chr, 1, 36, File);
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
	else
	{
		while (true)
		{
			printf_s(">");
			if (strcmp(identify(2, COMMAND, ">"), "quit") == 0)
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


void get_profile(void)
{
	char Name[11];
	char Pass[13];
	char License[6];
	logup* fp = AdmHLink = creat_profile();
	File = fopen("Annount.txt", "r");
	while (true)
	{
		if (fp != NULL)
		{
			if (fscanf_s(File, "%s%s%s\n\n\n", Name, 11, Pass, 13, License, 6) == -1)
			{
				free(fp);
				break;
			}
			Name[10] = '\0';
			strcpy_s(fp->AdmName, strlen(Name)+1, Name);
			Pass[12] = '\0';
			strcpy_s(fp->Password, strlen(Pass)+1, Pass);
			License[5] = '\0';
			if (strcmp(License, "true") == 0)
				fp->Permission = true;
			else
				fp->Permission = false;
			fp->AdmLink = NULL;
			Index++;
			fp = creat_profile();
		}
	}
	fclose(File);
}


bool check_profile(char* chr)
{
	logup* fp = AdmHLink;
	for (int i = 0; i < Index; i++)
	{
		if (strcmp(fp->AdmName, chr) == 0)
		{
			Annount = fp;
			return true;
		}
		fp = fp->AdmLink;
	}
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


logup* creat_profile(void)
{
	logup* Profile = (logup*)malloc(sizeof(logup));
	return Profile;
}


logup* new_profile(char* Name)
{
	char Chr[255] = "";
	logup* Profile = creat_profile();
	strcpy_s(Profile->AdmName, strlen(Name) + 1, Name);
	printf_s("请输入密码(<=12):");
	strcpy(Chr, identify(3, PASSWORD, "请输入密码(<=12):"));
	strcpy_s(Profile->Password, strlen(Chr) + 1, Chr);
	if (Annount->Permission && strcmp(identify(2, YORN, "是否设为管理员(y/n)"), "y") == 0)
		Profile->Permission = true;
	else
		Profile->Permission = false;
	Profile->AdmLink = NULL;
	return Profile;
}


void get_equipment(void)
{
	char Name[11];
	char Mod[16];
	time_t Purchase;
	char Storage[16];
	char BorrowerName[11];
	bool Sit;
	char MasterName[11];
	time_t Borrow;
	time_t Return;
}