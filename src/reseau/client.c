
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

#include "client.h"

void *send_mysnake(void *p)
{

}

void *get_theirsnake(void *p)
{
	int ret, continuer = 1, sock = *(int *)p;
	char *buf;
	while(continuer)
	{
		ret=read(sock, buf, 2*sizeof(char));
		if(ret<=0)
		{
			perror("get_theirsnake : deco");
			continuer = 0;
		}
	}
}

void init_client(int argc, char **argv)
{
	pthread_t t_send;
	pthread_t t_recv;

	int ret, errmask=0;	
	int sock = socket(AF_INET, SOCK_STREAM, 0);
		
	int *num;

	if(sock == -1)
	{
	    perror("fdet()");
	    exit(errno);
	}

	struct sockaddr_in m_sin; /* initialise la structure avec des 0 */

    if(atoi(argv[1])==2)//On passe 2 en premier param si on est le joueur, 3 si on est le seveur
    {
    	if (argc != 4)
		{
	    	printf("Usage : %s 2 <Pseudo> <IP de l'hôte>\n", argv[0]);
	        exit(1);
	    }
		m_sin.sin_addr.s_addr = inet_addr(argv[4]); /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	}
	else
	{
		pthread_t t_serv;
		ret = pthread_create(&t_serv, NULL, &lunch_serveur, NULL);//On lance le thread pour le serveur
		if(ret == -1)
	    {
	    	perror("Echec initialisation du threads t_serv");
	    	exit(1);
	    }

    	usleep(100);//Temps pour que le serveur soit op

		m_sin.sin_addr.s_addr = INADDR_ANY;
	}
	m_sin.sin_port = htons(3940); /* on utilise htons pour le port 3940*/
	m_sin.sin_family = AF_INET;
	bzero(&(m_sin.sin_zero), 8);

	if(connect(sock,(struct sockaddr *)&m_sin, sizeof(struct sockaddr)) == -1)
	{
	    perror("connect()");
	    exit(errno);
	}
	else
	{
		printf("Envoi du pseudo : %s (size:%d) (sock %d)\n", argv[2], (int)sizeof(argv[2]), sock);
		ret = write(sock, argv[2], sizeof(argv[2]));
		if(ret <= 0)
		{
			perror("Echec de l'envoi du pseudo DECO\n");
			exit(1);
		}

		printf("sizeof(int)=%d sock=%d\n", (int)sizeof(int), sock);
		ret = recv(sock, num, sizeof(int), 0);
		if(ret <= 0)
		{
			perror("Echec d'identification du serveur\n");
			exit(1);
		}
		printf("Connexion initialisée, id : %d\n", *num);

		ret=pthread_create(&t_recv, NULL, &get_theirsnake, &sock);
	    errmask |= (ret==-1);

	    ret=pthread_create(&t_send, NULL, &send_mysnake, &sock);
	    errmask |= (ret==-1);

	    if (errmask)
	    {
	    	perror("Echec initialisation des threads");
	    	exit(1);
	    }
	    printf("Threads initialisée\n");
	}
}