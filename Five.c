#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>


void read_file()
{
 int N = 200;
 char arr[N];

        FILE *file=fopen("/home/nikita/OperationSystems/Five.txt","r");
        if (file)
         {
               while (fgets (arr, N, file) != NULL)
               printf("%s", arr);
               fclose(file);
               printf("Ok\n");
         }
         else perror("This file doesn't open\n");
}
int main()
{
 read_file();
return 0;
}



