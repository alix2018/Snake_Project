
#include <clutter/clutter.h>
#include "struc.h"

#ifndef ALPHA_SNAKE_BONUS_H
#define ALPHA_SNAKE_BONUS_H


typedef struct _bouf Bouf;
typedef struct _bouf_actor BoufActor;

//Créé un nouvelle emplacement mémoir pour la bouf
Bouf *bouf_new(int x, int y);

//Créé les coordonnés de la bouf sur la map
void bouf_update(Bouf *bouf, int x, int y);
Coord bouf_coord(Bouf *bouf);

//Créé le BoufActor
BoufActor *create_bouf_actor(ClutterActor *parent, Bouf *b, ClutterColor *color);
void free_bouf_actor(BoufActor *b);
Bouf *bouf_actor_bouf(BoufActor *bouf_actor);

//Met à jour les coord du clutteractor
void bouf_actor_update(BoufActor *ba);

#endif //ALPHA_SNAKE_BONUS_H
