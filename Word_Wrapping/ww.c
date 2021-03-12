#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "ww.h"


#define BUFFSIZE 2

int wrap(int fd_read, size_t len, int fd_write)
{
    char* buff= malloc(BUFFSIZE);
    strbuf_t* currword= malloc(sizeof(strbuf_t));
    sb_init(currword,len);
    int num_read = 1;
    int offset = 0;
    int outcount=0;
    int newlineflag=0;
    int started=0;
    int isfirstword=1;
    int whitespaceflag=0;
    int fail=0;
    while(num_read>0)
    {
        // reads char from file into buffer (size specified by macro)
	    num_read = pread(fd_read,buff,BUFFSIZE,offset);
	    offset += num_read;
	    // Iterating through buffer
	    for(int i=0; i<num_read; i++)
	    {
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
                    //writes word to output when only encountering 1 space character
                    if((whitespaceflag==1 || newlineflag==1))
                    {
                        write_word(fd_write,currword,&outcount,len,newlineflag,started,isfirstword, &fail,0);
                    }
                    isfirstword=0;
                    sb_destroy(currword);
                    sb_init(currword,len);
                }
            }
            //normal char
            else
            {
                //checks for and writes paragraph breaks to output file
                if(newlineflag>=2)
                {
                    write_word(fd_write,currword,&outcount,len,newlineflag,started,isfirstword, &fail,1);
                }
                newlineflag=0;
                whitespaceflag=0;
            }
        }
    }
    //writes last word to output file and frees memory
    newlineflag=0;
    write_word(fd_write,currword,&outcount,len,newlineflag,started,isfirstword, &fail,0);
    write(fd_write,"\n",1);
    sb_destroy(currword);
    free(buff);
    free(currword);
    return fail;
}

strbuf_t* read_word(strbuf_t* currword, char currletter, int *started)
{
    if(!isspace(currletter))
    {
        *started=1;
        sb_append(currword,currletter);
    }
    return currword;
}

//
void write_word(int fd_write, strbuf_t* currword, int *outcount, size_t limit, int newlineflag, int started, int isfirstword, int *fail, int elsecall)
{    
    int sizewritten=*outcount;
    int currsize=currword->used;
    int added=1;
    //paragraph breaks
    if(newlineflag>=2 && elsecall==1)
    {
        char parabuf[2];
        parabuf[0]='\n';    
        parabuf[1]='\n';
        write(fd_write,parabuf,2);
        *outcount=0;
        return;
    }
    if(sizewritten==0 || isfirstword==1 || currword->used==0)
    {
        added=0;
    }
    //word fits on line
    if(sizewritten+currsize+added<=limit)
    {
        if(sizewritten!=0 && isfirstword==0 && currword->used!=0)
        {
            //putting prior space
            char tempspace[1];
            tempspace[0]=' ';
            write(fd_write,tempspace,1);
        }
        //writing word
        char *tempword=currword->data;
        write(fd_write,tempword,currsize);
        *outcount=sizewritten+currsize+added;
    }
    //doesn't fit
    else
    {
        //skip line
        if(newlineflag!=2 && sizewritten!=0 && currsize!=0)
        {
            char tempnewline[1];
            tempnewline[0]='\n';
            write(fd_write,tempnewline,1);
        }
        // if word is bigger than line length - writes word in own line and marks failure flag
        if(currsize>limit)
        {
            *fail=1;
            char *tempword=currword->data;
            write(fd_write,tempword,currsize);
            
            *outcount=currsize;
            return;
        }
        //writing word
        char *tempword=currword->data;
        write(fd_write,tempword,currsize);
        *outcount=currsize;
    }
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

int main(int argc,char* argv[argc+1])
{
    //exits if 1 or 2 arguments not given
    int num_arg = argc-1;
    if(num_arg!=1 && num_arg!=2)
    {
        return EXIT_FAILURE;
    }

    //exits if line length is not a number or less than equal to 0
    int line_len = atoi(argv[1]);
    if(line_len<=0)
    {
        printf("Width argument invalid\n");
        return EXIT_FAILURE;
    }

    //If 1 argument provided - Program reads from stdin 
    if(num_arg==1)
    {
        int fd = 0;
        int retval=wrap(fd,line_len,1);
            if(retval==1)
            {
                return EXIT_FAILURE;
            }
    }

    //If 2 arguments provided - Program checks for files/directory
    else
    {
        //checks to see if valid file/directory
        int argtype=isdirect(argv[2]);
        
        //exits program if file/directory not found
        if(argtype==2)
        {
            return EXIT_FAILURE;
        }

        //normal file
        if(argtype==0)
        {
            int file;
            file = open(argv[2],O_RDONLY);
            if(file==-1)
            {
                perror("Error: ");
                return EXIT_FAILURE;
            }
            
            int retval=wrap(file,line_len,1);

            if(retval==1)
            {
                return EXIT_FAILURE;
            }
            close(file);
        }
        //directory
        else if(argtype==1)
        {
            strbuf_t* dir= malloc(sizeof(strbuf_t));
            sb_init(dir,10);
            sb_concat(dir,argv[2]);
            DIR * directptr=opendir(dir->data);

            if(directptr==NULL)
            {
                perror("Error: ");
                return EXIT_FAILURE;
            }
            struct dirent *de;
        
            while ((de = readdir(directptr))) 
            { 
                char type=de->d_type;
                if(type==DT_REG)
                {
                    //open up file
                    char *inputfile=de->d_name;
                    strbuf_t* curr_read= malloc(sizeof(strbuf_t));
                    strbuf_t* curr_write= malloc(sizeof(strbuf_t));
                    char* addon = "wrap.";
                    sb_init(curr_read,10);
                    sb_init(curr_write,10);
                    sb_concat(curr_write,dir->data);
                    sb_concat(curr_read,dir->data);
                    sb_concat(curr_write,addon);
                    if(*inputfile=='.'|| strstr(inputfile,addon)!=NULL)
                    {
                        continue;
                    }
                    sb_concat(curr_write,inputfile);
                    sb_concat(curr_read,inputfile);
                    int fd_read=open(curr_read->data,O_RDONLY);
                    //create new file
                    //addon = strcat(addon,inputfile);
                    int fd_write=open(curr_write->data,O_RDWR|O_CREAT|O_TRUNC,0777);
                    //call write on it
                    wrap(fd_read,line_len,fd_write);
                    close(fd_read);
                    close(fd_write);
                    sb_destroy(curr_write);
                    sb_destroy(curr_read);
                    free(curr_write);
                    free(curr_read);
                }
                //else ignore subdirectories
            }
            sb_destroy(dir);
            free(dir);
            closedir(directptr);
        }
    }
  return EXIT_SUCCESS;
}
