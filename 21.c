#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>      
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>

int counter;
struct termios tty,savtty;

void handler(int sig) {
    if (sig == SIGQUIT) {
        
        tcsetattr(0, TCSANOW, &savtty);
        exit(0);
    }
    
    if(sig == SIGINT) {
        printf("\a");
        ++counter;
        fflush(stdout);         
     }
}
int main() {
      tcgetattr(0, &tty);
      savtty = tty;
      tty.c_lflag &= ~ECHO;
      tcsetattr(0, TCSANOW, &savtty);
      while(1) {
        if(signal(SIGINT, handler)==SIG_ERR) {
            perror("SIGINT");
        }
        if(signal(SIGQUIT, handler)==SIG_ERR) { 
            perror("SIGQUIT");
        }
      }

    return 0;
}
