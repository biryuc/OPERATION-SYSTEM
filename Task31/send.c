#include <sys/types.h> 
#include <sys/socket.h> 
#include <string.h> 
#include <sys/un.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SOCKETNAME  "SOCKET"

int main()
{
	int sock;       // socket
	int len;	// Длина sockaddr 
	int sockread;	// return from read() 
	int nready;	// # descriptors ready. 
	int maxfd;	  
	char buf[1024];
	fd_set fds;	 
	struct sockaddr_un server;


	if( (sock = socket(AF_UNIX, SOCK_STREAM, 0) ) < 0){
		perror("socket");
		exit(1);
	}

	/*Create the address of the server.*/

	memset(&server, 0, sizeof(struct sockaddr_un));

	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, SOCKETNAME);
	len = sizeof(server.sun_family) + strlen(server.sun_path);


	/*Connect to the server.*/

	if (connect(sock, (struct sockaddr *) &server, len) < 0){
		perror("connect");
		exit(1);
	}


	maxfd = sock + 1;
	while(1){
		
		FD_ZERO(&fds);
		FD_SET(sock,&fds);
		FD_SET(0,&fds);

		
		nready = select(maxfd, &fds, (fd_set *) 0, (fd_set *) 0,
				(struct timeval *) 0);

		
		// MSG from server
		if( FD_ISSET(sock, &fds))
		{
			sockread = recv(sock, buf, sizeof(buf), 0);
			
			if(sockread < 1){
			       close(sock);
			       return -1;
			}
			write(1, buf, sockread);
		}
		//send to server
		if( FD_ISSET(0, &fds))
		{
			sockread = read(0, buf, sizeof(buf));
			
			if(sockread < 1){
				close(sock);
				return -1;
			}
			send( sock, buf, sockread, 0); 
		}
	} 

}
