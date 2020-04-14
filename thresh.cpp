//#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup" ) 

#include "func.h"
#include "func.cpp"
#include <pthread.h>


int main()
{
pthread_t tids[5];
int ret1 = pthread_create(&tids[0],NULL,rua,NULL);
int ret2 = pthread_create(&tids[1],NULL,greeting,NULL);
/**Thresh is what we want to run ,
	It may be changed to kidnap.exe~
**/

//if(ret!=0)
//	std::cout<<"pthread_create error"<<ret<<"\n";



pthread_exit(NULL);
}


