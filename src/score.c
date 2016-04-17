#include "score.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>//Pour open
#include <unistd.h>//Pour read et write et close
#include <stdio.h>//Pour printf
#include <stdlib.h>//Pour malloc
#include <string.h>
#include <glib.h>

#define SCORE "score.txt"

struct _score
{
	int score;
	char *pseudo;
	char gagnant;
};

FILE *ouvrir(char *nomDuFichier){
	FILE *fichier = fopen(nomDuFichier, "a+"); //S_IRWXU : permission pour l'utilisateur
	if(0>fichier){
		printf("erreur à l'ouverture du fichier\n");
	}
	else{
		printf("fichier %s ouvert\n", nomDuFichier);
	}
	return(fichier);
}

void fermer(FILE *fichier){
	if(0>fclose(fichier)){
		printf("erreur à la fermeture du fichier\n");
	}
	else{
	printf("Fichier fermé\n");
	}
}

void ecrire(FILE *fd, char *pseudo, int nv_score, char gagnant)
{
	char n_sc[10];
	sprintf(n_sc, "%d ", nv_score);
	GString *nv_ligne = g_string_new(n_sc);
	nv_ligne = g_string_append(nv_ligne, pseudo);
	nv_ligne = g_string_append(nv_ligne, " ");
	nv_ligne = g_string_append(nv_ligne, &gagnant);
	nv_ligne = g_string_append(nv_ligne, "\n");
	int w = fputs(nv_ligne->str, fd);
	if(w<0)
	{
		printf("Erreur écriture\n");
	}
}

void score_enregistre(Snake *s, char gagnant)
{
	FILE *fd = ouvrir(SCORE);
	int continuer=1, line=0, r;

	int nv_score = snake_longueur(s);//, score;
	ecrire(fd, snake_pseudo(s), nv_score, gagnant);

	fermer(fd);
}

Score *new_score(char *pseudo, int score, char gagnant)
{
	Score *res = (Score *)malloc(sizeof(struct _score));
	res->score = score;
	res->pseudo = pseudo;
	res->gagnant = gagnant;
	return res;
}

char *get_pseudo(Score *s)
{
	return s->pseudo;
}

int get_score(Score *s)
{
	return s->score;
}

void free_score(Score *s)
{
	free(s);
}

List *get_table_scores()
{
	List *res = create_list();

	FILE *fd = ouvrir(SCORE);
	int score;

	char l[100], ps[30], gagnant;
	while(fgets(l, 32, fd)!=NULL)
	{
		sscanf(l, "%d %s %c", &score, ps, &gagnant);
		list_add_last(res, new_score(ps, score, gagnant));
	}

	fermer(fd);

	return res;
}

void afficher_score_console(List *tab_s)
{
	Score *s;
	Node n;
	for(n = list_get_first(tab_s); n!=NULL; n=node_next(n))
	{
		s = (Score *)node_elt(n);
		printf("%d : %s\n", get_score(s), get_pseudo(s));
	}
}