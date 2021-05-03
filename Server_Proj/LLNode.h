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
   strbuf_t *key;
   strbuf_t *value;
   struct LLNode *next;
}LLNode;

typedef struct LLNode* LLNodePTR;

LLNodePTR LLNodeInit(LLNodePTR,char*,char*);
void LLPrint(LLNodePTR);
int LLLength(LLNodePTR);
LLNodePTR SelectionSort(LLNodePTR);
void FreeLL(LLNodePTR );
