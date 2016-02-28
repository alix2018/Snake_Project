#include <clutter/clutter.h>
#include "struc.h"

#ifndef ALPHA_SNAKE_AFFICHAGE_H
#define ALPHA_SNAKE_AFFICHAGE_H

/**
 * Cette structure contient les ClutterActor nécessaires à l'affichage du snake.
 */
typedef struct _snake_actor SnakeActor;

SnakeActor *create_snake_actor(ClutterActor *parent, Snake *s);
void free_snake_actor(SnakeActor *sa);

void snake_actor_update(SnakeActor *sa);

void init_view(ClutterScript *ui, int width, int height, Direction direction,
               int size, Coord pos);




#endif //ALPHA_SNAKE_AFFICHAGE_H
