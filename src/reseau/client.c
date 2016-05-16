
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

#include "../partie.h"
#include "../struc.h"
#include "../bonus.h"
#include "../list.h"
#include "../collisions.h"

int socket_client, id;

void *send_mysnake(void *p)
{
	Partie *partie = (Partie *)p;
	TabSnakes *ts = partie_tab(p);
	Snake *mysnake = ts->snakes[id];
	print_snake(mysnake);
	Coord my_send[30];
	Node n;
	int i;

	printf("C: SENDING...\n");
	my_send[0] = coord_from_xy(-1, snake_longueur(mysnake));
	my_send[1] = coord_from_xy(-2, snake_direction(mysnake));
	i=2;
	for(n = snake_premier(mysnake); n != NULL; n = node_next(n))
	{
		my_send[i] = *(Coord *)node_elt(n);
		i += 1;
	}
	while(i<30)
	{
		my_send[i] = coord_from_xy(-1, -1);
		i += 1;
	}
	/*
    printf("C: Send\n");
    for(i = 0; i < 30; i++)
    {
        print_coord(my_send[i]);
    }
    printf("C: end\n");
    */
	if(write(socket_client, my_send, sizeof(my_send))<=0)
	{
		perror("C: Send my snake");
	}
	else
	{
		printf("C: Snake send!\n");
	}
}

/*
	Liste des indacteurs:
		(nb_snake, nb_bouf) : Premier élément du paquet reçu
		(-1, n) : nouveau snake en provenance de longueur n
		(-2, n) : n = direction du snake en cours (suit un (-2,n))
		(x, x)  : coordonnées d'un élèment de snake
		(-1, -1): fin de données utiles
*/
void update_partie(Partie *partie, Coord *recive)
{
	int nb_snake = recive[0].x, nb_bouf = recive[0].y, id_snake=0, lg_snake=0, dir_snake=0, i, n=0, cur_lg=0;
	TabBonus *tb = partie_tab_bonus(partie);
	TabSnakes *ts = partie_tab(partie);
	Snake *s = NULL;
	List *l = NULL;
	Node node = NULL;
	Bonus *b = NULL;

	n=2;
	for(i=0; i<tab_snakes_length(ts); i++)//On update tout les snakes sur notre partie
	{
		printf("C: update du snake %i\n", i);
		s = ts->snakes[i];
		l = snake_liste_snake(s);
		node = list_first_node(l);

		lg_snake = snake_longueur(s);

		//print_coord(recive[n]);
		dir_snake = recive[n].y;
		n += 1;
		//printf("C: Setting direction!\n");
		snake_set_direction(s, dir_snake);
		//printf("C: Direction is set!\n");
		cur_lg = 0;
		//print_coord(recive[n]);
		while(recive[n].x !=  -1)
		{
			if(cur_lg >= lg_snake )
			{
				node = cons_node_next(&(recive[n]), node);
				n += 1;
				list_add_node_last(l, node);
				printf("cur_lg : %i, lg_snake : %i\n", cur_lg, lg_snake);
			}
			Coord *elt = malloc(sizeof(Coord));
			*elt = coord_new(recive[n].x, recive[n].y);// !! Il faudra liberer l'espace à chaque espace !!
			node_set_elt(node, elt);
			node = node_next(node);
			n += 1;
			cur_lg += 1;
		}
	}
	if(tab_snakes_length(ts) < nb_snake)
	{
		printf("C: Nouveau joueur!\n");

		lg_snake = recive[n].y;
		n += 1;
		dir_snake = recive[n].y;
		n += 1;
		s = create_snake(lg_snake, recive[n], dir_snake);
		tab_snakes_add_object(ts, s);
		n += lg_snake;
	}
	/*
	printf("Coord à la fin de la mise à jour des snakes n:%i\n", n);
	print_coord(recive[n]);
	*/
	n += 1;
	/*printf("mise à jour de la bouf n=%i\n", n);
	for( i=n; i < n+5; i++)
	{
			print_coord(recive[i]);
	}
	*/
	for(i=0; i < tb->nb_bonus; i++)
	{
		print_coord(recive[n]);
		bonus_update(tb->bonus[i], recive[n].x, recive[n].y);
		n += 1;
	}
	if(tab_bonus_length(tb) < nb_bouf)
	{
		printf("C: Nouvelle bouf!\n");
		b = bonus_new(recive[n].x, recive[n].y);
		n += 1;
		tab_bonus_add_object(tb, b);
	}
	printf("C: Partie updated\n");
}

void *get_theirsnake(void *p)
{
	Partie *partie = (Partie *)p;
	TabSnakes *ts = partie_tab(p);
	pthread_t t_send;
	int ret, errmask, continuer = 1;
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
		printf("C: Thread d'emmission\n");
		ret=pthread_create(&t_send, NULL, &send_mysnake, partie);
		errmask |= (ret==-1);

	}
}

void init_client(int argc, char **argv, Partie *partie)
{
	pthread_t t_recv, t_send;

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
		ret = write(socket_client, argv[1], sizeof(argv[1]));
		if(ret <= 0)
		{
			perror("C: Echec de l'envoi du pseudo DECO\n");
			exit(1);
		}
		printf("C: Pseudo envoyé (%d)\n", ret);

		ret = read(socket_client, &id, sizeof(int));
		if(ret <= 0)
		{
			perror("C: Echec d'identification du serveur\n");
			exit(1);
		}
		printf("C: Connexion initialisée, id : %d\n", id);

		Snake *mysnake = create_snake(
				5,
				coord_from_xy(22, 2+5*id),
				HAUT
		);
		tab_snakes_init_add_object(partie_tab(partie), mysnake, id);
		partie_set_player(partie, mysnake);

		ret=pthread_create(&t_send, NULL, &send_mysnake, partie);
		errmask |= (ret==-1);

		ret=pthread_create(&t_recv, NULL, &get_theirsnake, partie);
		errmask |= (ret==-1);

		if (errmask)
		{
			perror("C: Echec initialisation des threads");
			exit(1);
		}
		printf("C: Threads initialisée\n");
	}
}