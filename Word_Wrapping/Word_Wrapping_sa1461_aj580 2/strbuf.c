#include <stdlib.h>
#include <stdio.h>
#include "strbuf.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int sb_init(strbuf_t *L, size_t length)
{
    L->data = malloc(sizeof(char) * length);
    if (!L->data) return 1;

    L->length = length;
    L->used   = 0;
    L=update_null(L);
    return 0;
}

void sb_destroy(strbuf_t *L)
{
    free(L->data);
}

int sb_append(strbuf_t *L, int item)
{
  if (L->used == L->length) {
    size_t size = L->length * 2;
    char *p = realloc(L->data, sizeof(int) * size);
    if (!p) return 1;

    L->data = p;
    L->length = size;

    if (DEBUG) printf("Increased size to %lu\n", size);
  }
  L=remove_null(L);
  L->data[L->used] = item;
  ++L->used;
  L=update_null(L);
  return 0;
}


int sb_remove(strbuf_t *L, char *item)
{
    if (L->used == 0) return 1;
    L=remove_null(L);
    --L->used;

    if (item) *item = L->data[L->used];
    L=update_null(L);
    return 0;
}

int sb_insert(strbuf_t *L, int index, char item)
{
  if(index<0){
    return 1;
  }
  if(index<=L->length-1){
    L=remove_null(L);
    if(L->used==L->length){
    	L=sb_expand(L,L->used);
    }
    if (index<=L->used-1){  //handle normal case
      for (int i = L->used-1;i>=index;i--){
	L->data[i+1]=L->data[i];
      }
      L->data[index]=item;
      L->used++;
    }
    else{  //handles when index exceed number of entries
      L->data[index]=item;
      L->used=index+1;
    }
  }
  else if (index > L->length-1){
    L=sb_expand(L,index);
    if(!L){
      return 1;
    }
    L=remove_null(L);
    L->data[index]=item;
    L->used=index+1;
  }  
  L=update_null(L);
  return 0;   
}

int sb_concat(strbuf_t* L, char *str)
{
  int si=len(str);
  //printf("%s \n",str);
  //printf("%d \n",si);
  if(si<=0 || L==0){
    return 1;
  }
  for (int i=0;i<si;i++){
    sb_append(L,str[i]);
  }
  return 0;
}

int len(char* str)
{
  int i =0;
  while(str[i]!='\0')
  {
    i+=1;
  }
  return i;
}
strbuf_t* sb_expand(strbuf_t *L, int index){
  size_t newsize = 2*L->length;
  if ((index+1)>newsize*sizeof(char)){
    newsize = (index+1);
  }
  char* val = (char *) realloc(L->data,newsize*sizeof(char));
  if(!val){
    return 0;
  }
  L->data=val;
  L->length=newsize;
  return L;
}

strbuf_t * update_null(strbuf_t* L)
{
  if(L->used==L->length){
    L=sb_expand(L,L->used);
  }
  L->data[L->used]='\0';
  return L;
}

strbuf_t* remove_null(strbuf_t* L)
{
  L->data[L->used]=-1;
  return L;
}
void sb_print(strbuf_t *L)
{
   printf("%s\n",L->data);
}
