//
// Created by thibaut on 16/03/16.



#ifndef ALPHA_SNAKE_PARTIE_H
#define ALPHA_SNAKE_PARTIE_H

#include <clutter/clutter.h>
#include "struc.h"
#include "bonus.h"
#include "config.h"


typedef struct _Map Map;
typedef struct _Partie Partie;


TabSnakes * partie_tab(Partie *p);
Snake * partie_player(Partie *p);
void partie_set_player(Partie *p,Snake *s);
//Snake * partie_snake(Partie *p);
//void partie_set_snake(Partie *p,Snake *s);
//Snake * partie_schlanga(Partie *p);
//void partie_set_schlanga(Partie *p,Snake *s);
TabBonus * partie_tab_bonus(Partie *p);
//void partie_set_bonus(Partie *p,Bonus *s);
Map *create_map(int width, int height);
void free_map(Map *map);
int map_width(Map *map);
int map_height(Map *map);


Partie *create_partie();
void partie_set_config(Partie *p,Config * c);
Config * partie_config(Partie *p);
void free_partie(Partie *partie);
Map *partie_map(Partie *partie);
int partie_duree(Partie *partie);
int partie_set_duree(Partie *partie, int duree);
void partie_add_bonus(Partie *partie,Bonus * bonus);


void init_partie(Partie *partie, ClutterScript *ui);
void init_pseudo(Partie *p, int argc, char **argv);

gboolean timeout_tick_cb(gpointer data);

#endif //ALPHA_SNAKE_PARTIE_H
