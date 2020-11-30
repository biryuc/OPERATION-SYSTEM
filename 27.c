#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    FILE* pipe;
    int status;
    char result[3];
 
    pipe = popen("grep \"^$\" /home/nikita/my.txt | wc -l ", "r");
    if(pipe == NULL){
        perror("popen fail!");
        exit(1);
    }
    fgets(result,3,pipe);
    if((status = pclose(pipe)) != 0){
        if(status == -1) perror("pclose faild or wait faild!");
        else {
        if(WIFEXITED(status) != 0) printf("exit status: %d\n", WEXITSTATUS(status));
        if(WIFSIGNALED(status) != 0) printf("It was signal: %d\n", WTERMSIG(status));}
        exit(1);
    }
    printf("result is: %s\n", result);
    printf("%s""%d\n","exit status is ",status);
    exit(0);
}
