//Client
/*

gcc multi_c.c struc.c game_multi.c affichage.c list.c -o multi_c `pkg-config clutter-1.0 --cflags --libs` -pthread -I/usr/include/clutter-1.0 -I/usr/include/pango-1.0 -I/usr/include/cogl -I/usr/include/cairo -I/usr/include/atk-1.0 -I/usr/include/json-glib-1.0 -I/usr/include/gtk-3.0 -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libdrm -I/usr/include/libpng12 -I/usr/include/pixman-1 -I/usr/include/gio-unix-2.0/ -lclutter-1.0 -latk-1.0 -lcogl-pango -ljson-glib-1.0 -lgdk-3 -lXi -lcogl -lgmodule-2.0 -lwayland-egl -lgbm -ldrm -lwayland-server -lEGL -lX11 -lXext -lXdamage -lXcomposite -lXrandr -lwayland-client -lgio-2.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lpango-1.0 -lcairo -lgobject-2.0 -lglib-2.0 -lXfixes -D _CLIENT_

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

#include "multi_c.h"

#define PORT 3490 /*port sur le serveur */
#define MAXDATASIZE 200

int fd, envoi;

Snake *snk_j1;
Snake *snk_j2;

Coord bouf_coord;

void *client_receive(){

	int n, i, continuer = 1;
	ListeSnake ls_j1;
	Coord *snake_tab;

	while(continuer == 1)
	{
	snake_tab = malloc(MAXDATASIZE*sizeof(Coord));

	if((n = recv(fd, snake_tab, MAXDATASIZE*sizeof(Coord), 0)) < 0)
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
		printf("\n ---------   Paquet reçut      Taille : %d  --------------  J2 envoi = %d\n", n, envoi);
		
		bouf_coord = snake_tab[0];

		if(coord_egales(coord_from_xy(1, 0), snake_tab[1])){
			snake_set_direction(snk_j1, DROITE);
		}
		if(coord_egales(coord_from_xy(-1, 0), snake_tab[1])){
			snake_set_direction(snk_j1, GAUCHE);
		}
		if(coord_egales(coord_from_xy(0, 1), snake_tab[1])){
			snake_set_direction(snk_j1, HAUT);
		}
		if(coord_egales(coord_from_xy(0, -1), snake_tab[1])){
			snake_set_direction(snk_j1, BAS);
		}

		ls_j1 = NULL;
		i=2;
		while(i*sizeof(Coord)<n)
		{
			ls_j1 = cons_liste_snake_fin(snake_tab[i], ls_j1);
			i += 1;
		}

		snake_set_liste(snk_j1, &ls_j1);

		printf("snk_j1 : \n");
		print_snake(snk_j1);
		printf("snk_j2 : \n");
		print_snake(snk_j2);
		printf("bouf\n");
		print_coord(bouf_coord);

		printf(" --------------  Fin de reception  --------------- J2 envoi = %d\n\n", envoi);

	}

	}

}

void *client_send()
{
	int ret, i, continuer = 1;
	ListeSnake ls_j2;
	Coord *snake_tab;

	while(continuer)
	{
		if(envoi == 1)
		{
		
		//On convertie les snakes en 1 tableaux et on met la bouf à la fin
		snake_tab = malloc((2+snake_longueur(snk_j2))*sizeof(Coord));
	
		snake_tab[0] = bouf_coord;

		
		switch(snake_direction(snk_j2))
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
		
		i = 2;
		ls_j2 = snake_premier(snk_j2);
		while(ls_j2 != NULL)
		{
			snake_tab[i] = liste_snake_coord(ls_j2);
			ls_j2 = liste_snake_suivant(ls_j2);
			i += 1;
		}

		ret = send(fd, snake_tab, (2+snake_longueur(snk_j2))*sizeof(Coord), 0);
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
			printf("\n---------------   ENVOIE  ---------------- J2 envoi = %d\n Taille de l'envoi : %d  (sizeof(Coord)=%d)\n ------------- FIN ENVOIE ---------- J2 envoi = 0\n\n", envoi, ret, (int)sizeof(Coord));
			envoi = 0;
		}
		
		free(snake_tab);	
	
		}
	}

}


Snake *serv_get_j1()
{
	envoi = 0;
	return snk_j1;
}

Snake *serv_get_j2()
{
	return snk_j2;
}


void serv_set_j1(Snake *s)
{
	snk_j1 = s;
}

void serv_set_j2(Snake *s)
{
	snk_j2 = s;
	envoi = 1;
	while(envoi == 1){}
}

Coord *serv_get_bouf()
{
	envoi = 0;
	return &bouf_coord;
}

void serv_set_bouf(Coord c)
{
	bouf_coord = c;
	envoi = 1;
}

int main(int argc, char **argv)
{

	int ret, errmask=0;	
	fd = socket(AF_INET, SOCK_STREAM, 0);

	pthread_t tids_rec;
	pthread_t tids_send;

	if(fd == -1)
	{
	    perror("fdet()");
	    exit(errno);
	}

	struct sockaddr_in m_sin; /* initialise la structure avec des 0 */

	if (argc != 2)
	{
    	printf("Usage : %s <IP de l'hôte>\n", argv[0]);
        exit(1);
    }

	m_sin.sin_addr.s_addr = inet_addr(argv[1]); /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	m_sin.sin_port = htons(PORT); /* on utilise htons pour le port */
	m_sin.sin_family = AF_INET;
	bzero(&(m_sin.sin_zero), 8);

	if(connect(fd,(struct sockaddr *)&m_sin, sizeof(struct sockaddr)) == -1)
	{
	    perror("connect()");
	    exit(errno);
	}
	else
	{

		printf("Nouveaux joueur\n");

		snk_j1 = create_snake(2, coord_from_xy(3,3), DROITE);
		snk_j2 = create_snake(2, coord_from_xy(27,27), GAUCHE);
		bouf_coord.x = 10;
		bouf_coord.y = 10;

		ret=pthread_create(&tids_rec, NULL, client_receive, NULL);
	    errmask |= (ret==-1);
		    
	    if (errmask)
	    {
	    	perror("pb creation threads");
	    	exit(1);
	    }

   		envoi = 1;

	    ret=pthread_create(&tids_send, NULL, client_send, NULL);
		errmask |= (ret==-1);
	    if (errmask)
	    {
	    	perror("pb creation threads");
	    	exit(1);
	    }


	    init_game_m(30,30);
	}

	return 0;
}
