/**
 * @file      score.c
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Toutes les fonctions permettant d'afficher la liste des scores.
 * @details   ---
 */

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


/**
 * @brief      La structure d'un score.
 *
 * @use Score
 * @param[in]  score        La liste des scores.
 * @param[in]  pseudo     	Le pseudo du joueur.
 * @param[in]  gagnant      Le gagnant de la partie.
 */
struct _score
{
	int score;
	char *pseudo;
	char gagnant;
};

/**
 * @brief   Ouvre le fichier
 *
 * @param[in]    nomDuFichier  Le nom du fichier à ouvrir.
 *
 * @return  Le file descriptor du fichier
 */
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

/**
 * @brief   Ferme le fichier.
 *
 * @param[in]    fichier  Le file descriptor à fermer.
 *
 */
void fermer(FILE *fichier){
	if(0>fclose(fichier))
	{
		printf("erreur à la fermeture du fichier\n");
	}
	else
	{
		printf("Fichier fermé\n");
	}
}

/**
 * @brief   Ecrit une nouvelle ligne dans le fichier score
 *
 * @param[in]    fd  Le file descriptor associé au fichier score
 * @param[in]    pseudo Le pseudo du joueur
 * @param[in]    nv_score Le score du joueur
 * @param[in]    gagnant "P" pour perdant, "G" pour gagnant
 *
 */
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

/**
 * @brief   Enregistre le score du snake s
 *
 * @param[in]    s 		 Le snake
 * @param[in]    gagnant 'P' perdant, 'G' gagnant
 *
 */
void score_enregistre(Snake *s, char gagnant)
{
	FILE *fd = ouvrir(SCORE);
	int continuer=1, line=0, r;

	int nv_score = snake_longueur(s);//, score;
	ecrire(fd, snake_pseudo(s), nv_score, gagnant);

	fermer(fd);
}

/**
 * @brief   Créé un nouveau score
 *
 * @param[in]    pseudo
 * @param[in]    score
 * @param[in]    gagnant
 *
 * @return  Le score initialisé
 */
Score *new_score(char *pseudo, int score, char gagnant)
{
	Score *res = (Score *)malloc(sizeof(struct _score));
	res->score = score;

	char *ps =(char *)malloc(sizeof(pseudo));
	int i;
	for(i=0; i<sizeof(pseudo)/sizeof(char); i++)
	{
		ps[i]=pseudo[i];
	}
	
	res->pseudo = ps;
	res->gagnant = gagnant;
	return res;
}

/**
 * @brief   Retourne le pseudo d'un score
 *
 * @param[in]    s score
 *
 * @return  Le pseudo du score
 */
char *get_pseudo(Score *s)
{
	return s->pseudo;
}

/**
 * @brief   Retourne la valeur score d'un score
 *
 * @param[in]    s score
 *
 * @return  Le score du score
 */
int get_score(Score *s)
{
	return s->score;
}

/**
 * @brief   Retourne la valeur gagnant d'un score
 *
 * @param[in]    s score
 *
 * @return  'P' si la partie a était perdu, 'G' gagnante
 */
char get_gagnant(Score *s)
{
	return s->gagnant;
}

/**
 * @brief   Retourne la liste des scores enregistré dans le fichier SCORE
 *
 * @return  La liste des scores
 */
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

/**
 * @brief   Libère la mémoire occupé par un score
 *
 * @param[in]    data score
 *
 */
void free_score(void *data)
{
	struct _score *s = data;
	free(s->pseudo);
	free(s);
}

/**
 * @brief   Libère la mémoire occupé par une liste de score
 *
 * @param[in]    l List
 *
 */
void free_table_scores(List *l)
{
	free_list_fn(l, (void *)free_score);
}

/**
 * @brief   Afficher une liste des score dans la console
 *
 * @param[in]    tab_s la liste des scores à afficher
 *
 */
void afficher_score_console(List *tab_s)
{
	Score *s;
	Node n;
	for(n = list_first_node(tab_s); n!=NULL; n=node_next(n))
	{
		s = (Score *)node_elt(n);
		printf("%d : %s\n", get_score(s), get_pseudo(s));
	}
}

/**
 * @brief   Retourne le score max d'une liste de score et le delete de la liste
 *
 * @param[in]    l Liste
 *
 */
void *elt_max(List *l)
{
	Node n_max = list_first_node(l);
	void *max = node_elt(n_max);
	Node n;
	for(n = list_first_node(l); n != NULL; n = node_next(n))
	{
		if(((struct _score *)max)->score < ((struct _score *)node_elt(n))->score)
		{
			n_max = n;
			max = node_elt(n_max);
		}
	}
	list_delete_node(l, n_max);
	return max;
}

/**
 * @brief   trie la liste l par ordre croissant
 *
 * @param[in]    l La liste des score à trier
 *
 * return 	La list des scores trié
 */
List *tri_max(List *l)
{
	List *res=create_list();
	Node m = NULL;
	while(list_first_node(l) != NULL)
	{
		list_add_last(res, elt_max(l));
	}
	free(l);
	return res;
}