#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define SCNo16 "o"

void connect_with_server(int *socket_fd, struct sockaddr_in *serveraddr);

int rreceive(int socket_fd);
void *ssend(int socket_fd);


int main()
{

	int socket_fd; 
	struct sockaddr_in serveraddr;
	
	pthread_t thread_id;

	connect_with_server(&socket_fd, &serveraddr);
	
	while(1)
	{

		pthread_create(&thread_id, NULL, &ssend, (void *)socket_fd);

		int tobreak = rreceive(socket_fd);
		if(tobreak==7)
		{
			break;
		}
	}
	
	close(socket_fd);
	printf("Client quited\n");
	return 0;
}


void connect_with_server(int *socket_fd, struct sockaddr_in *serveraddr)
{
	if((*socket_fd = socket(AF_INET, SOCK_STREAM,0))<0)
	{
		printf("Error: socket went wrong");
		return -1;
	}

	char ip[50];
	char p[10];
	int port = 5000;
	printf("Provide IP Address: ");
	gets(ip);

	
	printf("Provide Port Number: ");
	gets(p);
	port = atoi(p);
	

	serveraddr->sin_family = AF_INET;
	serveraddr->sin_port = htons(port);
	serveraddr->sin_addr.s_addr = inet_addr(ip);
	

	if(inet_pton(AF_INET, ip, &serveraddr->sin_addr)<=0)
	{
		printf("Invalid address\n");
		return -1;
	}

	if(connect(*socket_fd,(struct sockaddr *)serveraddr, sizeof(struct sockaddr)) < 0)
	{
		printf("Error in connect\n");
		exit(1);
	}
}


void *ssend(int socket_fd)
{
	char sender[3000];
	gets(sender);

	if(strcmp(sender, "exit")==0 || strcmp(sender, "quit")==0 || strcmp(sender, "bye")==0)
	{
		exit(0);
	}

	else
	{
		send(socket_fd, sender, 3000, 0);
	}
}


int rreceive(int socket_fd)
{
	char reciever[3000];
	int no_of_bytes;
	no_of_bytes = recv(socket_fd, reciever, 3000, 0);


	if(no_of_bytes==0)
	{
		return 7;
	}

	reciever[no_of_bytes] = '\0';
	printf(reciever);
	printf("\n");
		fflush(stdout);
	return 8;
}