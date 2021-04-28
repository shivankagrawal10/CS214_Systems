#include "server.h"
#define BACKLOG 5


//global vars
char *portnum;
pthread_mutex_t connlock;



void *respondwork(void *arg)
{


    return NULL;
}


int main(int argc,char* argv[argc+1])
{
     if(argc!=2)
     {
        fprintf(stderr,"%s","Invalid number of arguments");
        return EXIT_FAILURE;
     }
     int portcheck=atoi(argv[1]);
     if(portcheck<=0)
     {
        fprintf(stderr,"%s","Please make sure the port number is an integer");
        return EXIT_FAILURE;
     }
     portnum=malloc(strlen(argv[1])+1);
     portnum=argv[1];
}