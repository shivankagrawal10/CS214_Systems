#ifndef COMPARE_H
#define COMPARE_H
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
#include "strbuf.h"
#endif

typedef struct LLNode
{
   strbuf_t *word;
   int occurrences;
   float frequency;
   struct LLNode *next;
}LLNode;

typedef struct LLNode* LLNodePTR;

LLNodePTR* tokenize(int,int,LLNodePTR*);
strbuf_t *read_word(strbuf_t *, char, int *);
int isdirect(char *);
LLNode* LLNodeInit(LLNode*,int);
