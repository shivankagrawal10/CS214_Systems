#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<fcntl.h>
#include "ww.h"

int main(int argc,char* argv[argc+1]){
  char* text= malloc(30);
  int file = open(argv[1],O_RDONLY);
  read(file,text,20);
  printf("%s \n",text);
  close(file);
  /*
  FILE* file = fopen("argv[1]","r");
  fscanf(file,"%s",text);
  printf("%d\n",(int)sizeof(text));
  fclose(file);
  */
  free(text);
  return EXIT_SUCCESS;
  

}
