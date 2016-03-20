/**
 * @file         struc.c
 * @author    alpha-snake
 * @version   1
 * @date       25/02/2016
 * @brief      Toutes les fonctions permetant d'utiliser la structure et de jouer
 * @details    --- 
 */

#include <string.h>
#include <time.h>
#include "struc.h"
#include "list.h"


/**
 * @brief      la structure du snake 
 * 
 * @use Snake
 * @param[in]  tete    		   la tête de la liste
 * @param[in]  dernier  	   le dernier élément de la liste
 * @param[in]  longueur  	   la longueur de la liste
 * @param[in]  direction  	   la direction du snake
 */
struct snake
{
	List *liste_snake;
	int longueur;
	Direction direction;
};



/* Fonctions de base de coord */

/**
 * @brief      A partir de deux integer renvoie un couple de type coordonnée
 *
 * @param[in]  x     l'integer représentant l'axe des abscisse 
 * @param[in]  y     l'integer représentant l'axe des ordonnée
 *
 * @return     renvoie un couple de type coordonnée 
 */
Coord coord_from_xy(int x, int y)
{
	Coord c;
	c.x = x;
	c.y = y;

	return c;
}


/**
 * @brief      affiche un couple dans la console de la forme (x,y)
 *
 * @param[in]  c     la coordonnée à afficher
 */
void print_coord(Coord c)
{
	printf("(%d,%d)\n", c.x, c.y);
}


/**
 * @brief      Test l'égalité entre deux coordonnées
 *
 * @param[in]  c1  coord à vérifier
 * @param[in]  c2  coord à vérifier
 *
 * @return     1 si les coord sont égales, 0 sinon
 */
int coord_egales(Coord c1, Coord c2){
	if(c1.x==c2.x && c1.y==c2.y){
		return 1;
	}
	else{
		return 0;
	}
}


/* Fonctions de base de Snake */

/**
 * @brief      Génère un snake de différente taille avec une position initiale et un angle de direction
 *
 * @param[in]  longueur  la taille du snake initiale
 * @param[in]  c         la position initiale
 * @param[in]  dir       l'angle de direction
 *
 * @return     Renvoie un snake
 */
Snake *create_snake(int longueur, Coord c, Direction dir)
{
	int i;
    Snake *res;
    List *ls = create_list();
    Coord *cur;
    Coord *tmp;

    res = malloc(sizeof(Snake));
    res->longueur = longueur;
    res->direction = dir;
    res->liste_snake = ls;

    cur = malloc(sizeof(Coord));
    *cur = c;

	for (i = 0; i < longueur; i++)
	{
		switch (dir)
		{
			case GAUCHE:
                list_add_last(ls, cur);
				cur->x += 1;
				break;
			case DROITE:
                list_add_last(ls, cur);
				cur->x -= 1;
				break;
			case HAUT:
                list_add_last(ls, cur);
				cur->y += 1;
				break;
			case BAS:
                list_add_last(ls, cur);
				cur->y -= 1;
				break;
			default:
                list_add_last(ls, cur);
		}
        tmp = cur;
        cur = malloc(sizeof(Coord));
        *cur = *tmp;
	}

	return res;
}

/**
 * @brief      Libère en mémoire le snake
 *
 * @param      snake  le snake à libérer
 */
void free_snake(Snake *snake)
{
	free_list_fn(snake->liste_snake, free);
	free(snake);
}


static void print_coord_elt(void *coord_elt, void *data)
{
    Coord *coord = coord_elt;

    print_coord(*coord);
}

/**
 * @brief      Affiche un snake sur la console sous la forme L : #longeur - Direction : - La liste
 *
 * @param      snake  le snake à afficher
 */
void print_snake(Snake *snake)
{
	printf("# %d - %d : ", (*snake).longueur, (*snake).direction);
    list_foreach(snake->liste_snake, print_coord_elt, NULL);
}

/**
 * @brief      Récupère la position du snake
 *
 * @param      snake  le snake positionné
 *
 * @return     La position de type coordonnée
 */
Coord snake_pos(Snake *snake)
{
    Coord *res;

    res = node_elt(list_first_node(snake->liste_snake));

	return *res;
}

/**
 * @brief      Récupère la longueur d'un snake
 *
 * @param      snake  le snake avec une taille
 *
 * @return     Renvoie un integer représentant la longueur du snake
 */
int snake_longueur(Snake *snake)
{
    return snake->longueur;
}


/**
 * @brief      Récupère la liste du snake
 *
 * @param      snake  le snake avec une taille
 *
 * @return     Renvoie la liste représentant le corps du snake
 */
List *snake_liste_snake(Snake *snake)
{
    return snake->liste_snake;
}


/**
 * @brief      Récupère le dernier élément de la liste
 *
 * @param      snake  le snake avec une queue
 *
 * @return     Renvoie une liste comportant un élément : la fin de queue
 */
Node snake_dernier(Snake *snake)
{
	return list_last_node(snake->liste_snake);
}

/**
 * @brief      Récupère tout la liste en partant de la tête
 *
 * @param      snake  le snake
 *
 * @return     Renvoie la liste complête du snake
 */
Node snake_premier(Snake *snake)
{
	return list_first_node(snake->liste_snake);
}

/**
 * @brief      La direction de la tête au prochain coup
 *
 * @param      snake  le snake
 *
 * @return     Renvoie la direction du prochain coup
 */
Direction snake_direction(Snake *snake)
{
	return snake->direction;
}

/**
 * @brief      Change la direction future du snake
 *
 * @param      snake  le snake à changer
 * @param[in]  dir    La direction du déplacement
 */
void snake_set_direction(Snake *snake, Direction dir)
{
	snake->direction = dir;
}


/**
 * @brief      Affecte la nouvelle liste au snake
 *
 * @param      snake  le snake auquel on veut associer la liste
 * @param[in]  ls     la liste à affecter au snake
 */
void snake_set_liste(Snake *snake, List *ls)
{
	snake->liste_snake = ls;
}

/**
 * @brief      Déplace l'intégralité du snake
 *
 * @param      snake  le snake à déplacer
 * @param[in]  dir    La direction du déplacement
 */
void snake_forward(Snake *snake)
{
    Coord *coord_tete;
    Coord *coord_cou = node_elt(list_first_node(snake->liste_snake));

	free(list_pop_last(snake->liste_snake));

    coord_tete = malloc(sizeof(Coord));
    *coord_tete = *coord_cou;
	switch(snake->direction)
	{
		case DROITE :
			coord_tete->x += 1;
			break;
		case GAUCHE :
			coord_tete->x -= 1;
			break;
		case HAUT :
			coord_tete->y -= 1;
			break;
		case BAS :
			coord_tete->y += 1;
			break;
		default:
			printf("Problème de direction...\n");
	}

    list_add_first(snake->liste_snake, coord_tete);
}


/**
 * @brief      Augmente le snake d'un noeud
 *
 * @param      snake  le snake à agrandir
 */
void snake_increase(Snake *snake)
{
    Coord *c = malloc(sizeof(Coord));
    *c = *((Coord *) list_get_last(snake->liste_snake));
    list_add_last(snake->liste_snake, c);
}
