#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#define   MSGSIZE   20
#define   NAME   "socket4"


int main()
{
	int sock, msgsock, rval;
	struct sockaddr_un server;
	char buf[MSGSIZE];
	
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) 
	{
		perror("opening stream socket");
		exit(1);
	}

	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, NAME);//копирование строки NAME
	if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) 
	{
		perror("binding stream socket");
		exit(1);
	}

	printf("Socket has name %s\n", server.sun_path);
	listen(sock, 5);
	for (;;) 
	{
		msgsock = accept(sock, 0, 0);
		if (msgsock == -1)
			perror("accept");
		else do 
		{
			
			if ((rval = read(msgsock, buf, 1024)) < 0)
				perror("reading stream message");
			else if (rval == 0)
			{
				printf("Ending connection\n");
				break;
			}
			else
			for(int i = 0;i<MSGSIZE;i++) {
            			buf[i] = toupper(buf[i]);
        		}

				
				printf("-->%s\n", buf);
		} while (rval > 0);

		close(msgsock);
		break;
	}

	close(sock);
	
}
