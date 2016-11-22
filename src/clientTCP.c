/*      (C)2000 FEUP  */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>

#define SERVER_PORT 6000
#define SERVER_ADDR "192.168.28.96"

int conetion(){

	int	sockfd;
	struct	sockaddr_in server_addr;
	char	buf[] = "user anonymous\n";
	int	bytes;

	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("195.20.41.161");	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(21);		/*server TCP port must be network byte ordered */

	/*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    		perror("socket()");
        	exit(0);
    	}
			printf("Opened\n");
	/*connect to the server*/
    	if(connect(sockfd,
	           (struct sockaddr *)&server_addr,
		   sizeof(server_addr)) < 0){
        	perror("connect()");
		exit(0);
	}
	printf("Connected\n");

    	/*send a string to the server*/
	bytes = write(sockfd, buf, strlen(buf));
	printf("Bytes escritos %d\n", bytes);

	close(sockfd);
	exit(0);
}