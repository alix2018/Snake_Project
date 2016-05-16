
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

#include "partie.h"
#include "struc.h"
#include "bonus.h"
#include "list.h"

int socket_client, id;

void *send_mysnake(void *p)
{
	Partie *partie = (Partie *)p;
	TabSnakes *ts = partie_tab(p);
	Snake *mysnake = tab_snakes_get(ts, id);
	int continuer =1;
	while(continuer)
	{
	}
}

/*
	Liste des indacteurs:
		(-1, n) : n = Nombre de snakes en cours sur la partie, premier élément du paquet reçu
		(-2, n) : nouveau snake en provenance de longueur n
		(-3, n) : n = direction du snake en cours (suit un (-2,n))
		(x, x)  : coordonnées d'un élèment de snake
		(-1, -1): fin des données utiles
*/
void update_partie(Partie *partie, Coord *recive)
{
	int nb_snake = recive[0].y, id_snake=0, lg_snake=0, dir_snake=0, i, n=1, cur_lg=0;
	TabSnakes *ts = partie_tab(partie);
	Snake *s=NULL;
	List *l=NULL;
	Node node=NULL;
	
	for(i=0; i<tab_snakes_length(ts); i++)//On update tout les snakes sur notre partie
	{
		s = tab_snakes_get(ts, i);
		l = snake_liste_snake(s);
		node = list_first_node(l);
		
		lg_snake = snake_longueur(s);
		n += 1;
		dir_snake = recive[n].y;
		n += 1;
		snake_set_direction(s, dir_snake);
		cur_lg = 0;
		while(recive[n].x !=  -2)
		{
			if(cur_lg >= lg_snake )
			{
				node = node_cons_next(recive[n], node);
				list_add_last_node(l, node);
			}
			Coord new = coord_from_xy(recive[n].x, recive[n].y);
			void *ok = &new;
			node_set_elt(node, ok);
			node = node_next(node);
			n += 1;
			cur_lg += 1;
		}
	}
	print_coord(recive[n]);
	if(tab_snakes_length(ts) < nb_snake)
	{
		printf("Nouveau joueur!\n");

		lg_snake = recive[n].y;
		n += 1;
		dir_snake = recive[n].y;
		n += 1;
		s = create_snake(lg_snake, recive[n], dir_snake);

		tab_snakes_add_object(ts, s);
	}
}

void *get_theirsnake(void *p)
{
	Partie *partie = (Partie *)p;
	TabSnakes *ts = partie_tab(p);

	int ret, continuer = 1;
	Coord recive[900];
	while(continuer)
	{
		ret=read(socket_client, recive, sizeof(recive));
		if(ret<=0)
		{
			perror("C: get_theirsnake : deco");
			continuer = 0;
		}
		update_partie(partie, recive);
	}
}

void init_client(int argc, char **argv, Partie *partie)
{
	pthread_t t_send;
	pthread_t t_recv;

	int ret, errmask=0;	
	
	if((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	    perror("C: Problème socket");
	    exit(errno);
	}

	struct sockaddr_in m_sin; /* initialise la structure avec des 0 */

    if(strcmp(argv[2],"H")==0)//Si on est l'hôte on prend son ip
    {
		m_sin.sin_addr.s_addr = INADDR_ANY;
	}
	else//Si on n'est pas le serveur on prend l'ip passé en param
	{
		m_sin.sin_addr.s_addr = inet_addr(argv[2]); /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	}
	m_sin.sin_port = htons(3940); /* on utilise htons pour le port 3940*/
	m_sin.sin_family = AF_INET;
	bzero(&(m_sin.sin_zero), 8);

	if(connect(socket_client,(struct sockaddr *)&m_sin, sizeof(struct sockaddr)) == -1)
	{
	    perror("C: connect()");
	    exit(errno);
	}
	else
	{
		printf("C: Envoi du pseudo : %s (size:%d) (sock %d)\n", argv[1], (int)sizeof(argv[1]), socket_client);
		ret = write(socket_client, argv[2], sizeof(argv[2]));
		if(ret <= 0)
		{
			perror("C: Echec de l'envoi du pseudo DECO\n");
			exit(1);
		}
		printf("C: Pseudo envoyé (%d)\n", ret);

		printf("sizeof(int)=%d sock=%d\n", (int)sizeof(int), socket_client);
		ret = read(socket_client, &id, sizeof(int));
		if(ret <= 0)
		{
			perror("C: Echec d'identification du serveur\n");
			exit(1);
		}
		printf("C: Connexion initialisée, id : %d\n", id);
		
		ret=pthread_create(&t_recv, NULL, &get_theirsnake, partie);
	    errmask |= (ret==-1);

	    Snake *mysnake = create_snake(
            5,
            coord_from_xy(22, 2+5*id),
            HAUT
    	);
	    tab_snakes_add_object(partie_tab(partie), mysnake);
	    partie_set_player(partie, mysnake);

	    ret=pthread_create(&t_send, NULL, &send_mysnake, partie);
	    errmask |= (ret==-1);

	    if (errmask)
	    {
	    	perror("C: Echec initialisation des threads");
	    	exit(1);
	    }
	    printf("C: Threads initialisée\n");
	}
}