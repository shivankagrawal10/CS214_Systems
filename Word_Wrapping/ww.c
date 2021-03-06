#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include "ww.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


#define BUFFSIZE 2

int wrap(int fd, size_t len){
  char* buff= malloc(BUFFSIZE);
  strbuf_t* currword= malloc(sizeof(strbuf_t));
  sb_init(currword,len);
  int num_read = 1;
  int offset = 0;
  int outcount=0;
  int newlineflag=0;
  int started=0;
  int isfirstword=1;
  int whitespaceflag=0;
  int fail=0;
  while(num_read>0){
    num_read = pread(fd,buff,BUFFSIZE,offset);
    offset += num_read;
    //printf("%d \n",num_read);
    for(int i=0; i<num_read; i++){
      read_word(currword,buff[i],&started);
      if(isspace(buff[i])){
            //printf("%s \n",currword->data);

            if(buff[i]=='\n')
            {
               newlineflag++;
               whitespaceflag=0;
                 
            }
            else
            {
              whitespaceflag++;
              newlineflag=0;
            }
            if(started!=0)
            {
              if((whitespaceflag==1 || newlineflag!=0) && (whitespaceflag!=1 || newlineflag==0))
              {
                write_word(currword,&outcount,len,newlineflag,started,isfirstword, &fail);
              }
              
            isfirstword=0;
            sb_destroy(currword);
            sb_init(currword,len);
            }
      }
      //normal char
      else
      {
           newlineflag=0;
           whitespaceflag=0;
      }
    }
  }
  write_word(currword,&outcount,len,newlineflag,started,isfirstword, &fail);
  sb_destroy(currword);
  free(buff);
  free(currword);
  return fail;
}

strbuf_t* read_word(strbuf_t* currword, char currletter, int *started){
  if(isspace(currletter) && currword -> used == 0){
    return currword;
  }
  if(!isspace(currletter))
  {
    *started=1;
    sb_append(currword,currletter);
  }
  return currword;

}

void write_word(strbuf_t* currword, int *outcount, size_t limit, int newlineflag, int started, int isfirstword, int *fail){
      
      int sizewritten=*outcount;
      int currsize=currword->used;
      int added=1;
      if(newlineflag==2)
      {
        char parabuf[2];
        parabuf[0]='\n';
        
        parabuf[1]='\n';
        write(1,parabuf,2);
        sizewritten=0;
      }
      if(sizewritten==0 || isfirstword==1)
      {
        added=0;
      }
      
      if(sizewritten+currsize+added<=limit)
      {
        if(sizewritten!=0 && isfirstword==0 && currword->used!=0)
        {
        //putting prior space
         char tempspace[1];
         tempspace[0]=' ';
         write(1,tempspace,1);
        }
        //writing word

        char *tempword=currword->data;
        write(1,tempword,currsize);
        *outcount=sizewritten+currsize+added;
      }
      //doesn't fit
      else
      {
         //skip line
         if(newlineflag!=2)
         {
         char tempnewline[1];
         tempnewline[0]='\n';
         write(1,tempnewline,1);
         }
         if(currsize>limit)
         {
           *fail=1;
           char *tempword=currword->data;
           write(1,tempword,currsize);
           char tempnewline[1];
           tempnewline[0]='\n';
           write(1,tempnewline,1);
           *outcount=0;
           return;
         }

        char *tempword=currword->data;
        write(1,tempword,currsize);
        *outcount=currsize;
      }
      

}

int isdirect(char *name) {
    
    struct stat data;
    int err=stat(name,&data);
    
    if(err!=0)
    {
       perror(name);
       return 2;
    }

    return S_ISDIR(data.st_mode);


}

int main(int argc,char* argv[argc+1]){
  
  
  //Edge Case-if arguments are wrong
  if(argc<2)
  {
     return EXIT_FAILURE;
  }
  if(isdigit(argv[1][0])==0)
  {
     return EXIT_FAILURE;
  }
  
  int argtype=isdirect(argv[2]);

  if(argc==2)
  {
    argtype=0;
  }
  
  if(argtype==2)
  {
     return EXIT_FAILURE;
  }
  //normal file
  if(argtype==0)
  {
        int file;

        if(argc==2)
        {
          file=0;
        }
        else
        {
          file = open(argv[2],O_RDONLY);
        }

        if(file==-1)
        {
          perror("Error: ");
          return EXIT_FAILURE;
        }
        
        int retval=wrap(file,atoi(argv[1]));

        if(retval==1)
        {
          return EXIT_FAILURE;
        }
        close(file);
  }
  //directory
  else if(argtype==1)
  {
     DIR * directptr=opendir(argv[2]);

     if(directptr==NULL)
     {
       //maybe print what problem is here
       return EXIT_FAILURE;
     }
     struct dirent *de;
     
     while ((de = readdir(directptr))) {
         
         char type=de->d_type;
         if(type==DT_REG)
         {
            //open up file
            //char *inputfile=de->d_name;
            //int filedirect=open(inputfile,O_RDONLY);
            //create new file

            //call write on it

         }
         //else ignore subdirectories
     }

     closedir(directptr);
  }




  
  
  
  
  return EXIT_SUCCESS;
}
