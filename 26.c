#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#define MSGSIZE 20
int main()
{
    FILE *pipe;

    int i, status;

    pipe = popen("echo Hello world", "r");
    if (pipe == NULL)
    {
        perror("popen");
        return -1;
    }

    char buf[MSGSIZE];
    int read_on_bytes = fread(buf, 1, MSGSIZE, pipe);
    if (read_on_bytes == -1)
    {
        perror("while read");
        return -2;
    }


    for (int i = 0; i < read_on_bytes; i++)
    {
        buf[i] = toupper(buf[i]);
    }

    if (fwrite(buf, read_on_bytes, 1, stdout) == -1)
    {
        perror("while write");
        return -3;
    }


    if((status = pclose(pipe)) == -1) perror("pclose faild!");
    if(WIFEXITED(status) != 0) printf("exit status: %d\n", WEXITSTATUS(status));
   
    if(WIFSIGNALED(status) != 0) printf("It was signal: %d\n", WTERMSIG(status));
    exit(0);
}
