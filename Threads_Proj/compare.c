#include "compare.h"
#define LEN 10
#define BUFFSIZE 10
LLNodePTR* tokenize(int fd_read,int file_index, LLNodePTR* freq_dist)
{
    char* buff= malloc(BUFFSIZE);
    strbuf_t* currword= malloc(sizeof(strbuf_t));
    sb_init(currword,(size_t) LEN);
    int num_read = 1;
    int offset = 0;
    //int outcount=0;
    int newlineflag=0;
    int started=0;
    //int isfirstword=1;
    int whitespaceflag=0;
    //int fail=0;
    while(num_read>0)
    {
        // reads char from file into buffer (size specified by macro)
	    num_read = pread(fd_read,buff,BUFFSIZE,offset);
	    offset += num_read;
	    // Iterating through buffer
	    for(int i=0; i<num_read; i++)
	    {
            //printf("curr letter: %c",buff[i]);
            //adds nonspace char to currword strbuf
            read_word(currword,buff[i],&started);
            //space character
            if(isspace(buff[i]))
            {
                //checks if it's a newline character
                if(buff[i]=='\n')
                {
                    newlineflag++;
                }
                else
                {
                    whitespaceflag++;
                }
                //checks whether first word of file
                if(started!=0)
                {
                    //writes word to output when only encountering 1 space character
                    if((whitespaceflag==1 || newlineflag==1)&&currword->used>0)
                    {
                        printf("|%d|\n",freq_dist[file_index]);
                        //write_word(fd_write,currword,&outcount,len,newlineflag,started,isfirstword, &fail,0);
                    }
                    //isfirstword=0;
                    sb_destroy(currword);
                    sb_init(currword,LEN);
                }
            }
            else
            {
              newlineflag=0;
              whitespaceflag=0;
            }
        }
    }
    sb_destroy(currword);
    free(buff);
    free(currword);
    return freq_dist;
}

strbuf_t* read_word(strbuf_t* currword, char currletter,int *started)
{
    if(!isspace(currletter))
    {
        *started = 1;
        sb_append(currword,currletter);
    }
    return currword;
}

//returns 0 if file, 1 if directory, 2 if file/directory not found
int isdirect(char *name)
{
    struct stat data;
    int err=stat(name,&data);
    if(err!=0)
    {
       perror(name);
       return 2;
    }
    return S_ISDIR(data.st_mode);
}

LLNode* LLNodeInit(LLNode* freq_dist,int numfiles)
{
  for (int i=0;i<numfiles;i++)
  {
    freq_dist[i] = malloc(sizeof(LLNode));
  }
  return freq_dist;
}

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

  /*
  Temporary face holder code to test tokenize and create frequency distribution
  - needs to do necessary check for valid files and subdirectories
  - needs to feed file descriptors to tokenize method
  - numfiles: need count of how many files we are working with
  */
  printf("%d, %d, %d, %s\n",analysisthreads,filethreads,directthreads,suffix);
  int numfiles = 2;
  LLNode* freq_dist [numfiles];
  //LLNodeInit(freq_dist,numfiles);
  // temporary file input
  int file;
  int file_index = 2;
  file = open(argv[2],O_RDONLY);
  tokenize(file,file_index,freq_dist);



  return EXIT_SUCCESS;
}
