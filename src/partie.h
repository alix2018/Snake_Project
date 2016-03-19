//
// Created by thibaut on 16/03/16.
//

#ifndef ALPHA_SNAKE_PARTIE_H
#define ALPHA_SNAKE_PARTIE_H

#include "struc.h"

typedef struct _Partie Partie;

Partie *create_partie(Snake *snake, Snake *schlanga, Coord coord_bouf);
void free_partie(Partie *partie);

#endif //ALPHA_SNAKE_PARTIE_H
