#include "multi.h"
#include "serveur.h"
#include "../collisions.h"
#include "../affichage.h"
#include <pthread.h>
#include <glib.h>


gboolean timeout_tick_cb_multi(gpointer data)
{
	Partie *partie = data;
	TabSnakes *ts = partie_tab(partie);

	int i;
	for (i = 0; i < ts->nb_snakes; ++i)
	{
		snake_forward(ts->snakes[i]);
	}

	if(partie_en_cours(partie))
		affichage_update(partie_affichage(partie));

	return partie_en_cours(partie);
}


void init_multi(int argc, char **argv, Partie *partie, ClutterScript *ui, int width, int height)
{
	int i, ret;

	init_partie_min(partie, width, height);

	TabSnakes *ts = partie_tab(partie);
	TabBonus *tb = partie_tab_bonus(partie);
	Affichage *affichage = partie_affichage(partie);

	if(strcmp(argv[2],"H")==0)
	{

		tab_bonus_add_object(tb, bonus_new(width, height));

		pthread_t t_serv;
		ret = pthread_create(&t_serv, NULL, &init_serveur, partie);//On lance le thread pour le serveur
		if(ret == -1)
		{
			perror("Echec initialisation du threads t_serv");
			exit(1);
		}
		usleep(300);
		printf("Serveur up\n");
	}
	init_client(argc, argv, partie);

	init_affichage_min(partie, ui, width, height);// affichage.c
	printf("init_affichage ok\n");
	// On choisit une couleur
	GRand * randg = g_rand_new();
	gint32  r = g_rand_int_range(randg,0,360);
	int pas = 360/(ts->nb_snakes);
	for (i = 0; i < ts->nb_snakes ; ++i)
	{
		ClutterColor * color = clutter_color_alloc();
		clutter_color_from_hls(color,(r+pas*i)%360,0.4,1); // Puis le couleurs sont complémentaires
		affichage_add_snake(affichage, ts->snakes[i], color);// affichage.c
	}
	int pasb = 360/(tb->nb_bonus);
	for (i = 0; i < tb->nb_bonus ; ++i)
	{
		ClutterColor * color = clutter_color_alloc();
		clutter_color_from_hls(color,(r+pasb*i)%360,0.4,1);
		affichage_add_bonus(affichage, tb->bonus[i],  color);
	}
	printf("Init multi ok\n");
	g_timeout_add(1000, timeout_tick_cb_multi, partie);

}
