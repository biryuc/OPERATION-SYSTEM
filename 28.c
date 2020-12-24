#include <sys/types.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
 
int  main()
            {
            int i;
           FILE *ptrs[2];
           char num[4];

           srand(time(0));
           p2open("/bin/sort", ptrs);

           for( i = 1; i <= 100; i++) {
           sprintf(num, "%02d\n", rand() % 100);
           fputs(num, ptrs[0]);
           }
           fclose(ptrs[0]);
           i = 1;
           while(fgets(num,4,ptrs[1]) != NULL) {
           num[2] = '\0';
           printf("  %s", num);
           if ( (i++ % 10) == 0)
           putchar('\n');
           }
           putchar('\n');
        return 0;
           }
