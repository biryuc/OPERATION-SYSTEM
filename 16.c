#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
static char result(char *);
  
int   main()
   {
       char c;

	if (isatty(fileno(stdout)) == 0) {
        fprintf(stderr, "stdout not terminal\n");
        exit(-1);
   	 } 
 
      c = result("What will you choose?");
 
      switch(c) {
          case 'y':
              printf("You have selected y.\n");
              break;
          case 'n':
              printf("You have selected n.\n");
              break;
          default :
              printf("Invalid input.\n");
              break;
          }
  }
 
  static char result(char *question)
  {
      char c;
      struct termios tty, savtty;
 
      printf("%s (y/n)? ", question);
      fflush(stdout);
      tcgetattr(0, &tty);
      savtty = tty;
      tty.c_lflag &= ~(ISIG | ICANON);
      tty.c_cc[VMIN] = 1;
      tcsetattr(0, TCSAFLUSH, &tty);
      read(0, &c, 1);
      tcsetattr(0, TCSANOW, &savtty);
      putchar('\n');
      return(c);
  }
