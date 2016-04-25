

#ifndef ALPHA_SNAKE_BONUS_H
#define ALPHA_SNAKE_BONUS_H

#include <clutter/clutter.h>
#include "struc.h"

typedef struct _bonus Bonus;
typedef struct _bonus_actor BonusActor;


typedef struct _tabbonus TabBonus;

/**
 * @brief      La structure de tabsnake
 *
 * @use Bonus
 * @param[in]  bonus 		    Le tableau de tout les bonus
 * @param[in]  nb_bonus  	    Le nombre de snake
 * @param[in]  taille_bonus	La taille en mémoire
 */
struct _tabbonus
{
    Bonus **bonus; // pour acceder au tableau de snake ts->bonus
    int nb_bonus;
    int taille_bonus;
};

/*Fonctions de base de TabBonus*/

TabBonus *create_tab_bonus();
void free_tab_bonus(TabBonus *ts);
Bonus *tab_bonus_add_object(TabBonus *ts, Bonus *obj);
void tab_bonus_remove_object(TabBonus *ts, Bonus *obj);
int tab_bonus_length(TabBonus *ts);
int tab_bonus_memory_length(TabBonus *ts);


//Créé un nouvelle emplacement mémoir pour la bonus
Bonus *bonus_new(int x, int y);

//Créé les coordonnés de la bonus sur la map
void bonus_update(Bonus *bonus, int x, int y);
Coord bonus_coord(Bonus *bonus);

//Créé le BonusActor
BonusActor *create_bonus_actor(ClutterActor *parent, Bonus *b, ClutterColor *color);
void free_bonus_actor(BonusActor *b);
Bonus *bonus_actor_bonus(BonusActor *bonus_actor);

//Met à jour les coord du clutteractor
void bonus_actor_update(BonusActor *ba);

#endif //ALPHA_SNAKE_BONUS_H
