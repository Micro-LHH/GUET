#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdarg.h>
#define Version "0.3"

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

logup* AdmHLink;

//typedef struct equipment   //Device Info
//{
//	char DevName[11];
//	char Model[13];
//
//};


void logo(void);

void init(void);

void quit(void);

char* get_order(void);

char* identify(int VaNum, ...);

logup* init_profile(void);

void edit_profile(int VaNum, ...);

void get_link(void);

void close(void);

void login(logup* AnnCopy);

logup* read_profile(void);

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
	File = fopen("Annount", "r");
	if (File == NULL)
	{
		printf_s("用户数据丢失。。。\n");
		printf_s("是否进入初始化模式(y/n)\n$");
		if (strcmp(identify(1, YORN), "y") == 0)
		{
			printf_s("初始化中。。。\nSuperadministrator\n");
			Ann = init_profile();
			login(Ann);
		}
		else
		{
			printf("正在退出。。。\n");
		}
	}
	else
	{
		get_link();
		printf("Login/Logup\n");
		if (identify(1, LOG) == "login")
			;
		else
			login(init_profile);
	}
	
}


char* get_order(void)
{
	char OrderLine[255] = "";
	File = fopen("Command.txt", "w");
	gets_s(OrderLine, 255);
	fwrite(OrderLine, 1, strlen(OrderLine), File);
	fclose(File);
	return OrderLine;
}


char* identify(int VaNum, ...)
{
	char ID[255] = "";
	va_list VaList;
	va_start(VaList, VaNum);
	get_order();
	File = fopen("Command.txt", "r");
	fscanf_s(File, "%s", ID, 255);
	fclose(File);
	switch (va_arg(VaList, enum identifier))
	{
	case YORN:
		if ((strcmp(ID, "y") == 0) || (strcmp(ID, "Y") == 0))
			return "y"; 
		else if ((strcmp(ID, "n") == 0)||(strcmp(ID, "N") == 0))
			return "n";
		else
		{
			printf_s("请输入正确的命令(y/n)\n");
			return identify(1, VaList[0]);
		}


	case LENGTH:
		if (strlen(ID) <= va_arg(VaList, int))
			return ID;
		else
		{
			printf_s("请输入正确长度的命令(<=%d)\n", va_arg(VaList, int));
			return identify(2, va_arg(VaList, enum identifier), va_arg(VaList, int));
		}


	case LOG:
		if ((strcmp(ID, "Login") == 0) || (strcmp(ID, "login") == 0))
			return "login";
		else if ((strcmp(ID, "Logup") == 0) || (strcmp(ID, "logup") == 0))
			return "logup";


	case COMMAND:
	default:
		printf("请输入正确的命令\n");
		return identify(VaNum, va_arg(VaList, enum identifier));
	}
}


logup* init_profile()
{
	logup Profile = { "", "", false, NULL };
	printf_s("请输入用户名(<=10):");
	edit_profile(2, ADMNAME, &Profile);
	printf_s("密码(<=12):");
	edit_profile(2, PASSWORD, &Profile);
	return &Profile;
}


void edit_profile(int VaNum, ...)
{
	char* Edit = NULL;
	va_list VaList;
	va_start(VaList, VaNum);
	switch (va_arg(VaList, enum identifier))
	{
	case ADMNAME:
		Edit = identify(2, LENGTH, 10);
		strcpy_s(va_arg(VaList, logup*), strlen(Edit) + 1, Edit);
		break;
	case PASSWORD:
		Edit = identify(2, LENGTH, 12);
		strcpy_s(va_arg(VaList, logup*), strlen(Edit) + 1, Edit);
		break;
	default:
		break;
	}
	va_end(VaList);
}


void get_link(void)
{
	logup* fp = NULL;
	AdmHLink->AdmLink = fp = read_profile();
	while (fp != NULL)
		fp = fp->AdmLink = read_profile();
	fclose(File);
}


void close(void)
{
	int i = 0;
	
	File = fopen("Annount.txt", "w");
	fputs("AdmName\tPassword\tPermission\n", File);
	for (logup* fp = AdmHLink; fp != NULL; fp = fp->AdmLink)
	{
		for (i = strlen(fp->AdmName); i < 9; i++)
			fp->AdmName[i] = ' ';
		fp->AdmName[10] = '\0';
		fwrite(fp->AdmName, 1, 11, File);
		for (i = strlen(fp->Password); i < 11; i++)
			fp->Password[i] = ' ';
		fp->Password[12] = '\0';
		fwrite(fp->Password, 1, 13, File);
		if (fp->Permission)
			fwrite("true", 1, 5, File);
		else
			fwrite("false", 1, 6, File);
	}
	fclose(File);
}


void login(logup* AnnCopy)
{
	bool License = AnnCopy->Permission;
	do
	{
		;
	} while (identify(COMMAND) != "quit");
}


void quit(void)
{

}

logup* read_profile(void)
{
	logup Pro = { "", "", false, NULL };
	char Information[255] = "";
	fgets(Information, 255, File);
	fgets(Information, 15, File);
	strcpy_s(&Pro.AdmName, 11, Information);
	fgets(Information, 17, File);
	strcpy_s(&Pro.Password, 13, Information);
	fgets(Information, 5, File);
	if (Information == "true")
		Pro.Permission = true;
	return &Pro;
}