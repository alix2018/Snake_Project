//Serveur
/*
	Les deux joueurs s'envoient mutuellement les coordonnés de leurs snake
	j1 <-> serveur
	j2 <-> client

gcc multi_h.c struc.c game_multi.c affichage.c list.c -o multi_h `pkg-config clutter-1.0 --cflags --libs` -pthread -I/usr/include/clutter-1.0 -I/usr/include/pango-1.0 -I/usr/include/cogl -I/usr/include/cairo -I/usr/include/atk-1.0 -I/usr/include/json-glib-1.0 -I/usr/include/gtk-3.0 -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libdrm -I/usr/include/libpng12 -I/usr/include/pixman-1 -I/usr/include/gio-unix-2.0/ -lclutter-1.0 -latk-1.0 -lcogl-pango -ljson-glib-1.0 -lgdk-3 -lXi -lcogl -lgmodule-2.0 -lwayland-egl -lgbm -ldrm -lwayland-server -lEGL -lX11 -lXext -lXdamage -lXcomposite -lXrandr -lwayland-client -lgio-2.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lpango-1.0 -lcairo -lgobject-2.0 -lglib-2.0 -lXfixes -D _SERVEUR_

*/

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

#include "multi_h.h"

#define MYPORT 3490    /* Le port où les utilisateurs se connecteront */
#define MAXDATASIZE 200
#define BACKLOG 4     /* Nombre maxi de connections acceptées en file */

int client_fd, envoi;

Snake *snk_j1;
Snake *snk_j2;

Coord bouf_coord;

void *serveur_receive()
{
	int n, i, continuer = 1;
	ListeSnake ls_j2;
	Coord *snake_tab;

	while(continuer)
	{

	snake_tab = malloc(MAXDATASIZE*sizeof(Coord));

	if((n = recv(client_fd, snake_tab, MAXDATASIZE*sizeof(Coord), 0)) < 0)
	{
		perror("recv()");
		exit(errno);
	}

	if(n == 0)
	{
		printf("Déco\n");
		continuer = 0;
	}
	if(n > 0)
	{
		printf("\n ---------   Paquet reçut        Taille : %d  -------------- J1 envoi = %d\n", n, envoi);

		bouf_coord = snake_tab[0];
				
		if(coord_egales(coord_from_xy(1, 0), snake_tab[1])){
			snake_set_direction(snk_j2, DROITE);
		}
		if(coord_egales(coord_from_xy(-1, 0), snake_tab[1])){
			snake_set_direction(snk_j2, GAUCHE);
		}
		if(coord_egales(coord_from_xy(0, 1), snake_tab[1])){
			snake_set_direction(snk_j2, HAUT);
		}
		if(coord_egales(coord_from_xy(0, -1), snake_tab[1])){
			snake_set_direction(snk_j2, BAS);
		}

		ls_j2 = NULL;
		i=2;
		while(i*sizeof(Coord) < n)
		{
			ls_j2 = cons_liste_snake_fin(snake_tab[i], ls_j2);
			i += 1;
		}

		snake_set_liste(snk_j2, &ls_j2);

		printf("snk_j1 : \n");
		print_snake(snk_j1);
		printf("snk_j2 : \n");
		print_snake(snk_j2);
		printf("bouf\n");
		print_coord(bouf_coord);
		printf(" --------------  Fin de reception  --------------- J1 envoi = 1\n\n");

		envoi = 1;
	}

	}
}

//Le serveur envoie les coordonés des snakes et de la bouf
void *serveur_send()
{
	int ret, i, continuer = 1;
	ListeSnake ls_j1;
	Coord *snake_tab;

	while(continuer)
	{
		if(envoi == 1)
		{
		//On convertie les snakes en 1 tableaux et on met la bouf à la fin
		snake_tab = malloc((2+snake_longueur(snk_j1))*sizeof(Coord));
		
		snake_tab[0] = bouf_coord;
		
		switch(snake_direction(snk_j1))
		{
			case DROITE :
				snake_tab[1] = coord_from_xy(1, 0);
				break;
			case GAUCHE :
				snake_tab[1] = coord_from_xy(-1, 0);
				break;
			case HAUT :
				snake_tab[1] = coord_from_xy(0, 1);
				break;
			case BAS :
				snake_tab[1] = coord_from_xy(0, -1);
				break;
			default : 
				snake_tab[1] = coord_from_xy(0, 0);
		}
		i=2;
		ls_j1 = snake_premier(snk_j1);
		while(i*sizeof(Coord) < (2+snake_longueur(snk_j1))*sizeof(Coord))
		{
			snake_tab[i] = liste_snake_coord(ls_j1);
			ls_j1 = liste_snake_suivant(ls_j1);
			i += 1;
		}

		ret = send(client_fd, snake_tab, (2+snake_longueur(snk_j1))*sizeof(Coord), 0);
		if(ret == -1)
		{
			perror("send");
		}
		if(ret == 0)
		{
			printf("Déco du client\n");
			continuer = 0;
		}
		else{
			printf("\n---------------   ENVOIE  ---------------- J1 envoie = %d\n Taille de l'envoi : %d \n ------------- FIN ENVOIE ---------- J1 envoi = 0\n\n", envoi, ret);
			envoi = 0;
		}
		
		free(snake_tab);

		}
	}
}

Snake *serv_get_j1()
{
	return snk_j1;
}

Snake *serv_get_j2()
{	
	envoi = 0;
	return snk_j2;
}

void serv_set_j1(Snake *s)
{
	snk_j1 = s;
	envoi = 1;
	while(envoi == 1){}
}

void serv_set_j2(Snake *s)
{
	snk_j2 = s;
}

Coord* serv_get_bouf()
{
	return &bouf_coord;
}

void serv_set_bouf(Coord c)
{
	bouf_coord = c;
}

void serv_genere_bouf(int x, int y)
{
	srand(time(NULL));
	bouf_coord = coord_from_xy(rand()%x, rand()%y);
}

int main(int argc, char **argv)
{
	int sockfd;
	int ret, errmask=0;

	pthread_t tids_rec;
	pthread_t tids_send;

	struct sockaddr_in my_addr;    /* Adresse */
	struct sockaddr_in their_addr; /* Adresse du connecté  */
	socklen_t sin_size;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    perror("socket");
	    exit(1);
	}

	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
	    perror("bind");
	    exit(1);
	}
	printf("* %s bind port %d\n",argv[0],MYPORT);

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	printf("* %s listen (maxconn=%d)\n",argv[0],BACKLOG);

	sin_size = sizeof(struct sockaddr_in);
	printf("* %s accept on sockfd=%d ...\n",argv[0],sockfd);
			    
	client_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

	if(client_fd == -1) {
		perror("accept");
	}
	else{

		printf("Nouveaux joueur\n");
		
		snk_j1 = create_snake(2, coord_from_xy(3,3), DROITE);
		printf("ok j1\n");
	    snk_j2 = create_snake(2, coord_from_xy(27,27), GAUCHE);
	    printf("ok j2\n");
	    serv_genere_bouf(30, 30);
	    printf("ok bouf\n");

	    printf("j1_s:\n");
	    print_snake(snk_j1);

	    printf("j2_s:\n");
	    print_snake(snk_j1);

	    printf("Bouf :\n");
	    print_coord(bouf_coord);
	    printf("\n------- Snakes & bouf créés -----------\n\n");

	    envoi = 1;

	    ret=pthread_create(&tids_send, NULL, serveur_send, NULL);
		errmask |= (ret==-1);
	    if (errmask)
	    {
	    	perror("pb creation threads");
	    	exit(1);
	    }

	    if (errmask){perror("pb creation threads");exit(1);}

		ret=pthread_create(&tids_rec, NULL, serveur_receive, NULL);
	    errmask |= (ret==-1);
		    
	    if (errmask)
	    {
	    	perror("pb creation threads");
	    	exit(1);
	    }

	    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	    init_game_m(30, 30);
	}

	return EXIT_SUCCESS;
}