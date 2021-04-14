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
}Queue;

int qinit(Queue *,int ,QNode *,QNode *);
void QEnqueue(char *, char*, int);
void *DirQDequeue(void *);
void QClose(Queue *);
void QPrint(Queue *);
int DirectorySearch(QNode *);
void QFree();


void FDequeue();