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
    LLNode *current_node = freq_dist[file_index];
    LLNode *prev_node = 0;
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
                      current_node = freq_dist[file_index];
                      while(current_node!=0){
                        if(current_node->word==0) break;
                        if(sb_comp(current_node->word,currword)){
                          if(current_node != freq_dist[file_index]){
                            prev_node->next = current_node->next;
                            current_node = freq_dist[file_index];
                            freq_dist[file_index] = current_node;
                          }
                          current_node->occurrences++;
                          break;
                        }
                        prev_node = current_node;
                        current_node = current_node->next;
                      }
                      if(current_node == 0||current_node->word==0){
                        LLNode *new_node = malloc(sizeof(LLNode));
                        new_node->word = malloc(sizeof(strbuf_t));
                        sb_init(new_node->word,currword->used);
                        sb_concat(new_node->word,currword->data);
                        new_node->occurrences = 1;
                        new_node->next = 0;
                        if(current_node == 0){
                          prev_node -> next = new_node;
                        }
                        else if(current_node->word == 0){
                          freq_dist[file_index] = new_node;
                        }
                      }
                      //printf("|%s|\n",_node->word->data);
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
    if(!isspace(currletter)&&(isalpha(currletter)||isdigit(currletter)||currletter=='-'))
    {
        *started = 1;
        sb_append(currword,tolower(currletter));
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

LLNodePTR* LLNodeInit(LLNodePTR* freq_dist,int num_files)
{
  for (int i=0;i<num_files;i++)
  {
    freq_dist[i] = malloc(sizeof(LLNode));
    freq_dist[i]->word = 0;
    freq_dist[i]->next = 0;
  }
  return freq_dist;
}

void LLPrint(LLNodePTR *freq_dist,int num_files)
{
  for (int i = 0; i < num_files; i++)
  {
    LLNodePTR front = freq_dist[i];
    while(front!=0)
    {
      printf("|%s, %d| - ",front->word->data,front->occurrences);
      front = front->next;
    }
    printf("\n");
  }
}

int main(int argc,char* argv[argc+1])
{
  //dashed args must be at beginning
  int direct_threads=1;
  int file_threads=1;
  int analysis_threads=1;
  char *suffix=".txt";

  int nondash_arg=0;
  int nondash_start=0;

  for(int i=1;i<argc;i++)
  {
      char *current_arg=argv[i];
      if(current_arg[0]=='-')
      {
         if(current_arg[1]=='d')
         {
             char temp[strlen(current_arg)-2];
             int index=0;
             for(int x=2;x<strlen(current_arg);x++)
             {
                temp[index]=current_arg[x];
                index++;
             }
             int temp_val=atoi(temp);
             if(temp_val<=0)
             {
                 fprintf(stderr,"%s","Invalid -d argument");
                 return EXIT_FAILURE;
             }
             direct_threads=temp_val;
         }
         else if(current_arg[1]=='f')
         {
            char temp[strlen(current_arg)-2];
             int index=0;
             for(int x=2;x<strlen(current_arg);x++)
             {
                temp[index]=current_arg[x];
                index++;
             }
             int temp_val=atoi(temp);
             if(temp_val<=0)
             {
                 fprintf(stderr,"%s","Invalid -f argument");
                 return EXIT_FAILURE;
             }
             file_threads=temp_val;
         }
         else if(current_arg[1]=='a')
         {
            char temp[strlen(current_arg)-2];
             int index=0;
             for(int x=2;x<strlen(current_arg);x++)
             {
                temp[index]=current_arg[x];
                index++;
             }
             int temp_val=atoi(temp);
             if(temp_val<=0)
             {
                 fprintf(stderr,"%s","Invalid -a argument");
                 return EXIT_FAILURE;
             }
             analysis_threads=temp_val;
         }
         else if(current_arg[1]=='s')
         {
             char temp[strlen(current_arg)-2];
             int index=0;
             for(int x=2;x<strlen(current_arg);x++)
             {
                temp[index]=current_arg[x];
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
          nondash_arg++;
          if(nondash_start==0)
          {
              nondash_start=i;
          }
      }
  }


  if(nondash_arg<1)
  {
     fprintf(stderr,"%s","Not enough file arguments provided");
     return EXIT_FAILURE;
  }

  /*
  Temporary face holder code to test tokenize and create frequency distribution
  - needs to do necessary check for valid files and subdirectories
  - needs to feed file descriptors to tokenize method
  - num_files: need count of how many files we are working with
  */
  printf("%d, %d, %d, %s\n",analysis_threads,file_threads,direct_threads,suffix);
  int num_files = 1;
  LLNode** freq_dist = malloc(sizeof(LLNode*)*num_files); //[num_files];
  freq_dist = LLNodeInit(freq_dist,num_files);
  // temporary file input
  int file;
  int file_index = 0;
  file = open(argv[2],O_RDONLY);
  freq_dist = tokenize(file,file_index,freq_dist);
  LLPrint(freq_dist,num_files);



  return EXIT_SUCCESS;
}
