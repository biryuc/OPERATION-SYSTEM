#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define SIZE 256

int main(int argc, char** argv)
{
	struct dirent* p;
	DIR* dir = opendir("./");
	struct stat s;
	struct passwd* pwd;
	struct group* grp;
	struct tm* time;
	int i, isDir;
	if(!dir)
	{
		perror("opendir");
		exit(1);
	}
	while(0 != (p = readdir(dir)))
	{
		if(!strcmp(p->d_name,".") || !strcmp(p->d_name,".."))
		{
			continue;
		}
		for(i = 1; argc>i; i++)
		{
			if(!strcmp(p->d_name,argv[i]))
			{
				if(0 > stat(p->d_name,&s))
				{
					perror("stat");
					exit(2);
				}
				//isDir
				if(s.st_mode & S_IFDIR)
				{
					putchar('d');
					isDir = 1;
				}
				else if(s.st_mode & S_IFREG)
				{
					putchar('-');
					isDir = 0;
				}
				else
				{
					putchar('?');
					isDir = 0;
				}
				//mask
				printf(s.st_mode & S_IRUSR ? "r" : "-");
				printf(s.st_mode & S_IWUSR ? "w" : "-");
				printf(s.st_mode & S_IXUSR ? "x" : "-");
				printf(s.st_mode & S_IRGRP ? "r" : "-");
				printf(s.st_mode & S_IWGRP ? "w" : "-");
				printf(s.st_mode & S_IXGRP ? "x" : "-");
				printf(s.st_mode & S_IROTH ? "r" : "-");
				printf(s.st_mode & S_IWOTH ? "w" : "-");
				printf(s.st_mode & S_IXOTH ? "x" : "-");
				putchar('\t');
				
				printf("%2d\t",s.st_nlink);

				if(0 != (pwd = getpwuid(s.st_uid)))
				{
					printf("%s ", pwd->pw_name);
				}
				else
				{
					printf("%6d ", s.st_uid);
				}

				if(0 != (grp = getgrgid(s.st_gid)))
				{
					printf("%s\t", grp->gr_name);
				}
				else
				{
					printf("%6d\t", s.st_gid);
				}

				if(!isDir)
				{
					printf("%10d\t", s.st_size);
				}
				else
				{
					printf("%10c\t",' ');
				}

				time = localtime(&s.st_mtime);
				printf("%d:%d %d.%d.%d\t", time->tm_hour,
				time->tm_min, time->tm_mday, time->tm_mon+1,
				time->tm_year+1900);
				
				printf("%s\n", p->d_name);
				
				break;	
			}
		}
	}
	if(0 > closedir(dir))
	{
		perror("closedir");
		exit(2);
	}
	return 0;
}
