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

char* edit_profile(int VaNum, ...);

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
	printf("��Ȩ����--GUET  ��������Ȩ��\n\n");
	printf("��������  https://github.com/Micro-LHH/The-Light-Of-Photoelectric\n\n\n");
}


void init(void)
{
	logup* Ann = NULL;
	File = fopen("Annount", "r");
	if (File == NULL)
	{
		printf_s("�û����ݶ�ʧ������\n");
		printf_s("�Ƿ�����ʼ��ģʽ(y/n)\n$");
		if (strcmp(identify(2, YORN, "$"), "y") == 0)
		{
			printf_s("��ʼ���С�����\nSuperadministrator\n");
			AdmHLink = init_profile();
			AdmHLink->Permission = true;
			Ann = AdmHLink;
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
	va_list VaList;
	va_start(VaList, VaNum);
	enum identifer Identify = va_arg(VaList, enum identifier);
	int Length = va_arg(VaList, int);
	char* Command = va_arg(VaList, char*);
	char ID[255] = "";
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
				printf_s("������Ϸ�������(y/n)\n$");
			break;

		case LENGTH:
			if (strlen(ID) <= Length)
			{
				va_end(VaList);
				return ID;
			}
			else
				printf_s("������Ϸ����ȵ�����(<=%d)\n$", Length);
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
				printf_s("������ͷ�������(login/logup)\n");
			break;

		case COMMAND:
			if (strcmp(ID, "help") == 0)
				system("HELP");
			va_end(VaList);
			return ID;
		default:
			printf("������Ϸ�������\n$");
			break;
		}
	}
	
}


logup* init_profile()
{
	logup* Profile;
	Profile = (logup*)malloc((sizeof(logup)));
	printf_s("�������û���(<=10):");
	edit_profile(3, ADMNAME, Profile);
	printf_s("����������(<=12):");
	edit_profile(3, PASSWORD, Profile);
	if (Profile != NULL)
	{
		Profile->Permission = false;
		return Profile;
	}
	return NULL;
}


char* edit_profile(int VaNum, ...)
{
	va_list VaList;
	va_start(VaList, VaNum);
	enum identifer Identify = va_arg(VaList, enum identifier);
	char* Edit = NULL;
	switch (Identify)
	{
	case ADMNAME:
		Edit = identify(3, LENGTH, 10, "�������û���(<=10):");
		strcpy_s(va_arg(VaList, logup*)->AdmName, strlen(Edit) + 1, Edit);
		return Edit;
	case PASSWORD:
		Edit = identify(3, LENGTH, 12, "����������(<=12):");
		strcpy_s(va_arg(VaList, logup*)->Password, strlen(Edit) + 1, Edit);
		return Edit;
	default:
		break;
	}
	return NULL;
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
	fputs("AdmName   \tPassword    \tPermission\n", File);
	fclose(File);
	File = fopen("Annount.txt", "a");
	for (logup* fp = AdmHLink; fp != NULL; fp = fp->AdmLink)
	{
		fputs(File, fp->AdmName);
		for (i = strlen(fp->AdmName); i < 11; i++)
			fputs(File, " ");
		fputs(File, "\t");
		fputs(File, fp->Password);
		for (i = strlen(fp->Password); i < 13; i++)
			fputs(File, " ");
		fputs(File, "\t");
		if (fp->Permission)
			fputs(File, "true\n");
		else
			fputs(File, "false\n");
	}
	fclose(File);
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
	printf("�����˳�������\n");
	exit(0);
}

logup* read_profile(void)
{
	logup* Profile;
	Profile = (logup*)malloc((sizeof(logup)));
	char Information[255] = "";
	fgets(Information, 255, File);
	fgets(Information, 15, File);
	strcpy_s(Profile->AdmName, 11, Information);
	fgets(Information, 17, File);
	strcpy_s(Profile->Password, 13, Information);
	fgets(Information, 5, File);
	if (Information == "true")
		Profile->Permission = true;
	else
		Profile->Permission = false;
	return Profile;
}