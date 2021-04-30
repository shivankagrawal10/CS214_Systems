#include "server.h"
#define BACKLOG 5


//global vars
char *portnum;
pthread_mutex_t connlock;
LLNode* front=NULL;



void *respondwork(void *arg)
{
    struct connection *c = (struct connection *) arg;
    int file=c->fd;
    FILE *fin = fdopen(dup(file), "r");  // copy socket & open in read mode
    FILE *fout = fdopen(file, "w")  // open in write mode
    char code[3];
    
    int org=0;
    
    //new message instr check
    while(fscanf(fin, "%s\n", code)>0)
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
               
           
           break;
       }

    
    //subsequent message 
    //lock during message receiving  
    pthread_mutex_lock(&connlock);
    
    //len
    int inputlen;
    int err2=fscanf(fin, "%d\n", &inputlen);
    int fail=0;
    //wrong
    if(err2<=0)
    {
        //stop
        fprintf(fout,"ERR\nBAD\n");
        fflush(fout);
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
           if(c==EOF)
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
           key[index]=c;

       }
       if(fail==1)
       {
           //close conn and term. thread
           printf("Closing connection");
           fclose(fin);
           fclose(fout);
           free(c);
           return NULL;
       }
       
       //search through LL for key
       LLNode *ptr=front;
       int found=0;

       while(ptr!=NULL)
       {
          if((strlen(ptr->key->data)==inputlen-1) && (strncmp(ptr->key->data,key,inputlen-1)==0))
          {
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
           char *value=ptr->value;
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
           if(c==EOF)
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
           key[index]=c;
          
       }
       
       //search through LL for key
       LLNode *ptr=front;
       LLNode *prev=NULL;
       int found=0;

       while(ptr!=NULL)
       {
          if((strlen(ptr->key->data)==inputlen-1) && (strncmp(ptr->key->data,key,inputlen-1)==0))
          {
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
           char *value=ptr->value;
           int bytenum=strlen(value)+1;
           fprintf(fout,"OKD\n%d\n%s\n",bytenum,value);
           fflush(fout);

           //delete it
           prev->next=ptr->next;
           sb_destroy(ptr->key);
           free(ptr->key);
           sb_destroy(ptr->value);
           free(ptr->value);
           free(ptr);
           
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
           if(c==EOF)
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

       //insertat the end
       LLNodeInit(front,key,value);
       SelectionSort(front);
       
       fprintf(fout,"OKS\n");
       fflush(fout);
       
       

    }


    pthread_mutex_unlock(&connlock);
    sleep(2);
    }
    return NULL;
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
     portnum=malloc(strlen(argv[1])+1);
     portnum=argv[1];

    struct addrinfo hint, *info_list, *info;
    struct connection *con;
    int error, sfd;
    pthread_t tid;
    if(pthread_mutex_init(&connlock,NULL)!=0)
    {
        fprintf(stderr,"%s","\n mutex init has failed\n");
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
        return -1;
    }

    freeaddrinfo(info_list);


    printf("Waiting for a connection\n");
    for (;;) {
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

    // never reach here
    return 0;





}