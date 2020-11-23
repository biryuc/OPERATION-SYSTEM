#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/wait.h>
#include <errno.h>


int main(int argc, char *argv[]){

    pid_t pid;
    int status;
    char* arg0;
    if ((pid = fork()) > 0) {
        printf("Hello\n");
        if( wait(&status) == -1)
        {
            if(errno == EINTR) wait(&status);
            perror("no incomplete subprocess!");
            exit(1);
        }
        printf(" waited, goodbuy!");
    }
    else if (pid == 0){
        arg0 = argv[0];
        argv[0] = "cat";
        execvp("cat", argv);
        argv[0] = arg0;
        perror("Exec error");
        exit(1);
    }
    else {
        perror(argv[0]);
        exit(1);
    }
    exit(0);
}
