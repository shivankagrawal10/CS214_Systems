#ifndef STRBUF_H
#define STRBUF_H "strbuf.h"
#include STRBUF_H
#endif


int wrap(int,size_t,int);
strbuf_t* read_word(strbuf_t*, char, int*);
void write_word(int,strbuf_t*, int*, size_t,int, int, int,int*,int);
int isdirect(char *);

