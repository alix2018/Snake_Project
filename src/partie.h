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


Snake * partie_snake(Partie *p);
void partie_set_snake(Partie *p,Snake *s);
Snake * partie_schlanga(Partie *p);
void partie_set_schlanga(Partie *p,Snake *s);
Bouf * partie_bouf(Partie *p);
void partie_set_bouf(Partie *p,Bouf *s);
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
