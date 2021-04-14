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

/*
#ifndef LLNODE_H
#define LLNODE_H
#include "LLNode.h"
#endif

#ifndef QNODE_H
#define QNODE_H
#include "QNode.h"
#endif
*/
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

typedef struct QNode
{
    strbuf_t *path;
    struct QNode*next;
}QNode;

typedef struct Queue
{
    QNode *front;
    QNode *last;
    unsigned count;
    int open;
    pthread_mutex_t qlock;
    pthread_cond_t read_ready;
    //pthread_cond_t thread_ready;
}Queue;

typedef struct filepair
{
    int index1;
    int index2;

}filepair;

typedef struct anargs
{
    filepair* pairsarr;
    int pairbegin;
    int pairend;

}anargs;

typedef struct finalresult
{
    int commonwords;
    double JSD;
    char *f1path;
    char *f2path;

}finalresult;

int suffixcheck(char *,char*);

int qinit(Queue *,int ,QNode *,QNode *);
void QEnqueue(char *, char*, int);
void *DirQDequeue(void *);
void QClose(Queue *);
void QPrint(Queue *);
int DirectorySearch(QNode *);
void QFree();

void FDequeue();

LLNodePTR tokenize(int,char *);
strbuf_t *read_word(strbuf_t *, char, int *);
int isdirect(char *);

void * analysis(void *);

LLNodePTR* LLNodeInit(LLNodePTR*,int);
void LLPrint(LLNodePTR *,int);
int LLLength(LLNodePTR);
LLNodePTR SelectionSort(LLNodePTR);
void FreeLL(LLNodePTR *, int);

