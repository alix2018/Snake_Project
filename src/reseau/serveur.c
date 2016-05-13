
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <netdb.h>  /* for gethostbyaddr (MacOS) */
#include <sched.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define MAX_NB_CLIENT 10 
#define MAX_NAME_LENGTH 64

#define DATA_SIZE 2048

int nb_client=0;

struct client_rec{
    int   sock;
    char  name[MAX_NAME_LENGTH+1];
};

struct client_rec client_record[MAX_NB_CLIENT];

int read_pseudo (struct client_rec *client)
{
	char name[MAX_NAME_LENGTH+1];
	int res = read(client->sock, name, MAX_NAME_LENGTH);
	client_record[nb_client % MAX_NB_CLIENT].sock = client->sock;
	strcpy(client_record[nb_client % MAX_NB_CLIENT].name, name);
	return res;
}

int client_connection(int fd)
{
	struct sockaddr_in their_addr; /* Adresse du connecté  */
	socklen_t sin_size;
	sin_size = sizeof(struct sockaddr_in);
	int client_fd = accept(fd, (struct sockaddr *)&their_addr, &sin_size);
	
	if(client_fd < 0)
	{
		perror("client_connection");
		exit(1);
	}
	else
	{
		client_record[nb_client % MAX_NB_CLIENT].sock = client_fd;
		printf("Nouvelle connection : client %d, num %d\n", client_record[nb_client % MAX_NB_CLIENT].sock, nb_client%MAX_NB_CLIENT);
	}
	return client_fd;
}

int create_listen_socket(short port)
{
	int sockfd;
	struct sockaddr_in my_addr;    /* Adresse */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(port);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	    perror("SERVEUR socket");
	    exit(1);
	}

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
	    perror("SERVEUR bind");
	    exit(1);
	}

	if (listen(sockfd, MAX_NB_CLIENT) == -1) {
		perror("SERVEUR listen");
		exit(1);
	}
	printf("*listen (maxconn=%d)\n" ,MAX_NB_CLIENT);

	return sockfd;
}

void *serveur_recv()
{
	printf("Thread de reception pour le client %d \n", nb_client);
	int continuer = 1;
	while(continuer)
	{

	}
}
//Retourne 1 si le serveur s'est bien lancé
void *lunch_serveur()
{
	int sock, ret, errmask=0, srp, id, continuer=1;
	pthread_t t_recv;

	struct client_rec *nv_client = malloc(sizeof(struct client_rec));

	sock = create_listen_socket(3940);

	while(continuer)
	{
		nv_client->sock = client_connection(sock);
		srp = read_pseudo(nv_client);
		if(srp <= 0){
			perror("Echec sock_read_pseudo");
    		exit(1);
		}
		else{
			printf("\n---SERVEUR--- New client\n");
			id = nb_client%MAX_NB_CLIENT;
			printf("id : %d, pseudo : %s \nNombre de client : %d\n", id, client_record[id].name, nb_client+1);
			ret = write(client_record[id].sock, &id, sizeof(int));
			if(ret <= 0)
			{
				printf("SERVEUR Echec envoie id joueur");
			}
			else
			{
				printf("Envoie de l'id (taille:%d)\n", ret);
				pthread_create(&t_recv, NULL, &serveur_recv, NULL);
				nb_client += 1;
			}
		}


	}

	return 0;
}