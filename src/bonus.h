
#include <clutter/clutter.h>
#include "struc.h"

#ifndef ALPHA_SNAKE_BONUS_H
#define ALPHA_SNAKE_BONUS_H


typedef struct _bonus Bonus;
typedef struct _bonus_actor BonusActor;

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
