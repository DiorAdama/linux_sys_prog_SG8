#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "server.h"

void comm_loop_client(int sockfd){
	char buff[MAX_MESSAGE_LENGTH];
	int pos;
	char help[] = "supported commands are the following:\n \t - exit \n";
	printf("%s",help);
	for (;;) {
		bzero(buff, sizeof(buff));
		
		printf(" >>> ");
		pos = 0;
		while ((buff[pos] = getchar()) != '\n') pos++;

		write(sockfd, buff, sizeof(buff));
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Disconnecting\n");
			break;
		}

		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("Server response: %s", buff);
		
	}
}

int main(){
    int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("client_main.c: socket creation failed\n");
		exit(0);
	}
	else
		printf("Socket successfully created\n");

	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);


	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		printf("client_main.c: connection with the server failed\n");
		exit(0);
	}
	else
		printf("Connected to the server\n");

	comm_loop_client(sockfd);

	close(sockfd);
}

