#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup" )
#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include <windows.h>
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

void scan();
void copy(char *src, char *dst);
void* local2port(void* args);
void* greeting(void* args);
#endif // FUNC_H_INCLUDED
