#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
extern char *tzname[];
 
int main()
{
    time_t now;
    struct tm *sp;

    if(putenv("TZ=California+7") != 0)
    {	
    	perror("Error");
    	return 0;
    }
       
    (void) time( &now );

    printf("%s", ctime( &now ) );

    sp = localtime(&now);
    if (!sp)
    {
    	perror("Error");
    	return 0;
    }
        

    printf("%d/%d/%02d %d:%02d %s\n",
            sp->tm_mon + 1, sp->tm_mday,
            sp->tm_year, sp->tm_hour,
            sp->tm_min, tzname[sp->tm_isdst]);
    exit(0);
}
