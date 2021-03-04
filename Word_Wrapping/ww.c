#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include "ww.h"

#define BUFFSIZE 2

int wrap(int fd, size_t len){
  char* buff= malloc(BUFFSIZE);
  strbuf_t* currword= malloc(sizeof(strbuf_t));
  sb_init(currword,len);
  int num_read = 1;
  int offset = 0;
  while(num_read>0){
    num_read = pread(fd,buff,BUFFSIZE,offset);
    offset += num_read;
    printf("%d \n",num_read);
    for(int i=0; i<num_read; i++){
      read_word(currword,buff[i]);
      if(isspace(buff[i])&& currword->used !=0 ){
	printf("%s \n",currword->data);
	//write_word();
	sb_destroy(currword);
	sb_init(currword,len);
      }
    }
  }
  sb_destroy(currword);
  free(buff);
  free(currword);
  return 0;
}

strbuf_t* read_word(strbuf_t* currword, char currletter){
  if(isspace(currletter) && currword -> used == 0){
    return currword;
  }
  sb_append(currword,currletter);
  return currword;

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
  
  wrap(file,atoi(argv[1]));
  
  close(file);
  
  return EXIT_SUCCESS;
}
