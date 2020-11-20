//
// Created by Никита on 08.11.2020.
//
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
    off_t offsets[100],line_length[100];
    int fd,file_size,selectResult;
    int line_number = 0;
    int need_line = 1;
    char* buf;
    int max_fd=0;
    fd_set terminal;
    struct timeval timeout;

    if((fd = open(argv[1],O_RDONLY)) == -1) {
        perror("File not open\n");
        return 1;
    }


    file_size = lseek(fd, 0L, 2);
    lseek(fd,0L,0);

    buf = mmap(0,file_size,PROT_READ,MAP_SHARED,fd,0);
        
    for(int i = 0; i < file_size; i++) {

        if( buf[i] == '\n') {

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

        selectResult = select(max_fd+1, &terminal, NULL, NULL, &timeout);

        if (selectResult  == -1) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
           else perror("select()");
            if(close(fd) == -1) perror("Error while closing");
            return -1;
        }

        if (selectResult != 0) {
           if (FD_ISSET(fd, &terminal) != 0) {
            scanf("%d", &need_line);

            if(need_line == 0) {
                if(close(fd) == -1) perror("Error while closing");
                if(munmap(buf,file_size) == -1) perror("Error while munmap");
                return 0;
            }

            if(need_line > line_number || need_line < 0) {
                perror("Error! We haven't this line!");
                continue;
            }

            for(int i = offsets[need_line -1]; i < offsets[need_line -1] + line_length[need_line - 1]; i++)
                printf("%c", buf[i]);
            }  else {
            	   printf("fd absent in fd_set");
            	   return -1;
        	 }
        }

        if( selectResult == 0){
            for(int i = 0; i < file_size; i++)
                printf("%c", buf[i]);
            if(close(fd) == -1) perror("Error while closing");
            if(munmap(buf,file_size) == -1) perror("Error while munmap");
            return 0;
        }
    }
     if (munmap(buf, file_size)==-1) {
        close(fd);
        if (close(fd) == -1) {
            fprintf(stderr, "Error at close");
            return -1;
        }
        fprintf(stderr, "Error un-mmapping the file");
        return -1;
     } 
}


