#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>


int read_bytes(int fd, void* buf, int n) {
    int a = 0;
    int ret = 0;
    int bytes = 0;
    while(bytes != n){
        ret = read(fd,buf, n);
        
        if (ret == -1) {
            if (errno == EINTR || errno == EAGAIN)  {
                continue; 
            }
            else {
                perror("Error while reading!");
    		a = -2;
            }
        }
        if(ret == 0) {
            perror("Error! End of file You enter wrong value.");
    	    a = -3;
        }
       bytes+= ret;
       buf+= ret; 
    }
    return a;
}

int main(int argc , char* argv[])
{
    off_t offsets[100];
    off_t line_length[100];
    int line_number = 0;
    int need_line = 1;
    int fd, selectResult;
    int fd_max = 0;
    fd_set terminal;
	struct timeval timeout;

    if((fd = open(argv[1],O_RDONLY)) == -1) {
        perror("File not open\n");
        return 1;
    }  

    int file_size = lseek(fd, 0L, 2);
    lseek(fd,0L,0);
    char myfile[file_size];

    read_bytes(fd,myfile, file_size);
    for(int i = 0; i < file_size; i++) {

        if( myfile[i] == '\n') {
             
            if(line_number == 0) {
                offsets[line_number] = 0;
                line_length[line_number] = i+1;
            }
            else {
                offsets[line_number] = offsets[line_number - 1] + line_length[line_number - 1];
                line_length[line_number] = i+1 - offsets[line_number] ;
            }
            line_number++;
        }
    }
   
     printf("count of lines : %d\n ", line_number);

    while(!0){
        
         FD_SET(0, &terminal);
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        printf("You have only five second to enter line\n");
	printf("What line you need?\n");
        selectResult = select(fd_max+1, &terminal, NULL, NULL, &timeout);
	
        if (selectResult  == -1) { 
            perror("select()");
            if(close(fd) == -1) perror("Error while closing");
    		exit(0);
        }

        if (selectResult != 0) { 
            scanf("%d", &need_line);
            if(need_line == 0) {
                if(close(fd) == -1) perror("Error while closing");
                return 0;
            }

        if(need_line > line_number || need_line < 0) {
            perror("Error! We haven't this line!");
            if(close(fd) == -1) perror("Error while closing");
    	    exit(0);
        }

        for(int i = offsets[need_line -1]; i < offsets[need_line -1] + line_length[need_line - 1]; i++)
            printf("%c", myfile[i]);
    }
    	if( selectResult == 0){
            for(int i = 0; i < file_size; i++)
            printf("%c", myfile[i]);
            if(close(fd) == -1) perror("Error while closing");
            return 0;
        }
    }
}


