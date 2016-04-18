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
 * @brief      La structure du snake 
 * 
 * @use Snake
 * @param[in]  tete    		   La tête de la liste
 * @param[in]  dernier  	   Le dernier élément de la liste
 * @param[in]  longueur  	   La longueur de la liste
 * @param[in]  direction  	   La direction du snake
 */
struct snake
{
	List *liste_snake;
	int longueur;
	Direction direction;
	char *pseudo;
};



/* Fonctions de base de coord */

/**
 * @brief      A partir de deux integer renvoie un couple de type coordonnée
 *
 * @param[in]  x     L'integer représentant l'axe des abscisses 
 * @param[in]  y     L'integer représentant l'axe des ordonnées
 *
 * @return     Renvoie un couple de type coordonnée 
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
 * @brief      Teste l'égalité entre deux coordonnées
 *
 * @param[in]  c1  Coord à vérifier
 * @param[in]  c2  Coord à vérifier
 *
 * @return     1 si les Coord sont égales, 0 sinon
 */
int coord_egales(Coord c1, Coord c2)
{
	return c1.x == c2.x && c1.y == c2.y;
}


/* Fonctions de base de Snake */

/**
 * @brief       Génère un snake de différentes tailles avec une position initiale
				et un angle de direction
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

    cur = &c;

	for (i = 0; i < longueur; i++)
	{
		tmp = cur;
		cur = malloc(sizeof(Coord));
		*cur = *tmp;
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
 * @param      snake  Le snake à afficher
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
 * @param      snake  Le snake avec une taille
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
 * @param      snake  Le snake avec une taille
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
 * @brief      Récupère toutE la liste en partant de la tête
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
 * @brief      Change la direction du snake si le changement de direction
 *             n'implique pas que le snake se mange lui-même.
 *
 * @param      snake  le snake à changer
 * @param[in]  dir    La direction du déplacement
 *
 * @return  1 si le changement de direction a été possible, 0 sinon.
 */
int snake_set_direction(Snake *snake, Direction dir)
{
    Coord *coord_2eme = node_elt(node_next(list_first_node(snake->liste_snake)));
    Coord pos = snake_pos(snake);
    int dx = pos.x - coord_2eme->x;
    int dy = pos.y - coord_2eme->y;

    if (dx == 0 && dy == -1 && dir == BAS) // HAUT
    {
        return 0;
    }
    else if (dx == 0 && dy == 1 && dir == HAUT) // BAS
    {
        return 0;
    }
    else if (dx == -1 && dy == 0 && dir == DROITE) // GAUCHE
    {
        return 0;
    }
    else if (dx == 1 && dy == 0 && dir == GAUCHE) // DROITE
    {
        return 0;
    }
    else
    {
        snake->direction = dir;
        return 1;
    }
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

char *snake_pseudo(Snake *snake)
{
	return snake->pseudo;
}

void snake_set_pseudo(Snake *snake, char *pseudo)
{
	snake->pseudo = pseudo;
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
 * @param      snake  Le snake à agrandir
 */
void snake_increase(Snake *snake)
{
    Coord *c = malloc(sizeof(Coord));
    *c = *((Coord *) list_get_last(snake->liste_snake));
    list_add_last(snake->liste_snake, c);
	snake->longueur++;
}
