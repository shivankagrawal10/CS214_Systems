#include <stdlib.h>
#include <stdio.h>
#include "arraylist.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int al_init(arraylist_t *L, size_t length)
{
    L->data = malloc(sizeof(int) * length);
    if (!L->data) return 1;

    L->length = length;
    L->used   = 0;

    return 0;
}

void al_destroy(arraylist_t *L)
{
    free(L->data);
}

int al_append(arraylist_t *L, int item)
{
    if (L->used == L->length) {
	size_t size = L->length * 2;
	int *p = realloc(L->data, sizeof(int) * size);
	if (!p) return 1;

	L->data = p;
	L->length = size;

	if (DEBUG) printf("Increased size to %lu\n", size);
    }

    L->data[L->used] = item;
    ++L->used;

    return 0;
}


int al_remove(arraylist_t *L, int *item)
{
    if (L->used == 0) return 1;

    --L->used;

    if (item) *item = L->data[L->used];

    return 0;
}

int al_insert(arraylist_t *L, int index, int item)
{
  if(index<0){
    return 1;
  }
  if(index<=L->length-1){  
    if (index<=L->used-1){  //handle normal case
      if(L->used==L->length){
	L=al_expand(L,L->used);
      }
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
    L=al_expand(L,index);
    if(!L){
      return 1;
    }
    L->data[index]=item;
    L->used=index+1;
  }  
  return 0;   
}

arraylist_t* al_expand(arraylist_t *L, int index){
  size_t newsize = 2*L->length;
  if ((index+1)>newsize*sizeof(int)){
    newsize = (index+1);
  }
  int* val = (int *) realloc(L->data,newsize*sizeof(int));
  if(!val){
    return 0;
  }
  L->data=val;
  L->length=newsize;
  return L;
}

void al_print(arraylist_t *L)
{
  for (int i=0;i<L->used;i++){
    printf("%d, ",L->data[i]);
  }
  printf("\n");
}
