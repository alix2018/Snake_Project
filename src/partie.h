//
// Created by thibaut on 16/03/16.
#include <clutter/clutter.h>
#include "struc.h"
#include "bonus.h"
#include "affichage.h"


#ifndef ALPHA_SNAKE_PARTIE_H
#define ALPHA_SNAKE_PARTIE_H



typedef struct _Map Map;
typedef struct _Partie Partie;

Map *create_map(int width, int height);
void free_map(Map *map);
int map_width(Map *map);
int map_height(Map *map);

Partie *create_partie();
void free_partie(Partie *partie);
Map *partie_map(Partie *partie);

void init_partie(Partie *partie, ClutterScript *ui, int width, int height);

gboolean timeout_tick_cb(gpointer data);

#endif //ALPHA_SNAKE_PARTIE_H
