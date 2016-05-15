/**
 * @file      struc.c
 * @author    alpha-snake
 * @version   1
 * @date      25/02/2016
 * @brief     Toutes les fonctions permettant d'utiliser la structure et de jouer
 * @details   ---
 */

#include <string.h>
#include <time.h>
#include "struc.h"
#include "list.h"
#include "ia.h"
#include <math.h>

/**
 * @brief      La structure du snake 
 * 
 * @use Snake
 * @param[in]  tete    		   La tête de la liste
 * @param[in]  dernier  	   Le dernier élément de la liste
 * @param[in]  longueur  	   La longueur de la liste
 * @param[in]  direction  	   La direction du snake
 */
struct _snake
{
	List *liste_snake;
	int longueur;
	Direction direction;
	char *pseudo;
	int is_bot;
	char * script_name;
	int num_ia;
	int indic_duree;
};



/*********************************/
/* Fonctions de base de tabsnake */
/********************************/
/**
 * @brief   Crée un tableau de snake.
 *
 * @return  Le tableau de snake.
 */
TabSnakes *create_tab_snakes()
{
	TabSnakes *res;

	res = malloc(sizeof(TabSnakes));
	res->nb_snakes = 0;
	res->taille_snakes = 2;
	res->snakes = malloc(res->taille_snakes * sizeof(TabSnakes *));
	return res;
}


/**
 * @brief   Libère la mémoire le tableau de snake.
 *
 * @param[in]    ts  Le tableau de snake à supprimer.
 */
void free_tab_snakes(TabSnakes *ts)
{
	int i;

	for (i = 0; i < ts->nb_snakes; i++)
	{
		free_snake(ts->snakes[i]);
	}

	free(ts->snakes);
	free(ts);
}



/**
 * @brief   Ajoute d'un snake dans le tableau de snake
 *
 * @param[in]    ts     Le gestionnaire de collisions.
 * @param[in]    obj    L'objet à ajouter.
 *
 * @return  Le Snake crée lors de l'ajout de obj, ou le
 *          Snake qui gère obj si obj est déjà géré par gc.
 */
Snake *tab_snakes_add_object(TabSnakes *ts, Snake *obj)
{
	int i;

	for (i = 0; i < ts->nb_snakes; i++)
	{
		if (ts->snakes[i] == obj) // pas de duplicat
			return ts->snakes[i];
	}

	ts->nb_snakes++;
	if (ts->nb_snakes > ts->taille_snakes)
	{
		ts->taille_snakes *= 2;
		ts->snakes = realloc(ts->snakes,
						   ts->taille_snakes * sizeof(Snake *));
	}

	ts->snakes[ts->nb_snakes - 1] = obj;

	return ts->snakes[ts->nb_snakes - 1];
}


/**
 * @brief   Supprime le snake dans le tableau de snakes.
 *
 * @param[in]    ts     Le gestionnaire de collisions.
 * @param[in]    obj    L'objet à supprimer.
 */
void tab_snakes_remove_object(TabSnakes *ts, Snake *obj)
{
	int i = 0;

	while (i < ts->nb_snakes)
	{
		if (ts->snakes[i] == obj)
		{
			free_snake(ts->snakes[i]);
			ts->snakes[i] = ts->snakes[ts->nb_snakes - 1];
			ts->snakes[ts->nb_snakes - 1] = NULL;
			ts->nb_snakes--;
			return;
		}
	}
}
/**
 * @brief   Le nombre de snakes (obselete)
 *
 * @param[in]    ts    le tableau de snake.
 */
int tab_snakes_length(TabSnakes *ts)
{
	return ts->nb_snakes;
}
/**
 * @brief   La place en mémoire (obselete).
 *
 * @param[in]    ts    le tableau de snake.
 */
int tab_snakes_memory_length(TabSnakes *ts)
{
	return ts->taille_snakes;
}


/********************************/
/* Fonctions de base de coord */
/********************************/
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

/**
 * @brief      Calcul la distance entre deux coordonnées
 *
 * @param[in]  c1  Coord à vérifier
 * @param[in]  c2  Coord à vérifier
 *
 * @return    double de la distance
 */
double coord_distance(Coord c1, Coord c2)
{
	return sqrt( pow((c1.x - c2.x),2)  + pow(c1.y -  c2.y,2));
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
	res->is_bot = 0;
	res->script_name = NULL;

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


Snake *create_snake_bot(int longueur, Coord c, Direction dir,char * str)
{
	Snake * res = create_snake(longueur,c,dir);
	res->script_name = str;
	res->is_bot = 1;
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

void snake_set_longueur(Snake *snake,int nl)
{
    snake->longueur = nl;
}

/**
 * @brief      Récupère le num associé à l'ia
 *
 * @param      snake  Le snake
 *
 * @return     Renvoie le num associé à l'ia
 */
int snake_num_ia(Snake *snake)
{
    return snake->num_ia;
}

void snake_set_num_ia(Snake *snake,int num_ia)
{
    snake->num_ia = num_ia;
}

/**
 * @brief      Récupère l'indic de durée, utile pour les ia
 *
 * @param      snake  Le snake avec une taille
 *
 * @return     Renvoie l'indic de durée
 */
int snake_indic_duree(Snake *snake)
{
    return snake->longueur;
}

void snake_set_indic_duree(Snake *snake,int d)
{
    snake->indic_duree = d;
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
 * @brief      Récupère toute la liste en partant de la tête
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

int snake_set_pos(Snake *snake,Coord pos,Config * config )
{
	Node ls = list_first_node(snake_liste_snake(snake));
	Direction dir = HAUT;
	Coord * c;
	if(pos.x >= 0 && pos.x <= config->width && pos.y >= 0 && pos.y <= config->height )
	{
		int dx =0;
		int dy =0;
		if(pos.x == 0)
        {
            dx=-1;
            snake_set_direction(snake,DROITE);
        }
        else if(pos.x ==config->width)
        {
            dx = 1;
            snake_set_direction(snake,GAUCHE);

        }
        else if(pos.y == 0)
        {
            dy = -1;
            snake_set_direction(snake,BAS);
        }
        else if(pos.y == config->height)
        {
            dy = 1;
            snake_set_direction(snake,HAUT);
        }
		int i = 0;
		while (ls != NULL)
		{
            c =  malloc(sizeof(Coord));
			*c = coord_from_xy(pos.x+dx*i,pos.y+dy*i);
			node_set_elt(ls,c);
			i++;
			ls = node_next(ls);
		}

		return 1;
	}
	else
	{
		return 0;
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



int snake_is_bot(Snake *snake)
{
	return snake->is_bot;
}

char * snake_script_name(Snake * snake)
{
	return snake->script_name;
}