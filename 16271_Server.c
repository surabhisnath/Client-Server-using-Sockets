#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>


#define PORT 5000
#define MAX_CLIENTS 100

static int NUM_CLIENTS = 0;

static int client_id = 1;

struct client
{
	struct sockaddr_in address;
	int fd;
	int client_id;
	
}obj;



struct client *arrofclients[MAX_CLIENTS];

void add_client(struct client *cl);

void remove_client(int client_id);

void send_all(char *message);

void *manage_clients(void *arg);

void *rread();


int main(int argc, char *argv[])
{
	
	pthread_t thread_id;	

	struct sockaddr_in clientaddr;	
	struct sockaddr_in serveraddr;	
	

	int sock_fd = 0;			
	
	sock_fd=socket(AF_INET,SOCK_STREAM,0);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);

	//signal(SIGPIPE, SIG_IGN);


	if(bind(sock_fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
	{
		 printf("Error in binding\n");
		 return 1;
	}

	if(listen(sock_fd,10)<0)
	{
		printf("Error in listening\n");
		return 1;
	}

	printf("Server started\n");


	int fd = 0;
	char *sender;
	

	while(1)
	{
		pthread_create(&thread_id, NULL, &rread, NULL);
		
		socklen_t value = sizeof(clientaddr);
		
		fd = accept(sock_fd, (struct sockaddr*)&clientaddr, &value);
		
		
		if(NUM_CLIENTS+1>MAX_CLIENTS)
		{
			NUM_CLIENTS=MAX_CLIENTS;
			close(fd);
			continue;
		}

		struct client *new_client = (struct client *)malloc(sizeof(struct client));	

		new_client->fd = fd;
		new_client->client_id = client_id++;
		new_client->address = clientaddr;

		NUM_CLIENTS++;

		add_client(new_client);

		pthread_create(&thread_id, NULL, &manage_clients, (void*)new_client);
		sleep(2);
	}
}


void add_client(struct client *cl)
{
	int i = 0;

	while(i<MAX_CLIENTS)
	{
		if(!arrofclients[i])
		{
			arrofclients[i] = cl;
			break;
		}

		i++;
	}

	return;
}

void remove_client(int client_id)
{
	int i = 0;

	while(i<MAX_CLIENTS)
	{
		if(arrofclients[i] && arrofclients[i]->client_id == client_id)
		{
			arrofclients[i] = NULL;
			break;
		}

		i++;
	}

	return;
}


void send_all(char *message)
{
	int i = 0;

	while(i<MAX_CLIENTS)
	{
		if(arrofclients[i])
		{
			write(arrofclients[i]->fd, message, strlen(message));
		}

		i++;
	}
}


void *manage_clients(void *arg)
{

	char inp[3000];
	char outp[3000];
	int inplen;

	
	struct client *client_obj = (struct client *)arg;


	printf("Accepted client %d\n",client_obj->client_id);


	while((inplen = read(client_obj->fd, inp, sizeof(inp)-1))>0)
	{

		inp[inplen] = '\0';
		outp[0]='\0';
		
		int j=0;
		while(inp[j] != '\0')
		{
			if(inp[j] == '\r' || inp[j] == '\n')
			{
				inp[j] = '\0';
			}
		
			j++;
		}

		if(!strlen(inp))
			continue;


		sprintf(outp, "Received from client %d: %s", client_obj->client_id, inp);
		send_all(outp);		
	}


	int i = client_obj->client_id;
	printf("Client %d exited\n",i);

	NUM_CLIENTS = NUM_CLIENTS-1;

	close(client_obj->fd);
	remove_client(i);
	free(client_obj);
	pthread_detach(pthread_self());
	
	return NULL;
}


void *rread()
{
	int inplen;
	char inp[3000];

	scanf("%s",&inp);

	if(strcmp(inp, "exit")==0 || strcmp(inp, "quit")==0 || strcmp(inp, "bye")==0)
	{
		exit(0);
	}	
}