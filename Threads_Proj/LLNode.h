#ifndef STANDARD_H
#define STANDARD_H
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
#include <math.h>
#endif

#ifndef STRBUF_H
#define STRBUF_H
#include "strbuf.h"
#endif

typedef struct LLNode
{
   strbuf_t *word;
   char *name;
   int occurrences;
   double frequency;
   struct LLNode *next;
}LLNode;

typedef struct LLNode* LLNodePTR;

typedef struct LLNodeMean
{
    char *word;
    double frequency;
    struct LLNodeMean *next;
}LLNodeMean;

LLNodePTR* LLNodeInit(LLNodePTR*,int);
void LLPrint(LLNodePTR *,int);
int LLLength(LLNodePTR);
LLNodePTR SelectionSort(LLNodePTR);
void FreeLL(LLNodePTR *, int);