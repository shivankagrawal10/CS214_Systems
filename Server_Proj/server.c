#include "server.h"
#define BACKLOG 5


//global vars
char *portnum;
pthread_mutex_t connlock;
LLNode* front=NULL;
struct connection *con;
int serverup = 0;

//LL methods

LLNodePTR LLNodeInit(char* key,char *value)
{
  LLNodePTR temp = malloc(sizeof(LLNode));
  temp->key = malloc(sizeof(strbuf_t)); //0;
  sb_init(temp->key,10);
  sb_concat(temp->key,key);
  temp->value = malloc(sizeof(strbuf_t)); //0;
  sb_init(temp->value,10);
  sb_concat(temp->value,value);
  if(front==NULL)
  {
      front=temp;
      front->next=NULL;
  }
  else
  {
     temp->next = front;
     front=temp;
  }
  return temp;
}

void LLPrint(LLNodePTR front)
{
  while(front!=0)
  {
    //sb_print(front->word);
    printf("%s (%zu) -> %s (%zu), ",front->key->data,front->key->used,front->value->data,front->value->used);
    front = front->next;
  }
  printf("\n");
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
      if(strcmp(min->key->data,temp->key->data)>0)
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

void FreeLL()
{
    LLNodePTR temp = front;
    LLNodePTR next = 0;
    while (temp != 0)
    {
      next = temp -> next;
      free(temp->key->data);
      free(temp->key);
      free(temp->value->data);
      free(temp->value);
      free(temp);
      temp = next;
    }
  
}


void *respondwork(void *arg)
{
    printf("New connection\n");
    struct connection *c = (struct connection *) arg;
    int file=c->fd;
    FILE *fin = fdopen(dup(file), "r");  // copy socket & open in read mode
    FILE *fout = fdopen(file, "w");  // open in write mode
    char*code=malloc(4);
    
    int org=0;
    
    //new message instr check
    while(fscanf(fin, "%3s", code)>0)
    {
       if(code[0]=='G' && code[1]=='E' && code[2]=='T')
       {
           org=1;
       }
       else if(code[0]=='D' && code[1]=='E' && code[2]=='L')
       {
           org=2;
       }
       else if(code[0]=='S' && code[1]=='E' && code[2]=='T')
       {
           org=3;
       }
       //wrong
       else
       {
           fprintf(fout,"ERR\nBAD\n");
           fflush(fout);
           
           printf("Closing connection");
           fclose(fin);
           fclose(fout);
           free(code);
           
           return NULL;  
           
           
       }
       char nl=getc(fin);
    if(nl!='\n')
    {
        //stop
        fprintf(fout,"ERR\nBAD\n");
        fflush(fout);
        printf("Closing connection");
        fclose(fin);
        fclose(fout);
        free(c);
	    free(code);
        pthread_mutex_unlock(&connlock);
        return NULL;
    }

   printf("\n%s, %d\n",code,org); 
    //subsequent message 
    //lock during message receiving  
    pthread_mutex_lock(&connlock);
    
    //len
    int inputlen;
    int err2=fscanf(fin, "%d", &inputlen);
    int fail=0;
    printf("%d\n",inputlen);
    //wrong
    if(err2<=0)
    {
        //stop
        fprintf(fout,"ERR\nBAD\n");
        fflush(fout);
        printf("Closing connection");
        fclose(fin);
        fclose(fout);
        free(c);
	    free(code);
        pthread_mutex_unlock(&connlock);
        return NULL;
    }
    char nl1=getc(fin);
    if(nl1!='\n')
    {
        //stop
        fprintf(fout,"ERR\nBAD\n");
        fflush(fout);
        printf("Closing connection");
        fclose(fin);
        fclose(fout);
        free(c);
	    free(code);
        pthread_mutex_unlock(&connlock);
        return NULL;
    }
    //GET
    if(org==1)
    {
       char *key=malloc(inputlen+1);
       int index=0;
       for(int i=0;i<inputlen;i++)
       {
           char c;
           c=getc(fin);
           //too short
           if(c=='\0' || c==EOF)
           {
               fprintf(fout,"ERR\nLEN\n");
               fflush(fout);
               
               fail=1;
               break;
           }
           //too short
           if(c=='\n' && i!=inputlen-1)
           {
               fprintf(fout,"ERR\nLEN\n");
               fflush(fout);
               
               fail=1;
               break;
               
           }
           //too long
           if(c!='\n' && i==inputlen-1)
           {
                fprintf(fout,"ERR\nLEN\n");
                fflush(fout);
               
                fail=1;
               break;
           }
           if(c=='\n' && i==inputlen-1)
           {
                continue;
           }
           key[index]=c;
           index++;

       }
       
       
       if(fail==1)
       {
           //close conn and term. thread
           printf("Closing connection");
           fclose(fin);
           fclose(fout);
           free(c);
           free(key);
	       free(code);
           pthread_mutex_unlock(&connlock);
           return NULL;
       }
       
       //search through LL for key
       LLNode *ptr=front;
       int found=0;

       while(ptr!=NULL)
       {
          if((strlen(ptr->key->data)==inputlen-1) && (strncmp(ptr->key->data,key,inputlen-1)==0))
          {
              found=1;
              break;
          }
          ptr=ptr->next;
       }
       //output
       if(found==0)
       {
            fprintf(fout,"KNF\n");
            fflush(fout);
       }
       else
       {
           char *value=ptr->value->data;
           int bytenum=strlen(value)+1;
           fprintf(fout,"OKG\n%d\n%s\n",bytenum,value);
           fflush(fout);
           
       }
       free(key);

    }
    //DEL
    else if(org==2)
    {
       char *key=malloc(inputlen+1);
       int index=0;
       for(int i=0;i<inputlen;i++)
       {
           char c;
           c=getc(fin);
           //too short
           if(c=='\0' || c==EOF)
           {
               fprintf(fout,"ERR\nLEN\n");
               fflush(fout);
               
               fail=1;
               break;
           }
           //too short
           if(c=='\n' && i!=inputlen-1)
           {
               fprintf(fout,"ERR\nLEN\n");
               fflush(fout);
               
               fail=1;
               break;
               
           }
           //too long
           if(c!='\n' && i==inputlen-1)
           {
                fprintf(fout,"ERR\nLEN\n");
                fflush(fout);
               
                fail=1;
               break;
           }
           if(c=='\n' && i==inputlen-1)
           {
                continue;
           }
           key[index]=c;
           index++;
          
       }
       //check if is actually empty
       
       if(fail==1)
       {
           //close conn and term. thread
           printf("Closing connection");
           fclose(fin);
           fclose(fout);
           free(c);
           free(key);
           free(code);
           pthread_mutex_unlock(&connlock);
           return NULL;
       }
       
       //search through LL for key
       LLNode *ptr=front;
       LLNode *prev=NULL;
       int found=0;

       while(ptr!=NULL)
       {
          if((strlen(ptr->key->data)==inputlen-1) && (strncmp(ptr->key->data,key,inputlen-1)==0))
          {
              found=1;
              break;
          }
          prev=ptr;
          ptr=ptr->next;
       }
       //output
       if(found==0)
       {
            fprintf(fout,"KNF\n");
            fflush(fout);
       }
       else
       {
           char *value=ptr->value->data;
           int bytenum=strlen(value)+1;
           fprintf(fout,"OKD\n%d\n%s\n",bytenum,value);
           fflush(fout);

           //delete it
           if(prev==NULL && ptr->next==NULL)
           {
               free(ptr->key->data);
               free(ptr->key);
               free(ptr->value->data);
               free(ptr->value);
               free(ptr);
               front=NULL;
           }
           else if(prev==NULL && ptr->next!=NULL)
           {
               front=ptr->next;
               free(ptr->key->data);
               free(ptr->key);
               free(ptr->value->data);
               free(ptr->value);
               free(ptr);

           }
           else
           {
               prev->next=ptr->next;
               free(ptr->key->data);
               free(ptr->key);
               free(ptr->value->data);
               free(ptr->value);
               free(ptr);
           }
           
           
       }
       free(key);

       




    }
    //SET
    else if(org==3)
    {
       char *key=malloc(inputlen+1);
       char *value=malloc(inputlen+1);

       int indexk=0;
       int indexv=0;

       int iskey=1;
       for(int i=0;i<inputlen;i++)
       {
          char c;
          c=getc(fin);
          
          //key
          //too short
           if(c=='\0' || c==EOF)
           {
               fprintf(fout,"ERR\nLEN\n");
               fflush(fout);
               
               fail=1;
               break;
           }
           
           if(c=='\n' && i!=inputlen-1)
           {
               //too short
               if(iskey==0)
               {
                  fprintf(fout,"ERR\nLEN\n");
                  fflush(fout);
               
                  fail=1;
                  break;
               }
               //switch
               else
               {
                   iskey=0;
                                    
                   continue;

               }
               
               
           }
           //too long-very end
           if(c!='\n' && i==inputlen-1)
           {
                fprintf(fout,"ERR\nLEN\n");
                fflush(fout);
               
                fail=1;
               break;
           }
           //good-dont do anything
           if(c=='\n' && i==inputlen-1)
           {
                continue;
           }

           if(iskey==1)
           {
             key[indexk]=c;
             indexk++;
           }
           else
           {
             value[indexv]=c;
             indexv++;
           }


       }
       key[indexk]='\0'; 
       value[indexv]='\0';

       //check if is actually empty
       int allwhite=2;
       for(int i=0;i<strlen(key);i++)
       {
           if(isspace(key[i])==0)
           {
               allwhite--;
               break;
           }

       }
       for(int i=0;i<strlen(value);i++)
       {
           if(isspace(value[i])==0)
           {
               allwhite--;
               break;
           }

       }

       if(allwhite>0)
       {
           fprintf(fout,"ERR\nBAD\n");
           fflush(fout);
           printf("Closing connection");
           fclose(fin);
           fclose(fout);
           free(c);
           free(key);
           free(value);
           free(code);
           pthread_mutex_unlock(&connlock);
           return NULL;
       }


       if(fail==1)
       {
           //close conn and term. thread
           printf("Closing connection");
           fclose(fin);
           fclose(fout);
           free(c);
           free(key);
           free(value);
           free(code);
           pthread_mutex_unlock(&connlock);
           return NULL;
       }

       //insertat the end
      

       LLNode *ptr=front;
       int found=0;

       while(ptr!=NULL)
       {
          if((strlen(ptr->key->data)==strlen(key)) && (strncmp(ptr->key->data,key,strlen(key))==0))
          {
              found=1;
              break;
          }
          ptr=ptr->next;
       }
       //output
       if(found==0)
       {
            LLNodeInit(key,value);
       }
       else
       {
           free(ptr->value->data);
           free(ptr->value);
           ptr->value = malloc(sizeof(strbuf_t)); //0;
           sb_init(ptr->value,10);
           sb_concat(ptr->value,value);
           
       }



       
       //SelectionSort(front);
       
       fprintf(fout,"OKS\n");
       fflush(fout);

       

    }


    pthread_mutex_unlock(&connlock);
    sleep(2);
    }
    return NULL;
}

void siginthandler(int sig)
{
    FreeLL(front);
    //free(con);
    serverup = 1;
    printf("Freeing LL and closing server\n");
    //signal(SIGINT,siginthandler);
    //return EXIT_SUCCESS;
    exit(0);
}

int main(int argc,char* argv[argc+1])
{
     if(argc!=2)
     {
        fprintf(stderr,"%s","Invalid number of arguments");
        return EXIT_FAILURE;
     }
     int portcheck=atoi(argv[1]);
     if(portcheck<=0)
     {
        fprintf(stderr,"%s","Please make sure the port number is an integer");
        return EXIT_FAILURE;
     }
     
     portnum=argv[1];

    struct addrinfo hint, *info_list, *info;

    int error, sfd;
    pthread_t tid;
    if(pthread_mutex_init(&connlock,NULL)!=0)
    {
        fprintf(stderr,"%s","mutex init has failed\n");
        return EXIT_FAILURE;
    }

    // initialize hints
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    	// setting AI_PASSIVE means that we want to create a listening socket

    // get socket and address info for listening port
    // - for a listening socket, give NULL as the host name (because the socket is on
    //   the local host)
    error = getaddrinfo(NULL, portnum, &hint, &info_list);
    if (error != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        return -1;
    }
   
    // attempt to create socket
    for (info = info_list; info != NULL; info = info->ai_next) {
        sfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
        
        // if we couldn't create the socket, try the next method
        if (sfd == -1) {
            continue;
        }

        // if we were able to create the socket, try to set it up for
        // incoming connections;
        // 
        // note that this requires two steps:
        // - bind associates the socket with the specified port on the local host
        // - listen sets up a queue for incoming connections and allows us to use accept
        if ((bind(sfd, info->ai_addr, info->ai_addrlen) == 0) &&
            (listen(sfd, BACKLOG) == 0)) {
            break;
        }

        // unable to set it up, so try the next method
        close(sfd);
    }

    if (info == NULL) {
        // we reached the end of result without successfuly binding a socket
        fprintf(stderr, "Could not bind\n");
        freeaddrinfo(info_list);
        
        pthread_mutex_destroy(&connlock);
        return EXIT_FAILURE;
    }

    freeaddrinfo(info_list);

    //signal(SIGINT,siginthandler);
    printf("Waiting for a connection\n");
    while(serverup == 0){
    	// create argument struct for child thread
        con = malloc(sizeof(struct connection));
        con->addr_len = sizeof(struct sockaddr_storage);
        	// addr_len is a read/write parameter to accept
        	// we set the initial value, saying how much space is available
        	// after the call to accept, this field will contain the actual address length
        
        // wait for an incoming connection
        con->fd = accept(sfd, (struct sockaddr *) &con->addr, &con->addr_len);
        	// we provide
        	// sfd - the listening socket
        	// &con->addr - a location to write the address of the remote host
        	// &con->addr_len - a location to write the length of the address
        	//
        	// accept will block until a remote host tries to connect
        	// it returns a new socket that can be used to communicate with the remote
        	// host, and writes the address of the remote hist into the provided location
        
        // if we got back -1, it means something went wrong
        if (con->fd == -1) {
            perror("accept");
	    continue;

        }

		// spin off a worker thread to handle the remote connection
        error = pthread_create(&tid, NULL, respondwork, con);

		// if we couldn't spin off the thread, clean up and wait for another connection
        if (error != 0) {
            fprintf(stderr, "Unable to create thread: %d\n", error);
            close(con->fd);
            free(con);
            continue;
        }

		// otherwise, detach the thread and wait for the next connection request
        pthread_detach(tid);
    }
   
 
    free(con);
    // never reach here
    return 0;





   
}
