#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include<io.h>
#define Version "0.4"

FILE* File = NULL;

enum identifier
{
	NOTHING = 0, YORN , NAME, PASSWORD, LOG, CHECK, REMARK, ADD, CLEAR, QUIT, SU, LS, SUDO, FETCH, COMMIT, DISABLE, DEL, HELP, ALL
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
	char EquName[11];
	char Remark[17];
	char PurchaseTime[17];
	char StorageLocation[17];
	logup* Master;
	bool Situation;
	struct equipment* EquLink;
}make;

int Count = 0;
make* EquHLink = NULL;
make* Device = NULL;

typedef struct info
{
	make* Equipment;
	logup* Borrower;
	time_t BorrowTime;
	time_t ReturnTime;
	struct info* InfLink;
}generate;

int History = 0;
generate* InfHLink = NULL;
generate* Record;

void logo(void);

void init(void);

void quit(void);

void get_order(void);

char* identify(int VaNum, ...);

void login(logup* Copy);

void help(enum identifier Identify);


logup* creat_profile(void);

void pull_profile(void);

logup* init_profile(bool License);

logup* new_profile(char* Name);

void push_profile(void);

bool check_profile(char* chr);

void merege_profile(logup* Profile, int Ind);


make* creat_equipment(void);

void pull_equipment(void);

make* init_equipment(void);

make* new_equipment(char* Name);

void push_equipment(void);

bool check_equipment(char* chr);

void merege_equipment(make* Equipment, int Cou);


generate* creat_info(void);

void pull_info(void);

generate* init_info(make* Equ);

void push_info(void);

bool check_info();

void merege_info(generate* Info, int His);


void ls_a(void);

void ls_p();

void ls_v();

bool del_profile(void);

void del_equipment(void);

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
	printf("��Ȩ����--GUET  ��������Ȩ��\n\n");
	printf("��������  https://github.com/Micro-LHH/The-Light-Of-Photoelectric\n\n\n");
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
			if (fscanf_s(File, "%s", ID, 255) != -1)
			{
				fclose(File);
				break;
			}
			strcpy(ID, "");
			printf_s("%s", Command);
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
				printf_s("'%s'������Ϸ�������(y/n)\n%s", ID, Command);
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
				printf_s("'%s'������Ϸ�������(login/logup)\n$", ID);
			break;

		case NAME:
			if (strlen(ID) <= 10)
			{
				va_end(VaList);
				return ID;
			}
			else
				printf_s("'%s'������Ϸ����ȵ�����(< = 10)\n$", ID);
			break;

		case PASSWORD:
			if (strlen(ID) <= 12)
			{
				va_end(VaList);
				return ID;
			}
			else
				printf_s("'%s'������Ϸ����ȵ�����(< = 12)\n$", ID);
			break;

		case REMARK:
			if (strlen(ID) <= 16)
			{
				va_end(VaList);
				return ID;
			}
			else
				printf_s("'%s'������Ϸ����ȵ�����(< = 16)\n$", ID);
			break;

		case CHECK:
			switch (Check)
			{
			case NAME:
				if (check_profile(ID))
				{
					printf_s("����:");
					Check = PASSWORD;
					Command = "����:";
				}
				else
				{
					printf_s("���޴��˺�,�Ƿ񴴽�(y/n)\n$");
					if (strcmp(identify(2, YORN, "$"), "y") == 0)
					{
						Annount = new_profile(ID);
						va_end(VaList);
						merege_profile(Annount, Index);
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
					printf_s("�������,����������\n%s", Command);
					int err = 0;
					err++;
					if (err == 5)
					{
						printf_s("��δ���,�˳�\n");
						return "n";
					}
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


void init(void)
{
	pull_profile();
	pull_equipment();
	pull_info();
	if (Index == 0)
	{
		printf_s("�û����ݶ�ʧ������\n");
		printf_s("�Ƿ�����ʼ��ģʽ(y/n)\n$");
		if (strcmp(identify(2, YORN, "$"), "y") == 0)
		{
			printf_s("��ʼ���С�����\nSuperadministrator\n");
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
				printf_s("�û���:");
				if (strcmp(identify(3, CHECK, "�û���:", NAME), "y") == 0)
					break;
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
	login(Annount);
}


void login(logup* Copy)
{
	bool License = Annount->Permission;
	char* Prompt;
	char Command[255] = "";
	char Document[255] = "";
	char Option[255] = "";
	time_t Tim = 0;
	if (License)
		Prompt = "$";
	else
		Prompt = ">>>";
	system("cls");
	printf_s("%s\n", Annount->AdmName);
	while (true)
	{
		while (true)
		{
			strcpy(Command, "");
			strcpy(Option, "");
			strcpy(Document, "");
			printf_s("%s", Prompt);
			get_order();
			File = fopen("Command.txt", "r");
			if (fscanf_s(File, "%s %s %s", Command, 255, Option, 255, Document, 255) != -1)
			{
				fclose(File);
				break;
			}
			fclose(File);
		}
		if ((Option[0] != '-') && (strcmp(Document, "") == 0))
		{
			strcpy(Document, Option);
			strcpy(Option, "");
		}
		if (strcmp(Command, "quit") == 0)
		{
			if ((strcmp(Document, "") == 0) && (strcmp(Option, "") == 0))
				break;
			printf_s("�Ƿ�ʹ������'quit'\n");
			help(QUIT);
		}
		else if (strcmp(Command, "clear") == 0)
		{
			if ((strcmp(Option, "") == 0) && (strcmp(Document, "") == 0))
				system("cls");
			else
			{
				printf_s("�Ƿ�ʹ������'clear'\n");
				help(CLEAR);
			}
		}
		else if (strcmp(Command, "add") == 0)
		{
			if ((strcmp(Option, "-a") == 0) || (strcmp(Option, "-A") == 0) || (strcmp(Option, "") == 0))
			{
				if (strcmp(Document, "") != 0)
				{
					if (strlen(Document) <= 10)
						new_profile(Document);
					else
					{
						printf_s("'%s'�Ƿ�����\n", Document);
						help(NAME);
					}
				}
				else
					init_profile(true);
			}
			else if ((strcmp(Option, "-p") == 0) || (strcmp(Option, "-P") == 0))
			{
				if (strcmp(Document, "") != 0)
				{
					if (strlen(Document) <= 10)
						new_profile(Document);
					else
					{
						printf_s("'%s'�Ƿ�����\n", Document);
						help(NAME);
					}
				}
				else
					init_profile(false);
			}
			else if ((strcmp(Option, "-v") == 0) || (strcmp(Option, "-V") == 0))
			{
				if (strcmp(Document, "") != 0)
				{
					if (strlen(Document) <= 10)
						new_equipment(Document);
					else
					{
						printf_s("'%s'�Ƿ�����\n", Document);
						help(NAME);
					}
				}
				else
					init_equipment();
			}
			else
				help(ADD);
		}
		else if (strcmp(Command, "su") == 0)
		{
			if (strcmp(Option, "") == 0)
			{
				if (strcmp(Document, "") != 0)
				{
					if (strlen(Document) <= 10)
					{
						if (check_profile(Document))
						{
							while (true)
							{
								while (true)
								{
									printf_s("����:");
									strcpy(Command, "");
									get_order();
									File = fopen("Command.txt", "r");
									if (fscanf_s(File, "%s", Command, 255) != -1)
									{
										fclose(File);
										break;
									}
									fclose(File);
								}
								if (strcmp(Document, Annount) == 0)
								{
									login(Annount);
									quit();
								}
								else
								{
									printf_s("�������\n");
									int err = 0;
									err++;
									if (err % 5 == 0)
									{
										printf("��δ���,�˳�\n");
										break;
									}
								}
							}
						}
						else
						{
							printf_s("���޴��˺�,�Ƿ񴴽�(y/n)\n%s", Prompt);
							if (strcmp(identify(2, YORN, Prompt), "y") == 0)
							{
								login(new_profile(Document));
								break;
							}
						}
					}
					else
					{
						printf_s("'%s'�Ƿ�����\n", Document);
						help(NAME);
					}
				}
				else
					help(SU);
			}
			else
				help(SU);
		}
		else if (strcmp(Command, "sudo") == 0)
		{
			if (strcmp(Option, "") == 0)
			{
				if (strcmp(Document, "") != 0)
				{
					if (Copy->Permission)
					{
						if (strlen(Document) <= 10)
						{
							if (check_profile(Document))
							{
								Annount->Permission = true;
								printf_s("����Ա��Ȩ�ɹ�\n");
							}
							else
							{
								printf_s("���޴��˺�,�Ƿ񴴽�(y/n)\n%s", Prompt);
								if (strcmp(identify(2, YORN, Prompt), "y") == 0)
									new_profile(Document)->Permission = true;
							}
						}
						else
						{
							printf_s("'%s'�Ƿ�����\n", Document);
							help(NAME);
						}

					}
					else
						printf_s("�ǹ���ԱȨ��,�Ƿ�ʹ��sudo����");
				}
				else
					help(SUDO);
			}
			else
				help(SUDO);
		}
		else if (strcmp(Command, "ls") == 0)
		{
			if (strcmp(Document, "") == 0)
			{
				if ((strcmp(Option, "-a") == 0) || (strcmp(Option, "-A") == 0))
					ls_a();
				else if ((strcmp(Option, "-p") == 0) || (strcmp(Option, "-P") == 0))
					ls_p();
				else if ((strcmp(Option, "-v") == 0) || (strcmp(Option, "-V") == 0))
					ls_v();
				else
					help(LS);
			}
			else
				help(LS);
		}
		else if (strcmp(Command, "fetch") == 0)
		{
			if ((strcmp(Option, "\0") == 0) && (strcmp(Document, "") != 0))
			{
				if (check_equipment(Document))
				{
					init_info(Device);
					printf_s("'%s'���óɹ�\n", Document);
				}
				else
				{
					if (Copy->Permission)
					{
						printf_s("���豸������,�Ƿ񴴽����豸(y/n)\n$");
						if (strcmp(identify(2, YORN, "�Ƿ񴴽����豸(y/n)\n$"), "y") == 0)
							init_info(init_equipment(Document));
					}		
				}
			}
			else
				help(FETCH);
		}
		else if (strcmp(Command, "commit") == 0)
		{
			if ((strcmp(Option, "\0") == 0) && (strcmp(Document, "") != 0))
			{
				if (check_equipment(Document))
				{
					if (check_info(Device))
					{
						Record->ReturnTime = time(Tim);
						printf_s("'%s'�黹�ɹ�\n", Document);
					}
					else
					{
						printf_s("δ���ô��豸,�Ƿ����(y/n)\n%s", Prompt);
						if (strcmp(identify(2, YORN, Prompt), "y") == 0)
							init_info(Device);
					}
				}
				else
					printf_s("�����ڴ��豸\n");
			}
			else
				help(COMMIT);
		}
		else if (strcmp(Command, "help") == 0)
		{
			if ((strcmp(Option, "\0") == 0) && (strcmp(Document, "") == 0))
				help(ALL);
			else
				help(HELP);
		}
		else if (strcmp(Command, "disable") == 0)
		{
			if (strcmp(Option, "\0") == 0)
			{
				if (strcmp(Document,"") != 0)
				{
					if (check_equipment(Document))
					{
						if (!check_info())
						{
							Device->Situation = false;
							printf_s("'%s'���óɹ�\n", Document);
						}
						else
							printf_s("'%s'�ѱ����,����ʧ��\n", Document);
					}
					else
						printf_s("'%s'�޴��豸,����ʧ��\n", Document);
				}
				else
					help(DISABLE);
			}
			else
				help(DISABLE);
		}
		else if (strcmp(Command, "del") == 0)
		{
			if (strcmp(Option, "-a") == 0)
			{
				if (check_profile(Document))
				{
					if (del_profile())
						printf_s("'%s'ɾ���ɹ�\n", Document);
					else
						printf_s("'%s'��δ���豸,ɾ��ʧ��\n", Document);
				}
				else
					printf_s("'%s'�޴��û�,ɾ��ʧ��\n", Document);
			}
			else if (strcmp(Option, "-a") == 0)
			{
				if (check_equipment(Document))
				{
					if (check_info())
						printf_s("'%s'�ѱ����,ɾ��ʧ��\n", Document);
					else
					{
						del_equipment();
						printf_s("'%s'ɾ���ɹ�\n", Document);
					}
				}
				else
					printf_s("'%s'�޴��豸,ɾ��ʧ��\n", Document);
			}
			else
				help(DEL);
		}
		else
		{
			printf_s("'%s", Command);
			if (strcmp(Option, "") != 0)
				printf_s(" %s", Option);
			if (strcmp(Document, "") != 0)
				printf_s(" %s'", Option);
			else
				printf_s("'");
			printf_s("�Ƿ�����,����Գ�������help�����ȡ����������Ϣ\n");
		}
		Annount = Copy;
	}
}


void quit(void)
{
	push_profile();
	push_equipment();
	push_info();
	printf("�����˳�������\n");
	exit(0);
}


void help(enum identifier Identify)
{
	switch (Identify)
	{
	case ADD:printf_s("add -option [file] ����Գ�������: add -a [file] ����һ������Ա(ֻ�й���Ա���Խ��д˲���) -p [file] ����һ��ͨ�û� -v [file] ����һ���豸(ֻ�й���Ա���Խ��д˲���)\n"); break;
	case CLEAR:printf_s("clear ����Գ�������: clear ����\n"); break;
	case QUIT:printf_s("quit ����Գ�������: quit �˳�ϵͳ\n"); break;
	case NAME:printf_s("�������ȷǷ�(< = 10)\n"); break;
	case REMARK:printf_s("�������ȷǷ�(< = 16)\n"); break;
	case SU:printf_s("su file ����Գ�������: su [file] �л��û�\n"); break;
	case SUDO:printf_s("sudo file ����Գ�������: sudo file ������û�����ԱȨ��(ֻ�й���Ա���Խ��д˲���)\n"); break;
	case LS:printf_s("ls -option ����Գ�������: ls -a �鿴�����豸 ls -p �鿴������������豸 ls -v �鿴���������õ��豸\n"); break;
	case HELP:printf_s("help ����Գ�������: help �鿴��������\n"); break;
	case FETCH:printf_s("fetch file ����Գ�������: fetch file ��������豸\n"); break;
	case COMMIT:printf_s("commit file ����Գ�������: commit file ����黹�豸\n"); break;
	case DISABLE:printf_s("disable file ����Գ�������: disable file �����豸(ֻ�й���Ա���Խ��д˲���)\n"); break;
	case DEL:printf_s("del -option file ����Գ�������: del -a file ɾ���û�(ֻ�й���Ա���Խ��д˲���) del -v file ɾ���豸(ֻ�й���Ա���Խ��д˲���)\n"); break;
	case ALL:
		printf_s("add -option [file] ����Գ�������: add -a [file] ����һ������Ա(ֻ�й���Ա���Խ��д˲���) -p [file] ����һ��ͨ�û� -v [file] ����һ���豸(ֻ�й���Ա���Խ��д˲���)\n");
		printf_s("clear ����Գ�������: clear ����\n"); 
		printf_s("quit ����Գ�������: quit �˳�ϵͳ\n");
		printf_s("su file ����Գ�������: su [file] �л��û�\n");
		printf_s("sudo file ����Գ�������: sudo file ������û�����ԱȨ��(ֻ�й���Ա���Խ��д˲���)\n");
		printf_s("ls -option ����Գ�������: ls -a �鿴�����豸 ls -p �鿴������������豸 ls -v �鿴���������õ��豸\n");
		printf_s("help ����Գ�������: help �鿴��������\n"); 
		printf_s("fetch file ����Գ�������: fetch file �����������\n");
		printf_s("commit file ����Գ�������: commit file ����黹�豸\n");
		printf_s("disable file ����Գ�������: disable file �����豸(ֻ�й���Ա���Խ��д˲���)\n");
		printf_s("del -option file ����Գ�������: del -a file ɾ���û�(ֻ�й���Ա���Խ��д˲���) del -v file ɾ���豸(ֻ�й���Ա���Խ��д˲���)\n"); break;
	default:
		break;
	}
}


logup* creat_profile(void)
{
	logup* Profile = (logup*)malloc(sizeof(logup));
	return Profile;
}


void pull_profile(void)
{
	char Name[11];
	char Pass[13];
	char License[6];
	logup* fp = AdmHLink = creat_profile();
	File = fopen("Annount.txt", "r");
	if (File != NULL)
	{
		while (true)
		{
			if (fscanf_s(File, "%s\t%s\t%s\n", Name, 11, Pass, 13, License, 6) != 3)
			{
				free(fp);
				break;
			}
			Name[10] = '\0';
			strcpy_s(fp->AdmName, strlen(Name) + 1, Name);
			Pass[12] = '\0';
			strcpy_s(fp->Password, strlen(Pass) + 1, Pass);
			License[5] = '\0';
			if (strcmp(License, "true") == 0)
				fp->Permission = true;
			else
				fp->Permission = false;
			fp->AdmLink = NULL;
			merege_profile(fp, Index);
			fp = creat_profile();
		}
		fclose(File);
	}
}


logup* init_profile(bool License)
{
	logup* Profile = creat_profile();
	char* Edit = NULL;
	while(true)
	{
		if (Index == 0)
			AdmHLink = Profile;
		printf_s("�������û���(< = 10):");
		Edit = identify(2, NAME, "�������û���(< = 10):");
		if (!check_profile(Edit))
		{
			strcpy_s(Profile->AdmName, strlen(Edit) + 1, Edit);
			printf_s("����������(< = 12):");
			Edit = identify(2, PASSWORD, "����������(< = 12):");
			strcpy_s(Profile->Password, strlen(Edit) + 1, Edit);
			Profile->Permission = License;
			Profile->AdmLink = NULL;
			merege_profile(Profile, Index);
			return Profile;
		}
		else
			printf_s("������ͻ\n");
	}
	return NULL;
}


logup* new_profile(char* Name)
{
	char Chr[255] = "";
	logup* Profile = creat_profile();
	strcpy_s(Profile->AdmName, strlen(Name) + 1, Name);
	printf_s("����������(< = 12):");
	strcpy(Chr, identify(3, PASSWORD, "����������(< = 12):"));
	strcpy_s(Profile->Password, strlen(Chr) + 1, Chr);
	if (Annount != NULL)
	{
		if (Annount->Permission)
		{
			printf_s("�Ƿ���Ϊ����Ա(y/n)\n$");
			if (strcmp(identify(2, YORN, "$"), "y") == 0)
				Profile->Permission = true;
			else
				Profile->Permission = false;
		}
		else
			Profile->Permission = false;
	}
	else
		Profile->Permission = false;
	Profile->AdmLink = NULL;
	merege_profile(Profile, Index);
	return Profile;
}


void push_profile(void)
{
	logup* fp = AdmHLink;
	char chr[40] = "";
	File = fopen("Annount.txt", "w");
	fclose(File);
	File = fopen("Annount.txt", "a");
	for (int i = 0; i < Index; i++)
	{
		strcpy_s(chr, strlen(fp->AdmName) + 1, fp->AdmName);
		for (int j = strlen(fp->AdmName); j < 10; j++)
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
		strcat(chr, "\n");
		fwrite(chr, 1, strlen(chr), File);
		strcpy(chr, "");
		fp = fp->AdmLink;
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


void merege_profile(logup* Profile, int Ind)
{
	logup* fp = AdmHLink;
	for (int i = 0; i < Ind-1; i++)
		fp = fp->AdmLink;
	if (Ind < Index)
		Profile->AdmLink = fp->AdmLink;
	fp->AdmLink = Profile;
	Index++;
}



make* creat_equipment(void)
{
	make* Equipment = (make*)malloc(sizeof(make));
	return Equipment;
}


void pull_equipment(void)
{
	char Name[11];
	char Rem[17];
	char Purchase[17];
	char MasterName[11];
	char Storage[17];
	char Sit[6];
	make* fp = EquHLink = creat_equipment();
	File = fopen("Device.txt", "r");
	if (File != NULL)
	{
		while (true)
		{
			if (fscanf_s(File, "%s%s%s%s%s%s\n", Name, 11, Rem, 17, Purchase, 17, Storage, 17, MasterName, 11, Sit, 6) != 6)
			{
				free(fp);
				break;
			}
			Name[10] = '\0';
			strcpy_s(fp->EquName, strlen(Name) + 1, Name);
			Rem[16] = '\0';
			strcpy_s(fp->Remark, strlen(Rem) + 1, Rem);
			Purchase[16] = '\0';
			strcpy_s(fp->PurchaseTime, strlen(Purchase) + 1, Purchase);
			Storage[16] = '\0';
			strcpy_s(fp->StorageLocation, strlen(Storage) + 1, Storage);
			MasterName[10] = '\0';
			check_profile(MasterName);
			fp->Master = Annount;
			Sit[5] = '\0';
			if (strcmp(Sit, "true") == 0)
				fp->Situation = true;
			else
				fp->Situation = false;
			fp->EquLink = NULL;
			merege_equipment(fp, Count);
			fp = creat_profile();
		}
		fclose(File);
	}
}


make* init_equipment(void)
{
	make* Equipment = creat_equipment();
	char* Edit = NULL;
	if (Equipment != NULL)
	{
		if (Count == 0)
			EquHLink = Equipment;
		printf_s("�������豸��(< = 10):");
		Edit = identify(2, NAME, "�������豸(< = 10):");
		if (!check_equipment(Edit))
		{
			strcpy_s(Equipment->EquName, strlen(Edit) + 1, Edit);
			printf_s("�������豸�ͺ�(< = 16):");
			Edit = identify(2, REMARK, "����������ͺ�(< = 16):");
			strcpy_s(Equipment->Remark, strlen(Edit) + 1, Edit);
			printf_s("������ɹ�ʱ��(< = 16):");
			Edit = identify(2, REMARK, "������ɹ�ʱ��(< = 16):");
			strcpy_s(Equipment->PurchaseTime, strlen(Edit) + 1, Edit);
			Equipment->Master = Annount;
			printf_s("�����봢��λ��(< = 16):");
			Edit = identify(2, REMARK, "�����봢��λ��(< = 16):");
			strcpy_s(Equipment->StorageLocation, strlen(Edit) + 1, Edit);
			Equipment->Situation = true;
			Equipment->EquLink = NULL;
			merege_equipment(Equipment, Count);
			return Equipment;
		}
		else
			printf_s("������ͻ\n");
	}
	return NULL;
}


make* new_equipment(char* Name)
{
	make* Equipment = creat_equipment();
	char* Edit = NULL;
	if (Equipment != NULL)
	{
		if (EquHLink == NULL)
			EquHLink = Equipment;
		strcpy_s(Equipment->EquName, strlen(Name) + 1, Name);
		printf_s("�������豸�ͺ�(<=16):");
		Edit = identify(2, REMARK, "����������ͺ�(< = 16):");
		strcpy_s(Equipment->Remark, strlen(Edit) + 1, Edit);
		printf_s("������ɹ�ʱ��(<=16):");
		Edit = identify(2, REMARK, "������ɹ�ʱ��(< = 16):");
		strcpy_s(Equipment->PurchaseTime, strlen(Edit) + 1, Edit);
		Equipment->Master = Annount;
		printf_s("�����봢��λ��(<=16):");
		Edit = identify(2, REMARK, "�����봢��λ��(< = 16):");
		strcpy_s(Equipment->StorageLocation, strlen(Edit) + 1, Edit);
		Equipment->Situation = true;
		Equipment->EquLink = NULL;
		merege_equipment(Equipment, Count);
		return Equipment;
	}
	return NULL;
}


void merege_equipment(make* Equipment, int Cou)
{
	make* fp = EquHLink;
	for (int i = 0; i < Cou - 1; i++)
		fp = fp->EquLink;
	if (Cou < Count)
		Equipment->EquLink = fp->EquLink;
	fp->EquLink = Equipment;
	Count++;
}


bool check_equipment(char* chr)
{
	make* fp = EquHLink;
	for (int i = 0; i < Count; i++)
	{
		if (strcmp(fp->EquName, chr) == 0)
		{
			Device = fp;
			return true;
		}
		fp = fp->EquLink;
	}
	return false;
}


void push_equipment(void)
{
	make* fp = EquHLink;
	char chr[100] = "";
	File = fopen("Device.txt", "w");
	fclose(File);
	File = fopen("Device.txt", "a");
	for (int i = 0; i < Count; i++)
	{
		strcpy_s(chr, strlen(fp->EquName) + 1, fp->EquName);
		for (int j = strlen(fp->EquName); j < 10; j++)
			strcat(chr, " ");
		strcat(chr, "\t");
		strcat(chr, fp->Remark);
		for (int j = strlen(fp->Remark); j < 16; j++)
			strcat(chr, " ");
		strcat(chr, "\t");
		strcat(chr, fp->PurchaseTime);
		for (int j = strlen(fp->PurchaseTime); j < 16; j++)
			strcat(chr, " ");
		strcat(chr, "\t");
		strcat(chr, fp->StorageLocation);
		for (int j = strlen(fp->StorageLocation); j < 16; j++)
			strcat(chr, " ");
		strcat(chr, "\t");
		strcat(chr, fp->Master->AdmName);
		for (int j = strlen(fp->Master->AdmName); j < 10; j++)
			strcat(chr, " ");
		strcat(chr, "\t");
		if (fp->Situation)
			strcat(chr, "true ");
		else
			strcat(chr, "false");
		strcat(chr, "\n");
		fwrite(chr, 1, strlen(chr), File);
		strcpy(chr, "");
		fp = fp->EquLink;
	}
	fclose(File);
}



generate* creat_info(void)
{
	generate* Info = (generate*)malloc(sizeof(generate));
	return Info;
}


void pull_info(void)
{
	char Name[11];
	char Bow[11];
	time_t Borrow;
	time_t Return;
	generate* fp = InfHLink = creat_info();
	File = fopen("Info.txt", "r");
	if (File != NULL)
	{
		while (true)
		{
			if (fscanf_s(File, "%s%s%lld%lld\n\n\n", Name, 11, Bow, 11, &Borrow, &Return) != 4)
			{
				free(fp);
				break;
			}
			Name[10] = '\0';
			check_equipment(Name);
			fp->Equipment = Device;
			check_profile(Bow);
			fp->Borrower = Annount;
			fp->BorrowTime = Borrow;
			fp->ReturnTime = Return;
			fp->InfLink = NULL;
			merege_info(fp, History);
			fp = creat_info();
		}
		fclose(File);
	}
}


generate* init_info(make* Equ)
{
	time_t Tim = 0;
	generate* Info = creat_info();
	if (History == 0)
		InfHLink = Info;
	if (Equ->Situation)
	{
		Info->Equipment = Equ;
		Info->Borrower = Annount;
		time(&Tim);
		Info->BorrowTime = Tim;
		Info->ReturnTime = 0;
		Info->InfLink = NULL;
		merege_info(Info, History);
	}
	else
		printf("���豸�ѱ�����\n");
}

void push_info(void)
{
	generate* fp = InfHLink;
	char chr[120];
	File = fopen("Info.txt", "w");
	fclose(File);
	File = fopen("Info.txt", "a");
	for (int i = 0; i < History; i++)
	{
		strcpy_s(chr, strlen(fp->Equipment->EquName) + 1, fp->Equipment->EquName);
		for (int j = strlen(fp->Equipment->EquName); j < 10; j++)
			strcat(chr, " ");
		strcat(chr, "\t");
		strcat(chr, fp->Borrower->AdmName);
		for (int j = strlen(fp->Borrower->AdmName); j < 10; j++)
			strcat(chr, " ");
		strcat(chr, "\t");
		fwrite(chr, 1, strlen(chr), File);
		fprintf(File, "%lld\t%lld\n", fp->BorrowTime, fp->ReturnTime);
		strcpy(chr, "");
		fp = fp->InfLink;
	}
	fclose(File);
}

bool check_info(void)
{
	generate* fp = InfHLink;
	for (int i = 0; i < History; i++)
	{
		if ((fp->Equipment == Device) && (fp->ReturnTime == 0))
		{
			Record = fp;
			return true;
		}
		fp = fp->InfLink;
	}
	return false;
}


void merege_info(generate* Info, int His)
{
	generate* fp = InfHLink;
	for (int i = 0; i < History - 1; i++)
		fp = fp->InfLink;
	if (His < History)
		Info->InfLink = fp->InfLink;
	fp->InfLink = Info;
	History++;
}


void ls_a(void)
{
	make* fp = EquHLink;
	char chr[120] = "";
	if (Count != 0)
	{
		printf("������\t\t�ͺ�\t\t\t�ɹ�ʱ��\t\t����ص�\t\t������\t\t״̬\n");
		for (int i = 0; i < Count; i++)
		{
			strcpy_s(chr, strlen(fp->EquName) + 1, fp->EquName);
			for (int j = strlen(fp->EquName); j < 10; j++)
				strcat(chr, " ");
			strcat(chr, "\t");
			strcat(chr, fp->Remark);
			for (int j = strlen(fp->Remark); j < 16; j++)
				strcat(chr, " ");
			strcat(chr, "\t");
			strcat(chr, fp->PurchaseTime);
			for (int j = strlen(fp->PurchaseTime); j < 16; j++)
				strcat(chr, " ");
			strcat(chr, "\t");
			strcat(chr, fp->StorageLocation);
			for (int j = strlen(fp->StorageLocation); j < 16; j++)
				strcat(chr, " ");
			strcat(chr, "\t");
			strcat(chr, fp->Master->AdmName);
			for (int j = strlen(fp->Master->AdmName); j < 10; j++)
				strcat(chr, " ");
			strcat(chr, "\t");
			if (fp->Situation)
			{
				Device = fp;
				if (check_info())
				{
					strcat(chr, "�ѽ��");
					strcat(chr, "-");
					strcat(chr, ctime(&Record->BorrowTime));
				}
				else
					strcat(chr, "�ɽ�");
			}
			else
				strcat(chr, "����");
			printf_s("%s\n", chr);
			strcpy(chr, "");
			fp = fp->EquLink;
		}
	}
	else
		printf_s("�����豸\n");
}


void ls_p(void)
{
	make* fp = EquHLink;
	for (int i = 0; i < Count; i++)
	{
		if (fp->Master == Annount)
			printf_s("%s\t", fp->EquName);
		fp = fp->EquLink;
	}
	printf_s("\n");
}


void ls_v(void)
{
	generate* fp = InfHLink;
	for (int i = 0; i < History; i++)
	{
		if ((fp->Borrower == Annount) && (fp->ReturnTime == 0))
			printf_s("%s\t%s\n", fp->Equipment->EquName, ctime(&fp->BorrowTime));
		fp = fp->InfLink;
	}
}


bool del_profile(void)
{
	logup* fp = AdmHLink;
	generate* p = InfHLink;
	for (int i = 0; i < History; i++)
	{
		if (p->Borrower == Annount)
			return false;
		p = p->InfLink;
	}
	for (int i = 0; i < Index; i++)
	{
		if (fp->AdmLink == Annount)
			break;
		fp->AdmLink;
	}
	fp->AdmLink = fp->AdmLink->AdmLink;
	return true;
}


void del_equipment(void)
{
	make* fp = EquHLink;
	for (int i = 0; i < Count; i++)
	{
		if (fp->EquLink == Device)
			break;
		fp->EquLink;
	}
	fp->EquLink = fp->EquLink->EquLink;
}