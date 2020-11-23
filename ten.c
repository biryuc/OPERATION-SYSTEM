#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[ ])
{
    int status;

    if (fork() == 0) {
        execvp(argv[1], &argv[1]); 
        perror(argv[1]);
        exit(1);
    }
    
    if( wait(&status) == -1)
    {
        if(errno == EINTR) wait(&status);
        perror("no incomplete subprocess!");
        exit(1);
    }   
    
    if(WIFEXITED(status) != 0) printf("exit status: %d\n", WEXITSTATUS(status));
   
    if(WIFSIGNALED(status) != 0) printf("It was signal: %d\n", WTERMSIG(status));

    exit(0);
}
