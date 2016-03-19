//
// Created by thibaut on 16/03/16.
//

#include "partie.h"
#include "struc.h"
#include "bonus.h"

struct _Partie
{
    Snake *snake;
    Snake *schlanga;
    Bouf *nourriture;
};

Partie *create_partie(Snake *snake, Snake *schlanga, Coord coord_bouf)
{
    Partie *res;

    res = malloc(sizeof(struct _Partie));
    res->snake = snake;
    res->schlanga = schlanga;
    res->nourriture = bouf_new(coord_bouf.x, coord_bouf.y);

    return res;
}

void free_partie(Partie *partie)
{
    free(partie->nourriture);
    free(partie);
}
