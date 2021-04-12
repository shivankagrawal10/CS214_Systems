#include "compare.h"
#define LEN 10
#define BUFFSIZE 10

//global vars
LLNode** freq_dist;
pthread_mutex_t anlock;
finalresult**results;
int resultsindex=0;

int suffixcheck(char *pathname,char*suffix)
{
    int suffixlen=strlen(suffix);
    int start=strlen(pathname)-suffixlen;
    int sindex=0;
    for(int i=start;i<strlen(pathname);i++)
    {
        if(pathname[i]!=suffix[sindex])
        {
            return 0;
        }
        suffix++;
    }
    return 1;
}

int qinit(Queue *Q,int num,QNode*ptrtoLL)
{
     Q->count=0;
     Q->open=1;
     Q->front=ptrtoLL;
    //pthread_mutex_init(&Q->qlock, NULL);
	//pthread_cond_init(&Q->read_ready, NULL);
     return 0;
}

LLNodePTR tokenize(int fd_read,char *filename,int file_index, LLNodePTR* freq_dist)
{
    char* buff= malloc(BUFFSIZE);
    strbuf_t* currword= malloc(sizeof(strbuf_t));
    sb_init(currword,(size_t) LEN);
    int num_read = 1;
    int offset = 0;
    int total_words = 0;
    //int outcount=0;
    int newlineflag=0;
    int started=0;
    //int isfirstword=1;
    int whitespaceflag=0;
    //int fail=0;
    LLNode *current_node = freq_dist[file_index];
    LLNode *prev_node = 0;
    while(num_read>0)
    {
        // reads char from file into buffer (size specified by macro)
	    num_read = pread(fd_read,buff,BUFFSIZE,offset);
	    offset += num_read;
	    // Iterating through buffer
	    for(int i=0; i<num_read; i++)
	    {
            //printf("curr letter: %c",buff[i]);
            //adds nonspace char to currword strbuf
            read_word(currword,buff[i],&started);
            //space character
            if(isspace(buff[i]))
            {
                //checks if it's a newline character
                if(buff[i]=='\n')
                {
                    newlineflag++;
                }
                else
                {
                    whitespaceflag++;
                }
                //checks whether first word of file
                if(started!=0)
                {
                  if((whitespaceflag==1 || newlineflag==1)&&currword->used>0)
                    {
                      ++total_words;
                      current_node = freq_dist[file_index];
                      while(current_node!=0){
                        if(current_node->word==0) break;
                        if(sb_comp(current_node->word,currword)){
                          //printf("%s, %s",current_node->word->data,currword->data);
                          if(current_node != freq_dist[file_index]){
                            prev_node->next = current_node->next;
                            current_node ->next = freq_dist[file_index];
                            freq_dist[file_index] = current_node;
                          }
                          current_node->occurrences++;
                          break;
                        }
                        prev_node = current_node;
                        current_node = current_node->next;
                      }
                      if(current_node == 0||current_node->word==0){
                        LLNode *new_node = malloc(sizeof(LLNode));
                        new_node->word = malloc(sizeof(strbuf_t));
                        sb_init(new_node->word,currword->used);
                        sb_concat(new_node->word,currword->data);
                        new_node->occurrences = 1;
                        new_node->next = 0;
                        new_node->name=filename;
                        if(current_node == 0){
                          prev_node -> next = new_node;
                        }
                        else if(current_node->word == 0){
                          freq_dist[file_index] = new_node;
                        }
                      }
                    }
                    //isfirstword=0;
                    sb_destroy(currword);
                    sb_init(currword,LEN);
                }
            }
            else
            {
              newlineflag=0;
              whitespaceflag=0;
            }
        }
    }
    sb_destroy(currword);
    free(buff);
    free(currword);
    current_node = freq_dist[file_index];
    while(current_node != 0){
      current_node -> frequency = current_node -> occurrences / total_words;
      current_node = current_node -> next;
    }
    freq_dist[file_index] = SelectionSort(freq_dist[file_index]);
    return freq_dist[file_index];
}

strbuf_t* read_word(strbuf_t* currword, char currletter,int *started)
{
    if(!isspace(currletter)&&(isalpha(currletter)||isdigit(currletter)||currletter=='-'))
    {
        *started = 1;
        sb_append(currword,tolower(currletter));
    }
    return currword;
}

void * analysis(void *anarguments)
{
   pthread_mutex_lock(&anlock);
   anargs* arguments=anarguments;
   filepair*parr=arguments->pairsarr;
   int start=arguments->pairbegin;
   int stop=arguments->pairend;


   for(int b=start;b<=stop;b++)
   {
       int commonwords=0;
       int f1index=parr[b].index1;
       int f2index=parr[b].index2;
       LLNode*ptr1=freq_dist[f1index];
       LLNodeMean*fmean=NULL;
       LLNodeMean*fmeanlast=NULL;
       //f1words & common words
       while(ptr1!=NULL)
       {
          char *firstword=ptr1->word->data;
          LLNode*ptr2=freq_dist[f2index];
          int found=0;
          while(ptr2!=NULL)
          {
             char*secondword=ptr2->word->data;
             //match found
             if(strncmp(firstword,secondword,strlen(firstword))==0)
             {
                 double mean=(ptr1->frequency+ptr2->frequency)*0.5;
                 //start mean list
                 if(fmean==NULL)
                 {
                     fmean=malloc(sizeof(LLNodeMean));
                     fmean->word=firstword;
                     fmean->frequency=mean;
                     fmean->next=NULL;
                     fmeanlast=fmean;
                 }
                 else
                 {
                     LLNodeMean*insert=malloc(sizeof(LLNodeMean));
                     insert->frequency=mean;
                     insert->word=firstword;
                     insert->next=NULL;
                     fmeanlast->next=insert;
                     fmeanlast=insert;
                 }
                 commonwords++;
                 found=1;
                 break;
             }
             ptr2=ptr2->next;

          }
          if(found==0)
          {
              double firstnotfoundmean=0.5*(ptr1->frequency);
              if(fmean==NULL)
                 {
                     fmean=malloc(sizeof(LLNodeMean));
                     fmean->word=firstword;
                     fmean->frequency=firstnotfoundmean;
                     fmean->next=NULL;
                     fmeanlast=fmean;
                 }
                 else
                 {
                     LLNodeMean*insert=malloc(sizeof(LLNodeMean));
                     insert->frequency=firstnotfoundmean;
                     insert->word=firstword;
                     insert->next=NULL;
                     fmeanlast->next=insert;
                     fmeanlast=insert;
                 }

           }
           ptr1=ptr1->next;


       }

       //f2wordsonly

       LLNode *ptr2f2only=freq_dist[f2index];

       while(ptr2f2only!=NULL)

       {
           char *f2word=ptr2f2only->word->data;
           LLNode *ptr1f2only=freq_dist[f1index];
           int found2=0;
           while(ptr1f2only!=NULL)
           {
              char *f1word=ptr1f2only->word->data;
              if(strncmp(f2word,f1word,strlen(f2word))==0)
              {
                  found2=1;
                  break;
              }
              ptr1f2only=ptr1f2only->next;
           }
           //if not found, then add
           if(found2==0)
           {
               double secondnotfoundmean=0.5*(ptr2f2only->frequency);
               if(fmean==NULL)
                 {
                     fmean=malloc(sizeof(LLNodeMean));
                     fmean->word=f2word;
                     fmean->frequency=secondnotfoundmean;
                     fmean->next=NULL;
                     fmeanlast=fmean;
                 }
                 else
                 {
                     LLNodeMean*insert=malloc(sizeof(LLNodeMean));
                     insert->frequency=secondnotfoundmean;
                     insert->word=f2word;
                     insert->next=NULL;
                     fmeanlast->next=insert;
                     fmeanlast=insert;
                 }
           }
           ptr2f2only=ptr2f2only->next;
       }

       //KLD distance f1 and mean
       double kld1=0.0;
       LLNode *p1=freq_dist[f1index];
       while(p1!=NULL)
       {
          //find word in mean list and compute kld
          LLNodeMean *mp1=fmean;
          while(mp1!=NULL)
          {
             if(strncmp(mp1->word,p1->word->data,strlen(mp1->word))==0)
             {
                 double freq1=p1->frequency;
                 double freqmean=mp1->frequency;
                 kld1=kld1+(freq1*log2(freq1/freqmean));
                 break;

             }
             mp1=mp1->next;
          }
          p1=p1->next;

       }

       //KLD distance f2 and mean
       double kld2=0.0;
       LLNode *p2=freq_dist[f2index];
       while(p2!=NULL)
       {
          //find word in mean list and compute kld
          LLNodeMean *mp2=fmean;
          while(mp2!=NULL)
          {
             if(strncmp(mp2->word,p2->word->data,strlen(mp2->word))==0)
             {
                 double freq2=p2->frequency;
                 double freqmean=mp2->frequency;
                 kld2=kld2+(freq2*log2(freq2/freqmean));
                 break;

             }
             mp2=mp2->next;
          }
          p2=p2->next;

       }
       //final JSD and put in results set
       double inroot=(0.5*kld1)+(0.5*kld2);
       double jsd=sqrt(inroot);
       finalresult *insertfr=malloc(sizeof(finalresult));
       insertfr->commonwords=commonwords;
       insertfr->JSD=jsd;
       insertfr->f1path=freq_dist[f1index]->name;
       insertfr->f2path=freq_dist[f2index]->name;
       results[resultsindex]=insertfr;
       resultsindex++;

       //freeing

      if(fmean!=NULL)
      {
          LLNodeMean*temp;

          while(fmean!=NULL)
          {
            temp=fmean;
            fmean=fmean->next;
            free(temp);

          }
      }
   }

   pthread_mutex_unlock(&anlock);
   return NULL;
}

//returns 0 if file, 1 if directory, 2 if file/directory not found
int isdirect(char *name)
{
    struct stat data;
    int err=stat(name,&data);
    if(err!=0)
    {
       perror(name);
       return 2;
    }
    return S_ISDIR(data.st_mode);
}

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



int main(int argc,char* argv[argc+1])
{
  //User Interface
  int direct_threads=1;
  int file_threads=1;
  int analysis_threads=1;
  char *suffix=".txt";

  int nondash_arg=0;
  int nondash_start=0;

  for(int i=1;i<argc;i++)
  {
      char *current_arg=argv[i];
      if(current_arg[0]=='-')
      {
         if(current_arg[1]=='d')
         {
             char temp[strlen(current_arg)-2];
             int index=0;
             for(int x=2;x<strlen(current_arg);x++)
             {
                temp[index]=current_arg[x];
                index++;
             }
             int temp_val=atoi(temp);
             if(temp_val<=0)
             {
                 fprintf(stderr,"%s","Invalid -d argument");
                 return EXIT_FAILURE;
             }
             direct_threads=temp_val;
         }
         else if(current_arg[1]=='f')
         {
            char temp[strlen(current_arg)-2];
             int index=0;
             for(int x=2;x<strlen(current_arg);x++)
             {
                temp[index]=current_arg[x];
                index++;
             }
             int temp_val=atoi(temp);
             if(temp_val<=0)
             {
                 fprintf(stderr,"%s","Invalid -f argument");
                 return EXIT_FAILURE;
             }
             file_threads=temp_val;
         }
         else if(current_arg[1]=='a')
         {
            char temp[strlen(current_arg)-2];
             int index=0;
             for(int x=2;x<strlen(current_arg);x++)
             {
                temp[index]=current_arg[x];
                index++;
             }
             int temp_val=atoi(temp);
             if(temp_val<=0)
             {
                 fprintf(stderr,"%s","Invalid -a argument");
                 return EXIT_FAILURE;
             }
             analysis_threads=temp_val;
         }
         else if(current_arg[1]=='s')
         {
             char temp[strlen(current_arg)-2];
             int index=0;
             for(int x=2;x<strlen(current_arg);x++)
             {
                temp[index]=current_arg[x];
                index++;
             }
             suffix=temp;
         }
         else
         {
             fprintf(stderr,"%s","Invalid dash argument");
             return EXIT_FAILURE;
         }
      }
      else
      {
          nondash_arg++;
          if(nondash_start==0)
          {
              nondash_start=i;
          }
      }
  }


  if(nondash_arg<1)
  {
     fprintf(stderr,"%s","Not enough file arguments provided");
     return EXIT_FAILURE;
  }

  //initial placement of directories and files in queues
   Queue directq;
   Queue fileq;
   QNode *frontd=NULL;
   QNode *lastd=NULL;
   int countd=0;

   QNode *frontf=NULL;
   QNode *lastf=NULL;
   int countf=0;

  for(int x=nondash_start;x<argc;x++)
  {
      int directorycheck=isdirect(argv[x]);
      //file
      if(directorycheck==0)
      {
         countf++;
         if(frontf==NULL)
         {
             frontf=malloc(sizeof(QNode));
             frontf->path=argv[x];
             frontf->next=NULL;
             lastf=frontf;
         }
        else
        {
            QNode*insertf=malloc(sizeof(QNode));
            insertf->path=argv[x];
            insertf->next=NULL;
            lastf->next=insertf;
        }
      }
      //direct
      else if(directorycheck==1)
      {
        countd++;
        if(frontd==NULL)
        {
            frontd=malloc(sizeof(QNode));
            frontd->path=argv[x];
            frontd->next=NULL;
            lastd=frontd;
        }
        else
        {
            QNode*insertd=malloc(sizeof(QNode));
            insertd->path=argv[x];
            insertd->next=NULL;
            lastd->next=insertd;
        }
      }
      else
      {
          fprintf(stderr,"%s"," File/Directory Argument ");
          fprintf(stderr,"%d",x-nondash_start+1);
          fprintf(stderr,"%s","is not a file or directory");
      }
  }
  qinit(&directq,countd,frontd);
  qinit(&fileq,countf,frontf);




  //analysis section
  int totalfiles;
  int totalpairs=(totalfiles*(totalfiles-1))/2;
  filepair *pairsarray=malloc(totalpairs*(sizeof(filepair)));
  int pairsarrindex=0;
  for(int i=0;i<totalfiles;i++)
  {
      for(int j=i+1;j<totalfiles;j++)
      {
          filepair insert={i,j};
          pairsarray[pairsarrindex]=insert;
          pairsarrindex++;
      }
  }
  int pairsperthread=totalpairs/analysis_threads;
  int remainder=totalpairs%analysis_threads;
  int firstpairnum=pairsperthread+remainder;

  pthread_t *tids;
  tids = malloc(analysis_threads * sizeof(pthread_t));
  anargs *args;
  args = malloc(analysis_threads * sizeof(anargs));

  int begin=0;
  int end=firstpairnum-1;

  if (pthread_mutex_init(&anlock, NULL) != 0) {
        fprintf(stderr,"%s","\n mutex init has failed\n");
        free(pairsarray);
        free(args);
        free(tids);
        return EXIT_FAILURE;
    }
    results=malloc(totalpairs*(sizeof(finalresult)));

  for(int a=0;a<analysis_threads;a++)
  {
      args[a].pairsarr=pairsarray;
      args[a].pairbegin=begin;
      args[a].pairend=end;
      int err=pthread_create(&tids[a], NULL, analysis, &args[a]);
      if(err!=0)
      {
         fprintf(stderr,"%s","pthread_create failed");
         free(pairsarray);
         free(args);
         free(tids);
         return EXIT_FAILURE;
      }
      begin=end+1;
      end=end+pairsperthread;
  }
  for (int i = 0; i < analysis_threads; ++i) {
		int errjoin=pthread_join(tids[i], NULL);
        if(errjoin!=0)
        {
            fprintf(stderr,"%s","pthread_join failed");
            free(pairsarray);
            free(args);
            free(tids);
            return EXIT_FAILURE;
        }

    }
 //sorting and printing final results by commonwords
  //qsort(results,totalpairs,sizeof(finalresult),cmpfnc);

  for(int i=0;i<totalpairs;i++)
  {
      printf("%f, %s, %s",results[i]->JSD,results[i]->f1path,results[i]->f2path);
      printf("\n");
  }


  pthread_mutex_destroy(&anlock);
  free(pairsarray);
  free(args);
  free(tids);

  //free the resultsarr
  for(int i=0;i<totalpairs;i++)
  {
      free(results[i]);
  }
  free(results);

  /*
  Temporary face holder code to test tokenize and create frequency distribution
  - needs to do necessary check for valid files and subdirectories
  - needs to feed file descriptors to tokenize method
  - num_files: need count of how many files we are working with
  */
  /*
  printf("%d, %d, %d, %s\n",analysis_threads,file_threads,direct_threads,suffix);
  int num_files = 1;
  freq_dist = malloc(sizeof(LLNode*)*num_files); //[num_files];
  freq_dist = LLNodeInit(freq_dist,num_files);
  // temporary file input
  int file;
  int file_index = 0;
  file = open(argv[2],O_RDONLY);
  freq_dist[file_index] = tokenize(file,argv[2],file_index,freq_dist);
  LLPrint(freq_dist,num_files);
  FreeLL(freq_dist,num_files);
  free(freq_dist);
  */

  return EXIT_SUCCESS;
}
