#include <stdio.h>
   #include <stdlib.h>
   #include <ctype.h>
   #include <termios.h>
   #include <unistd.h>
   #include <stdlib.h>
   #define   BEL   "\07"           /* cntl-G */
   #define   MAXLINE  512
   #define   LINESIZE  40
  #define   BACKSPACE   write(1, "\b \b", 3)
 
int  main(int argc, char *argv[])
  {
      char line[MAXLINE], ch;
      int pos, newpos, savpos, i;
      struct termios tty, savetty;
 
      if (!isatty(0)) {
          perror(argv[0]);
          exit(1);
      }
      if (tcgetattr(0, &tty) == -1) {
          perror(argv[0]);
          exit(2);
      }
      /* turnoff canonical input and echoing */
      savetty = tty;
      tty.c_cc[VMIN] = 1;
      tty.c_cc[VTIME] = 1;
      tty.c_lflag &= ~(ISIG | ICANON | ECHO);
      if (tcsetattr(0, TCSANOW, &tty) == -1) {
          perror(argv[0]);
          exit(3);
      }
 
      pos = 0;
      while (read(0, &ch, 1) > 0) {
          /* if EOF in first position stop */
          if (ch == CEOT) {  /* cntl-D in termios.h */
              if (pos == 0)
                  break;
          }
          /* if ERASE character then erase previous character DELETE */
          else if (ch == tty.c_cc[VERASE]) {
              if (pos > 0) {
                  BACKSPACE;
                  --pos;
              }
          }
          /* if KILL character erase all data on current line DELETE ALL DATA IN STRING */
          else if (ch == tty.c_cc[VKILL]) {
              while (pos > 0) {
                  BACKSPACE;
                  --pos;
              }
          }
          /* if cntl-W erase until beginning of last word */
          else if (ch == CWERASE) {  /* cntrl-W in termios.h */
              while (pos > 0 && isspace(line[pos-1])) {
                  BACKSPACE;
                  --pos;          /* ПРОБЕЛЫ */
              }
              while (pos > 0 && !isspace(line[pos-1])) {
                  BACKSPACE;
                  --pos;          /* ПОСЛЕДНЕЕ СЛОВО */
              }
          }
          /* if new-line start a new line */
          else if (ch == '\n') {
              write(1, &ch, 1);
              pos = 0;
          }
          /* if not printable ring bell instead */
          else if (!isprint(ch)) {
              write(1, BEL, 1);
          }
          /* write character and continue */
          else {
              write(1, &ch, 1);
              line[pos++] = ch;
          }
          /* wrap-around -- OPTIONAL */
          if (pos >= LINESIZE && !isspace(ch)) {
              savpos = pos;
              /* find begining of last word */
              while (pos > 0 && !isspace(line[pos-1]))
                  --pos;
              if (pos > 0) {
                  newpos = 0;
                 /* copy last word to beginning of line */
                  for (i = pos; i < savpos; i++) {
                      BACKSPACE;
                      line[newpos++] = line[i];
                  }
                  pos = newpos;
                  /* display word at beginning of line */
                  write(1, "\n", 1);
                  for (i = 0; i < pos; i++)
                      write(1, &line[i], 1);
              }
             else
                  write(1, "\n", 1);
          }
      }
 
      /* reset terminal settings */
      tcsetattr(0, TCSANOW, &savetty);
 }
