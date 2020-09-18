#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	for (int i = 0; i < 2; ++i)
	{
		FILE *file=fopen("Third.c","r");
		if (file)
		{
			fclose(file);
			printf("Ok\n");
		}
		else perror("This file doesn't open\n");
		
			printf("RID = %d\n",(uid_t) getuid());
			printf("EUID = %d\n",(uid_t) geteuid());
		if(i==0)
		{
			if(setuid(getuid())!=0)
			{
				perror("Error");
				exit(1);
			}
		}
	}
	exit(0);
}
