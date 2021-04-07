#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

typedef struct LLNode
{
   char *word;
   int occurrences;
   float frequency;
   LLNode *next;

}LLNode;



int main(int argc,char* argv[argc+1])
{
  //dashed args must be at beginning
  int directthreads=1;
  int filethreads=1;
  int analysisthreads=1;
  char *suffix=".txt";
  
  int nondasharg=0;
  int nondashstart=0;
  
  for(int i=1;i<argc;i++)
  {
      char *currentarg=argv[i];
      if(currentarg[0]=='-')
      {
         if(currentarg[1]=='d')
         {
             char temp[strlen(currentarg)-2];
             int index=0;
             for(int x=2;x<strlen(currentarg);x++)
             {
                temp[index]=currentarg[x];
                index++;
             }
             int tempval=atoi(temp);
             if(tempval<=0)
             {
                 fprintf(stderr,"%s","Invalid -d argument");
                 return EXIT_FAILURE;
             }
             directthreads=tempval;
         }
         else if(currentarg[1]=='f')
         {
            char temp[strlen(currentarg)-2];
             int index=0;
             for(int x=2;x<strlen(currentarg);x++)
             {
                temp[index]=currentarg[x];
                index++;
             }
             int tempval=atoi(temp);
             if(tempval<=0)
             {
                 fprintf(stderr,"%s","Invalid -f argument");
                 return EXIT_FAILURE;
             }
             filethreads=tempval;
         }
         else if(currentarg[1]=='a')
         {
            char temp[strlen(currentarg)-2];
             int index=0;
             for(int x=2;x<strlen(currentarg);x++)
             {
                temp[index]=currentarg[x];
                index++;
             }
             int tempval=atoi(temp);
             if(tempval<=0)
             {
                 fprintf(stderr,"%s","Invalid -a argument");
                 return EXIT_FAILURE;
             }
             analysisthreads=tempval;
         }
         else if(currentarg[1]=='s')
         {
             char temp[strlen(currentarg)-2];
             int index=0;
             for(int x=2;x<strlen(currentarg);x++)
             {
                temp[index]=currentarg[x];
                index++;
             }
             suffix=temp;
         }
         else
         {
             fprintf(stderr,"%s","Invalid dash argument");
             return EXIT_FAILURE;
         }
      }
      else
      {
          nondasharg++;
          if(nondashstart==0)
          {
              nondashstart=i;
          }
      }
  }


  if(nondasharg<1)
  {
     fprintf(stderr,"%s","Not enough file arguments provided");
     return EXIT_FAILURE;
  }





  return EXIT_SUCCESS;
}
