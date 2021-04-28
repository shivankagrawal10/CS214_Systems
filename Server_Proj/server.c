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
    
    //new message instr check
    while(fscanf(fin, "%s\n", code)!=0)
    {
       if(code[0]=='G' && code[1]=='E' && code[2]=='T')
       {

       }
       else if(code[0]=='D' && code[1]=='E' && code[2]=='L')
       {

       }
       else if(code[0]=='S' && code[1]=='E' && code[2]=='T')
       {

       }
       //wrong
       else()
       {

       }

    
    //subsequent message 
    //lock during message receiving  
    pthread_mutex_lock(&connlock);
    


    pthread_mutex_unlock(&connlock);
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