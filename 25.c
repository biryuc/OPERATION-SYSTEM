
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <wait.h>
#include <fcntl.h>
#define   MSGSIZE   20

int main(int argc, char **argv)
{
    int fd[2]; pid_t pid;
    static char msgout[MSGSIZE]="Hello,world";
    static char msgin[MSGSIZE];

    if (pipe(fd) == -1) {
        perror(argv[0]);
        exit(1);
    }

    if((pid = fork()) == 0){ /*first child */
        if(close(fd[0]) == -1) perror("Error with close4");
        write(fd[1], msgout, MSGSIZE);
        if(close(fd[1]) == -1) perror("Error with close3");
        exit(0);
    }
    if(pid < 0) {          /* cannot fork */
          perror(argv[0]);
          if(close(fd[1]) == -1) perror("Error with close3");
          if(close(fd[0]) == -1) perror("Error with close4");
          exit(2);
     }
    if(pid >0){
    if ((pid = fork()) == 0 ) {      /*second child */
        if(close(fd[1]) == -1) perror("Error with close6");
        read(fd[0], msgin, MSGSIZE);
        for(int i = 0;i<MSGSIZE;i++)
        {
            msgin[i] = toupper(msgin[i]);
        }
        puts(msgin);
        if(close(fd[0]) == -1) perror("Error with close5");
    }
    if(pid < 0) {          /* cannot fork */
          perror(argv[0]);
           if(close(fd[1]) == -1) perror("Error with close3");
           if(close(fd[0]) == -1) perror("Error with close4");
           wait(NULL);
          exit(2);
      }
    if (pid > 0) {  /* parent */
        while(wait(NULL) != -1);
        if(close(fd[0]) == -1) perror("Error with close one!");
        if(close(fd[1])== -1) perror("Error with close two!");
    }
    }
    exit(0);
}
