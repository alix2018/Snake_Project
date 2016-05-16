/**
 * @file      partie.h
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Toutes les fonctions permettant de créer et lancer une partie.
 * @details   ---
 */


#ifndef ALPHA_SNAKE_PARTIE_H
#define ALPHA_SNAKE_PARTIE_H


typedef struct _Map Map;
typedef struct _Partie Partie;

#include <clutter/clutter.h>
#include "struc.h"
#include "bonus.h"
#include "config.h"

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
void remove_advanced_bonus(Partie *p, Bonus *b);

void init_partie(Partie *partie, ClutterScript *ui);
void init_pseudo(Partie *p, int argc, char **argv);

gboolean timeout_tick_cb(gpointer data);

#endif //ALPHA_SNAKE_PARTIE_H
