#include"LLNode.h"

LLNodePTR LLNodeInit(LLNodePTR front,char* key,char *value)
{
  LLNodePTR temp = malloc(sizeof(LLNode));
  temp->key = malloc(sizeof(strbuf_t)); //0;
  sb_init(temp->key,10);
  sb_concat(temp->key,key);
  temp->value = malloc(sizeof(strbuf_t)); //0;
  sb_init(temp->value,10);
  sb_concat(temp->value,value);
  temp->next = front;
  return temp;
}

void LLPrint(LLNodePTR front)
{
  while(front!=0)
  {
    //sb_print(front->word);
    printf("%s (%zu) -> %s (%zu), ",front->key->data,front->key->used,front->value->data,front->value->used);
    front = front->next;
  }
  printf("\n");
}

int LLLength(LLNodePTR front){
  int len = 0;
  while(front != 0)
  {
    len ++;
    front = front -> next;
  }
  return len;
}

LLNodePTR SelectionSort(LLNodePTR front)
{
  LLNodePTR temp = front;
  LLNodePTR min = temp;
  LLNodePTR curr = 0;
  LLNodePTR prev = 0;
  int ll_length = LLLength(front);
  for (int i = 0; i < ll_length; i++)
  {
    LLNodePTR temp_prev = 0;
    if(curr != 0)
    {
      temp = curr->next;
      min = temp;
    }
    while(temp!=0)
    {
      if(strcmp(min->key->data,temp->key->data)>0)
      {
        prev = temp_prev;
        min = temp;
      }
      temp_prev = temp;
      temp = temp -> next;
    }
    if(curr != 0 && curr -> next == min)
    {
       prev = curr;
       curr = curr -> next;
       continue;
    }
    if(curr == 0 && min != front)
    {
      prev -> next = min -> next;
      curr = min;
      curr -> next = front;
      front = curr;
    }
    else
    {
      prev -> next = min -> next;
      min -> next = curr -> next;
      curr -> next = min;
      curr = min;
    }
  }
  return front;
}

void FreeLL(LLNodePTR* LL, int num_files)
{
  for (int i = 0;i < num_files; i++)
  {
    LLNodePTR temp = LL[i];
    LLNodePTR next = 0;
    while (temp != 0)
    {
      next = temp -> next;
      free(temp->key->data);
      free(temp->key);
      free(temp->value->data);
      free(temp->value);
      free(temp);
      temp = next;
    }
  }
}