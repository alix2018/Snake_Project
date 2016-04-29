#include "multi.h"
#include "serveur.h"
#include <pthread.h>



void init_multi(int argc, char **argv, Partie *partie, ClutterScript *ui, int width, int height)
{
	int ret;

	init_partie_min(partie, width, height);

	if(strcmp(argv[2],"H")==0)
	{
		pthread_t t_serv;
		ret = pthread_create(&t_serv, NULL, &init_serveur, partie);//On lance le thread pour le serveur
		if(ret == -1)
	    {
	    	perror("Echec initialisation du threads t_serv");
	    	exit(1);
	    }
	    usleep(300);
	}
	init_client(argc, argv, partie);
}
