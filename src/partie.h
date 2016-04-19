//
// Created by thibaut on 16/03/16.
#include <clutter/clutter.h>

#include "bonus.h"
#include "affichage.h"


#ifndef ALPHA_SNAKE_PARTIE_H
#define ALPHA_SNAKE_PARTIE_H



typedef struct _Map Map;
typedef struct _Partie Partie;


Snake * partie_snake(Partie *p);
void partie_set_snake(Partie *p,Snake *s);
Snake * partie_schlanga(Partie *p);
void partie_set_schlanga(Partie *p,Snake *s);
Bonus * partie_bonus(Partie *p);
void partie_set_bonus(Partie *p,Bonus *s);
Map *create_map(int width, int height);
void free_map(Map *map);
int map_width(Map *map);
int map_height(Map *map);


Partie *create_partie();
void free_partie(Partie *partie);
Map *partie_map(Partie *partie);

void init_partie(Partie *partie, ClutterScript *ui, int nb_snakes, int width, int height);
void init_pseudo(Partie *p, int argc, char **argv);

gboolean timeout_tick_cb(gpointer data);

#endif //ALPHA_SNAKE_PARTIE_H
