#include"QNode.h"

int qinit(Queue *Q,int num,QNode*front,QNode*last)
{
    Q->count=0;
    Q->open=1;
    Q->front=front;
    Q->last = last;
    pthread_mutex_init(&Q->qlock, NULL);
    pthread_cond_init(&Q->read_ready, NULL);
    return 0;
}

void QEnqueue(char * path_name, char *suffix, int b_thread)
{
    int directorycheck=isdirect(path_name);
    //file
    Queue * Q=NULL;
    if(directorycheck==0)
    {
        if(strstr(path_name,suffix)!=0)
        {
          Q = fileq;
        }
        else {return;}
        //if(b_thread){}
    }
    else if (directorycheck == 1)
    {
        Q = directq;
    }
    //if(b_thread) pthread_mutex_lock(&Q->qlock);
    strbuf_t* item_path = malloc(sizeof(strbuf_t));
    sb_init(item_path,BUFFSIZE);
    sb_concat(item_path,path_name);
    if(Q->front==NULL)
    {
           Q->front=malloc(sizeof(QNode));
           Q->front->path=item_path;
           Q->front->next=NULL;
           Q->last=Q->front;
    }
    else
    {
           QNode *insert=malloc(sizeof(QNode));
           insert->path=item_path;
           insert->next=NULL;
           Q->last->next=insert;
           Q->last = Q->last->next;
    }

    ++Q -> count;
    if(b_thread)
    {
      //pthread_mutex_unlock(&Q->qlock);
      pthread_cond_signal(&Q->read_ready);
    }
    /*
    int directorycheck=isdirect(path_name);
    //direct
    if(directorycheck==0)
    {
    //countf++;
        if(frontf==NULL)
        {
                frontf=malloc(sizeof(QNode));
                frontf->path=path_name;
                frontf->next=NULL;
                lastf=frontf;
        }
       else
       {
                QNode*insertf=malloc(sizeof(QNode));
                insertf->path=path_name;
                insertf->next=NULL;
                lastf->next=insertf;
       }
    }
    else if(directorycheck==1)
    {
         //countd++;
         if(frontd==NULL)
         {
                 frontd=malloc(sizeof(QNode));
                 frontd->path=path_name;
                 frontd->next=NULL;
                 lastd=frontd;
         }
        else
        {
                 QNode*insertd=malloc(sizeof(QNode));
                 insertd->path=path_name;
                 insertd->next=NULL;
                 lastd->next=insertd;
        }
    }
    else
    {
        fprintf(stderr,"%s"," File/Directory Argument ");
        //fprintf(stderr,"%d",x-nondash_start+1);
        fprintf(stderr,"%s","is not a file or directory");
    }
    */
}

void *DirQDequeue(void *arg)
{
  pthread_mutex_lock(&directq->qlock);
  while(directq->count == 0 && directq->open != 0)
  {
      pthread_cond_wait(&directq->read_ready,&directq->qlock);
  }
  //exits when thread receives signal to proceed but still nothing in queue
  if(directq->count == 0)
  {
      pthread_mutex_unlock(&directq->qlock);
      return NULL;
  }
  ++activedthreads;
  QNode *temp = directq -> front;
  DirectorySearch(temp);
  directq->front = directq->front->next;
  --directq->count;
  sb_destroy(temp->path);
  free(temp->path);
  free(temp);
  pthread_mutex_unlock(&directq->qlock);
  --activedthreads;
  return NULL;
}

int DirectorySearch(QNode *front)
{
  strbuf_t* dir= malloc(sizeof(strbuf_t));
  sb_init(dir,10);
  sb_concat(dir,front->path->data);
  if(dir->data[dir->used-1] != '/')
  {
      sb_concat(dir,"/");
  }
  DIR * directptr=opendir(dir->data);

  if(directptr==NULL)
  {
      perror("Error: ");
      return EXIT_FAILURE;
  }
  struct dirent *de;
  //int count = 0;
  while ((de = readdir(directptr)))
  {

      char *inputfile=de->d_name;
      if(strcmp(inputfile,".")==0 || strcmp(inputfile,"..")==0) {continue;}
      strbuf_t* item_path = malloc(sizeof(strbuf_t));
      sb_init(item_path,10);
      sb_concat(item_path,dir->data);
      sb_concat(item_path,inputfile);
      //sb_print(item_path);
      QEnqueue(item_path->data,suffix,1);
      sb_destroy(item_path);
      free(item_path);
  }
  sb_destroy(dir);
  free(dir);
  closedir(directptr);
  return EXIT_SUCCESS;
}

void QPrint(Queue *Que)
{
    QNode *front = Que -> front;
    while(front!=0)
    {
      printf("|%s| - ",front -> path -> data);
      front = front->next;
    }
    printf("\n");
    //printf("size: %d\n",Que->count);
}


void QClose(Queue *Q)
{
    pthread_mutex_lock(&Q->qlock);
    Q->open = 0;
    pthread_cond_broadcast(&Q->read_ready);
    pthread_mutex_unlock(&Q->qlock);
    //pthread_mutex_destroy(&Q->qlock);
    //pthread_cond_destroy(&Q->read_ready);
}

void QFree()
{
  QNode *temp = fileq->front;;
  while(fileq->front!=0)
  {
    temp = fileq->front;
    fileq->front = fileq->front->next;
    sb_destroy(temp->path);
    free(temp->path);
    free(temp);
    --fileq->count;
  }
}

void FDequeue()
{
   Queue *Q=fileq;
   QNode *todeq=Q->front;
   char *path=todeq->path->data;
   int filedes=open(path,O_RDONLY);
   if(filedes==-1)
   {
      fail=1;
      return;
   }
   tokenize(filedes,path);
   --Q -> count;
   //free the node
   QNode *temp=Q->front;
   Q->front=Q->front->next;
   free(temp);
}