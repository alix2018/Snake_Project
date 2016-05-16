/**
 * @file      score.h
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Toutes les fonctions permettant d'afficher la liste des scores.
 * @details   ---
 */

#ifndef _SCORE_
#define _SCORE_

#include "struc.h"
#include "list.h"

typedef struct _score Score;

void score_enregistre(Snake *s, char gagnant);
char *get_pseudo(Score *s);
int get_score(Score *s);
char get_gagnant(Score *s);

List *get_table_scores();
void afficher_score_console(List *tab_s);
void free_table_scores(List *l);
List *tri_max(List *l);

#endif