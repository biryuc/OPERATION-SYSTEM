#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int execvpe(char *file, char *argv[], char *envp[])
{
    extern char **environ;
    printf("Old environ is:\n");
    for(int i=0; environ[i] != NULL; i++)
    {
        printf("%s\n", environ[i]);
        environ[i] = NULL;
    }



    for (int i=0; envp[i] != NULL; i++)
    {
        if (putenv(envp[i]) == -1)
        {
            return -1;
        }
        printf("%s\n", envp[i]);
        
    }
    return execvp(file, argv);
}   

int main()
{
    char *file = "/bin/sh";
    char *argv[] = {file, "-c", "env", (char *) 0};
    static char *envp[] = {"PATH=/usr/bin","KEY=VALUE",(char *) 0};
    if (execvpe(file, argv, envp) == -1)
    {
        perror("execvpe() Error: ");
        return -1;
    }

    return 0;
} 
