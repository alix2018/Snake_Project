#include "config.h"
#include <stdlib.h>
Config * init_default_config()
{
    Config * res;
    res = malloc(sizeof(Config));
    res->nb_snakes = DEFAULT_NB_SNAKES;
    res->nb_bonus = DEFAULT_NB_BONUS;
    res->taille_snake = DEFAULT_TAILLE_SNAKE;
    res->taille_bot = DEFAULT_TAILLE_BOT;
    res->height = DEFAULT_HEIGHT;
    res->width = DEFAULT_WIDTH;
    res->type_partie = DEFAULT_TYPE_PARTIE;
    res->grid_size = DEFAULT_GRID_SIZE;
    res->collision = DEFAULT_COLLISION;
    res->advanced_bonus = DEFAULT_ADVANCED_BONUS;
    res->interval = DEFAULT_INTERVAL;
    return res;

}


void free_config(Config * c)
{
    free(c);
}

Config * init_config()
{
    Config * res = init_default_config();
    //TODO changer les params en fct de ce qu'on veut
    return res;
}