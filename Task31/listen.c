#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#define SOCKETNAME  "SOCKET"

int main()
{
	char buf[1024];			
	int sock;				/* Listen socket */
	int ns;				/* Socket for first connection. */
	int ns2;			/* Socket for second connection. */
	int len;			/* len of sockaddr */	
	int maxfd;			
	int sockread;			/* # chars on read()*/
	int nready;			/* # descriptors ready. */
	struct sockaddr_un server;
	fd_set fds;			

	/* Remove any previous socket.*/
	unlink(SOCKETNAME);

	/* Create the socket. */

	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("socket");
		return -1;
         }

	/* Create the address of the server.*/

	memset(&server, 0, sizeof(struct sockaddr_un));

	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, SOCKETNAME);
	len = sizeof(server.sun_family) + strlen(server.sun_path);

	/*Bind the socket to the address.*/

	if (bind(sock, (struct sockaddr *) &server, len) < 0) {
		perror("bind");
		return -2;
	}
 	
	/* Listen for connections. */
	if (listen(sock, 5) < 0){
		perror( "listen");
		return -3;
	}


        /*Accept a connection.*/
	if ((ns = accept(sock, (struct sockaddr *) &server, &len)) < 0) {
		perror("accept");
		return -4;
	}

	/* Accept another connection. */
	if ((ns2 = accept(sock, (struct sockaddr *) &server, &len)) < 0) {
		perror("accept");
		return -5;
	}
	
	if (ns > ns2){
	maxfd = ns + 1;
	}
	else {
	maxfd = ns2 + 1;
	}
	while(1){
		
		FD_ZERO(&fds);
		FD_SET(ns,&fds);
		FD_SET(ns2,&fds);

		
		nready = select(maxfd, &fds, (fd_set *) 0, (fd_set *) 0,
				(struct timeval *) 0);
		//send second client
		if( FD_ISSET(ns, &fds))
		{
			sockread = recv(ns, buf, sizeof(buf), 0);
			
			if(sockread < 1){
				close(ns);
				close(ns2);
				return -6;
			}
			send( ns2, buf, sockread, 0);
		}
		//send first client
		if( FD_ISSET(ns2, &fds))
		{
			sockread = recv(ns2, buf, sizeof(buf), 0);
			
			if(sockread < 1){
				close(ns);
				close(ns2);
				return -7;
			}
			send( ns, buf, sockread, 0);
		}
	} 
}
