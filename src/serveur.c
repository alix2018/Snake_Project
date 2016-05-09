
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

#include "partie.h"
#include "struc.h"
#include "bonus.h"
#include "list.h"

#define MAX_NB_CLIENT 10 
#define MAX_NAME_LENGTH 64

#define DATA_SIZE 2048

int nb_client=0;

struct client_rec{
    int   sock;
    int   updated;
    char  name[MAX_NAME_LENGTH+1];
};

typedef struct partie_id{
    int   id;
    Partie *partie;
} PartieId;

struct client_rec client_record[MAX_NB_CLIENT];

void *serveur_send(void *p)
{
	printf("S: serveur_send...\n");
	int continuer=1, envoi=0, c, i, n, ret;
	Coord msg[300];
	Partie *partie = (Partie *)p;
	TabSnakes *ts = partie_tab(p);
	TabBonus *tb = partie_tab_bonus(p);
	Node node;

	while(continuer)
	{	
		if(nb_client > 0)
		{
			envoi = 1;
		}
		for(c=0; c < nb_client; c++)
		{
			if(client_record[c].updated != 1)
			{
				envoi = 0;
			}
			else
			{
				printf("S: Client %i updated\n", c);
			}
		}
		if(envoi)
		{
			printf("S: SENDING PARTIE\n");

			msg[0] = coord_from_xy(tab_snakes_length(ts), tab_bonus_length(tb));
			n = 1;
			for(i=0; i<msg[0].x; i++)
			{
				msg[n] = coord_from_xy(-1, snake_longueur(ts->snakes[i]));
				n += 1;
				msg[n] = coord_from_xy(-2, snake_direction(ts->snakes[i]));
				n += 1;

				for(node = snake_premier(ts->snakes[i]); node != NULL; node = node_next(node))
				{
					msg[n] = *(Coord *)node_elt(node);
					n += 1;
				}
			}
			msg[n] = coord_from_xy(-1, -1);
			n += 1;
			for(i = 0; i<msg[0].y; i++)
			{
				msg[n] = bonus_coord(tb->bonus[i]);
				n += 1;
			}
			while(n < 300)
			{
				msg[n] = coord_from_xy(-1, -1);
				n += 1;
			}
			/*
			for(n = 0; n<20; n++)
			{
				print_coord(msg[n]);
			}
			*/
			for(i=0; i<nb_client; i++)
			{
				ret = write(client_record[i].sock, msg, sizeof(msg));
				printf("S: Partie send to the client %d (size: %d)\n", i, (int)ret);
				if(ret <= 0)
				{
					perror("S: Send fail");
				}
				client_record[i].updated = 0;
			}
		}
	}
}

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
	
	printf("S: En attente de joueur...\n");
	int client_fd = accept(fd, (struct sockaddr *)&their_addr, &sin_size);
	
	if(client_fd < 0)
	{
		perror("S: client_connection");
		exit(1);
	}
	else
	{
		client_record[nb_client % MAX_NB_CLIENT].sock = client_fd;
		printf("S: Nouvelle connection : client %d, num %d\n", client_record[nb_client % MAX_NB_CLIENT].sock, nb_client%MAX_NB_CLIENT);
		client_record[nb_client % MAX_NB_CLIENT].updated = 0;
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

	printf("S: *listen (maxconn=%d)\n" ,MAX_NB_CLIENT);

	return sockfd;
}

//Quand le serveur recoit l'info du snake, il met à jour sa partie.
//Quand il a recut l'info de tout les joueurs, ils transemet aux joueur sa partie
void *serveur_recv(void *p)
{
	PartieId *pid = (PartieId *)p;
	int id = pid->id, i, c, ret;

	printf("S: Thread de reception pour le client %d \n", id);

	int continuer = 1;
	Coord recive[30];

	TabSnakes *ts = partie_tab(pid->partie);
	printf("Tab Snake ok\n");
	Snake *s = create_snake(1, coord_from_xy(5,5), HAUT);
	print_snake(s);
	printf("Snake create\n");
	tab_snakes_add_object(ts, s);
	printf("Add ok\n");
	Node n;
	while(continuer)
	{
		ret = read(client_record[id].sock, recive, sizeof(recive));
		if(ret <= 0)
		{
			perror("S: Sereur recive");
			continuer = 0;
		}
		printf("S: Recive from client %i, size:%i\n", id, (int)ret);
		/*	
			printf("S: recive\n");
			for( c = 0; c < 30; c++)
			{
				print_coord(recive[c]);
			}
			printf("S: end\n");
		*/	

		if(snake_longueur(s)<recive[0].y)
		{
			snake_increase(s);
			printf("S: Snake increase !\n");
		}
		printf("S: Set direction\n");
		print_coord(recive[1]);
		snake_set_direction(s, recive[1].y);
		i = 2;
		printf("S: Updating coord... ");
		for(n = snake_premier(s); n != NULL; n = node_next(n))
		{
			Coord *elt = coord_new(recive[i].x, recive[i].y);
			node_set_elt(n, elt);
			i += 1;
		}
		client_record[id].updated = 1;
		printf("S: Snake %i updated!\n", id);
		usleep(150);
	}
}

//Retourne 1 si le serveur s'est bien lancé
void *init_serveur(void *p)
{
	Partie *partie = create_partie();
	TabSnakes *ts = partie_tab(p);
	TabBonus *tb = partie_tab_bonus(p);

	ts = create_tab_snakes();
	tb = create_tab_bonus();

	PartieId *pid = malloc(sizeof(struct partie_id));
	pid->partie = partie;

	int sock, ret, errmask=0, srp, id, continuer=1;
	pthread_t t_recv, t_send;

	struct client_rec *nv_client = malloc(sizeof(struct client_rec));

	sock = create_listen_socket(3940);

	pthread_create(&t_send, NULL, &serveur_send, partie);

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
			printf("S: id : %d, pseudo : %s \nNombre de client : %d\n", id, client_record[id].name, nb_client+1);
			ret = write(client_record[id].sock, &id, sizeof(int));
			if(ret <= 0)
			{
				printf("SERVEUR Echec envoie id joueur");
			}
			else
			{
				printf("S: Envoie de l'id (taille:%d)\n", ret);
				pid->id = id;
				pthread_create(&t_recv, NULL, &serveur_recv, pid);
				nb_client += 1;
			}
		}


	}

	return 0;
}