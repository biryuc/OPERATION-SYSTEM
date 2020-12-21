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
    }

    environ = envp;
    printf("New environ is:\n");
    for(int i=0; environ[i] != NULL; i++)
    {
        printf("%s\n", environ[i]);
    }
    execvp(file, argv);
    return(1);
}   

int main()
{
    static char *argv[ ] =
    { "/home/nikita/", "text", (char *) 0 };
    static char *nenv[ ] =
    { "ChangeEnv=yes", "MYFILE=text", (char *) 0 };

    execvpe(argv[0], argv, nenv);
    perror(argv[0]);
    exit(1);
} 
