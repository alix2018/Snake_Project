/**
 * @file         struc.h
 * @author    alpha-snake
 * @version   1
 * @date       25/02/2016
 * @brief      Toutes les fonctions permetant d'utiliser la structure et de jouer
 * @details    --- 
 */
#ifndef _STRUCT_
#define _STRUCT_

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief    liste de Snake
 */
typedef struct maillon_snake *ListeSnake;

typedef struct snake Snake;
/**
 * 
 */
typedef struct
{
	int x;
	int y;
} Coord;

typedef enum
{
	/* Sens trigo */
	GAUCHE = 180, DROITE = 0, BAS = 270, HAUT = 90, ARRET = -1
} Direction;

/* Fonction de base de coord */

Coord coord_from_xy(int x,int y);
void print_coord(Coord c);
int coord_egales(Coord c1, Coord c2);

/*Fonctions de base de ListeSnake*/


ListeSnake cons_liste_snake_debut(Coord c, ListeSnake liste);
ListeSnake cons_liste_snake_fin(Coord c, ListeSnake liste);
void free_liste_snake(ListeSnake liste);
void print_liste_snake(ListeSnake liste);

Coord liste_snake_coord(ListeSnake l);
ListeSnake liste_snake_suivant(ListeSnake liste);
ListeSnake liste_snake_precedent(ListeSnake liste);

int coord_is_in_liste_snake( Coord c,ListeSnake liste);


/* Fonctions de base de Snake */

Snake *create_snake(int longueur, Coord c, Direction dir);
void free_snake(Snake *snake);
void print_snake(Snake *snake);

Coord snake_pos(Snake *snake);
int snake_longueur(Snake *snake);
ListeSnake snake_dernier(Snake *snake);
ListeSnake snake_premier(Snake *snake);
Direction snake_direction(Snake *snake);
void snake_set_direction(Snake *snake, Direction dir);
void snake_set_liste(Snake *snake, ListeSnake *ls);

void snake_forward(Snake *snake);
void snake_forward_ia1(Snake *snake_ia, Snake *snake, Coord bouf);

void snake_increase(Snake *snake);

#endif