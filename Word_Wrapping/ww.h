#ifndef STRBUF_H
#define STRBUF_H "strbuf.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include STRBUF_H
#endif


int wrap(int,size_t,int);
strbuf_t* read_word(strbuf_t*, char, int*);
void write_word(int,strbuf_t*, int*, size_t,int, int, int,int*,int);
int isdirect(char *);

