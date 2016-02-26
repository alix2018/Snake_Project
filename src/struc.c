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
	ListeSnake suivant;/*la suite de la liste*/
	ListeSnake precedent; /*la fin de la liste*/
};

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
	ListeSnake tete;
	ListeSnake dernier;
	int longueur;
	Direction direction;
};

/* Fonction de base de coord */
/**
 * @brief      A partir de deux integer renvoie un couple de type coordonnée
 *
 * @param[in]  x     l'integer représentant l'axe des abscisse 
 * @param[in]  y     l'integer représentant l'axe des ordonnée
 *
 * @return     renvoie un couple de type coordonnée 
 */
Coord coord_from_xy(int x,int y)
{
	Coord res;
	res.x = x;
	res.y = y;
	return res;
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

/*Fonctions de base de ListeSnake*/


/**
 * @brief      construit en mémoire une liste représentant le snake
 *
 * @param[in]  c      la position du couple de type coordonnée
 * @param[in]  liste  la suite de la liste (NULL si on crée une liste à un élément)
 *
 * @return     une liste augmenté ou l'on a ajouté c au début de la liste
 */
ListeSnake cons_liste_snake_debut(Coord c, ListeSnake liste)
{
	ListeSnake res = malloc(sizeof(struct maillon_snake));
	res->coord = c;
	res->precedent = NULL;
	res->suivant = liste;
	return res;
}

/**
 * @brief      construit en mémoire une liste représentant le snake
 *
 * @param[in]  c      la position du couple de type coordonnée
 * @param[in]  liste  la suite de la liste (NULL si on crée une liste à un élément)
 *
 * @return     une liste augmenté ou l'on a ajouté c au début de la liste
 */
ListeSnake cons_liste_snake_fin(Coord c, ListeSnake liste)
{
	ListeSnake dernier = malloc(sizeof(struct maillon_snake));
	dernier->coord = c;

	if(liste == NULL)
	{
		dernier->precedent = NULL;
		dernier->suivant = NULL;
		return dernier;
	}
	else
	{
		ListeSnake res = liste;
		while(liste->suivant != NULL)
		{
			liste=liste->suivant;
		}
		liste->suivant = dernier;
		dernier->precedent = liste;
		dernier->suivant = NULL;
		return res;
	}
}

/**
 * @brief      Libère en mémoire toute la liste
 *
 * @param[in]  liste  la liste à libérer
 */
void free_liste_snake(ListeSnake liste)
{
	if(liste->suivant==NULL){
		free(liste);
	}
	else{
		free_liste_snake(liste->suivant);
		free(liste);
	}
}

/**
 * @brief      Affiche sur la console la liste complête sous la forme (x1,y1) - (x2,y2) - (x3,y3) ...
 *
 * @param[in]  liste  la liste à afficher
 */
void print_liste_snake(ListeSnake liste)
{
	while(liste != NULL)
	{
		printf(" (%d, %d)", liste->coord.x, liste->coord.y);
		liste = liste->suivant;
	}
	printf("\n");
}

/**
 * @brief      Récupère les coordonnées de la tête d'une liste
 *
 * @param[in]  l     la liste avec une tête
 *
 * @return     Renvoie les coordonnées de la tête
 */
Coord liste_snake_coord(ListeSnake l)
{
	return l->coord;
}

/* Fonctions de base de Snake */

/**
 * @brief      Genère un snake de différente taille avec une position initial et un angle de direction
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
	Snake *res = (Snake *)malloc(sizeof(Snake));
	(*res).longueur = longueur;
	(*res).direction = dir;
	ListeSnake ls = NULL;
	for( i = 0 ; i < longueur ; i++ )
	{
		switch(dir)
		{
			case GAUCHE :
				ls = cons_liste_snake_fin(c, ls);
				c.x += 1;
				break;
			case DROITE :
				ls = cons_liste_snake_fin(c, ls);
				c.x -= 1;
				break;
			case HAUT :
				ls = cons_liste_snake_fin(c, ls);
				c.y += 1;
				break;
			case BAS : 
				ls = cons_liste_snake_fin(c, ls);
				c.y -= 1;
				break;
			default:
				ls = cons_liste_snake_fin(c, ls);
		}
	}

	(*res).tete=ls;

	while( ls->suivant != NULL )
	{
		ls=ls->suivant;
	}
	(*res).dernier=ls;

	return res;
}
/**
 * @brief      Libère en mémoire le snake
 *
 * @param      snake  le snake à libérer
 */
void free_snake(Snake *snake)
{
	free_liste_snake((*snake).tete);
	free(snake);
}

/**
 * @brief      Affiche un snake sur la console sous la forme L : #longeur - Direction : - La liste
 *
 * @param      snake  le snake à afficher
 */
void print_snake(Snake *snake)
{
	printf("# %d - %d : ", (*snake).longueur, (*snake).direction);
	print_liste_snake((*snake).tete);
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
	return (*snake).tete->coord;
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
	int res = 0;
	Snake s = *snake;
	while(s.tete != NULL)
	{
		s.tete = s.tete->suivant;
		res += 1;
	}
	return res;
}

/**
 * @brief      Récupère le dernier élément de la liste 
 *
 * @param      snake  le snake avec une queue
 *
 * @return     Renvoie une liste comportant un élément : la fin de queue
 */
ListeSnake snake_dernier(Snake *snake)
{
	return (*snake).dernier;
}

/**
 * @brief      Récupère tout la liste en partant de la tête  
 *
 * @param      snake  le snake
 *
 * @return     Renvoie la liste complête du snake
 */
ListeSnake snake_premier(Snake *snake)
{
	return (*snake).tete;
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
	return (*snake).direction;
}

/**
 * @brief      Change la direction futur du snake
 *
 * @param      snake  le snake à changer
 * @param[in]  dir    La direction du déplacement
 */
void snake_set_direction(Snake *snake, Direction dir)
{
	(*snake).direction = dir;
}

/**
 * @brief      Déplace l'intégralité du snake
 *
 * @param      snake  le snake à déplacer
 * @param[in]  dir    La direction du déplacement
 */
void snake_forward(Snake *snake)
{

	ListeSnake ls = (*snake).dernier;

	while(ls->precedent != NULL){
		ls->coord = ls->precedent->coord;
		ls = ls->precedent;
	}

	switch((*snake).direction)
	{
		case DROITE :
			ls->coord.x += 1;
			break;
		case GAUCHE :
			ls->coord.x -= 1;
			break;
		case HAUT :
			ls->coord.y -= 1;
			break;
		case BAS :
			ls->coord.y += 1;
			break;
		default:
			printf("Problème de direction...\n");
	}

	(*snake).tete=ls;
}
/*
int main(int argc, char const *argv[])
{

	Coord c1 = coord_from_xy(2, 8);
	Coord c2 = coord_from_xy(10, 10);

	ListeSnake ls = NULL;
	ls = cons_liste_snake_debut(c1, ls);
	ls = cons_liste_snake_fin(c2, ls);
	print_liste_snake(ls);
	free_liste_snake(ls);
	
	Snake *s = create_snake(8, c1, HAUT);

	print_snake(s);
	printf("%d\n", snake_longueur(s));
	printf("%d\n", snake_longueur(s));

	snake_set_direction(s, BAS);
	print_snake(s);

	snake_forward(s);
	print_snake(s);
	
	free_snake(s);

	return 0;
}
*/