#include <clutter/clutter.h>
#include "struc.h"

#ifndef ALPHA_SNAKE_AFFICHAGE_H
#define ALPHA_SNAKE_AFFICHAGE_H

struct snake_actor {
    ClutterActor *rectangle;
    Direction direction;
};

void init_affichage(ClutterScript *ui, struct snake_actor *sa);

#endif //ALPHA_SNAKE_AFFICHAGE_H
