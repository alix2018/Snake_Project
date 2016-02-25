/**
 * @file         struc.c
 * @author    alpha-snake
 * @version   1
 * @date       25/02/2016
 * @brief      Toutes les fonctions permetant d'utiliser la structure et de jouer
 * @details    --- 
 */

#include "struc.h"
/**
 * @brief    liste de coordonnées
 */
struct maillon_snake
{
	Coord coord;/*la coordonnée dans la liste*/
	Snake suivant;/*la suite de la liste*/
	Snake precedent; /*la fin de la liste*/
};

struct snake
{
	ListeSnake tete;
	ListeSnake dernier;
	int longueur;
	Direction direction;
};