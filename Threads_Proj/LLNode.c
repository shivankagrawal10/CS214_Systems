#include"LLNode.h"

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
    printf("start (%d):\n",i);
    LLNodePTR front = freq_dist[i];
    while(front!=0)
    {
      printf("|%s, %d| - ",front->word->data,front->occurrences);
      front = front->next;
    }
    printf("\n");
  }
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
      if(strcmp(min->word->data,temp->word->data)>0)
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
      free(temp->word->data);
      free(temp->word);
      free(temp);
      temp = next;
    }
  }
}