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
	printf("(%d,%d)\n",c.x,c.y);
}


/* Fonctions de base de ListeSnake */

/**
 * @brief      construit en mémoire une liste représentant le snake
 *
 * @param[in]  c      la position du couple de type coordonnée
 * @param[in]  liste  la suite de la liste (NULL si on crée une liste à un seul élément)
 *
 * @return     une liste augmentée avec le nouvel élement en début de liste
 */
ListeSnake cons_liste_snake(Coord c, ListeSnake liste)
{

	ListeSnake res;
	res = malloc (sizeof(struct maillon_snake));
	res -> coord = c;
	res -> suivant = liste;
	res -> precedent = NULL;

	return res;
}

/**
 * @brief      construit en mémoire une liste représentant le snake
 *
 * @param[in]  c      la position du couple de type coordonnée
 * @param[in]  liste  la suite de la liste (NULL si on crée une liste à un seul élément)
 *
 * @return     une liste augmentée avec le nouvel élement en fin de liste
 */
ListeSnake add_liste_snake(Coord c, ListeSnake liste)
{
	ListeSnake res;

	if ( liste == NULL )
	{
		res = cons_liste_snake(c,res);
	}

	else 
	{
		res = liste;

		while ( res -> suivant != NULL ) 
		{
			res = res -> suivant;
		}

		res -> suivant = cons_liste_snake(c,res); /* ne s'occupe pas de changer le pointeur sur la nouvelle queue du snake */
	}

	return res;	
}


/**
 * @brief      Libère en mémoire toute la liste
 *
 * @param[in]  liste  la liste à libérer
 */
void free_liste_snake(ListeSnake liste) 
{
	ListeSnake temp;

	while ( liste != NULL ) 
	{
		temp = liste -> suivant;
		free(liste);
		liste=temp;
	}

}


/**
 * @brief      Affiche sur la console la liste complête sous la forme (x1,y1) - (x2,y2) - (x3,y3) ...
 *
 * @param[in]  liste  la liste à afficher
 */
void print_liste_snake(ListeSnake liste)
{

	if ( liste == NULL ) {
		printf("liste nulle");
	}

	else 
	{
		printf("(%d,%d)", liste -> coord.x, liste -> coord.y);
		liste = liste -> suivant;

		while ( liste != NULL ) {
			printf(" - (%d,%d)", liste -> coord.x, liste -> coord.y);
			liste = liste -> suivant;
		}

	printf("\n");

	}

}


/**
 * @brief      Récupère les coordonnées de la tête d'une liste
 *
 * @param[in]  l     la liste avec une tête
 *
 * @return     Renvoie les coordonnées de la tête
 */
Coord liste_snake_coord(ListeSnake liste)
{
	Coord coord = liste -> coord;

	return coord;
}


/* Fonctions de base de Snake */

/**
 * @brief      Genère un snake de différente taille avec une position initiale et un angle de direction
 *
 * @param[in]  longueur  la taille du snake initiale
 * @param[in]  c         la position initiale
 * @param[in]  dir       l'angle de direction
 *
 * @return     Renvoie un snake
 */
Snake *create_snake(int longueur, Coord c, Direction dir);


/**
 * @brief      Libère en mémoire le snake
 *
 * @param      snake  le snake à libérer
 */
void free_snake(Snake *snake);
/**
 * @brief      Affiche un snake sur la console sous la forme L : #longeur - Direction : - La liste
 *
 * @param      snake  le snake à afficher
 */
void print_snake(Snake *snake);

/**
 * @brief      Récupère la position du snake
 *
 * @param      snake  le snake positionné
 *
 * @return     La position de type coordonnée
 */
Coord snake_pos(Snake *snake);
/**
 * @brief      Récupère la longueur d'un snake
 *
 * @param      snake  le snake avec une taille
 *
 * @return     Renvoie un integer représentant la longueur du snake
 */
int snake_longueur(Snake *snake);
/**
 * @brief      Récupère le dernier élément de la liste 
 *
 * @param      snake  le snake avec une queue
 *
 * @return     Renvoie une liste comportant un élément : la fin de queue
 */
ListeSnake snake_dernier(Snake *snake);
/**
 * @brief      Récupère tout la liste en partant de la tête  
 *
 * @param      snake  le snake
 *
 * @return     Renvoie la liste complête du snake
 */
ListeSnake snake_premier(Snake *snake);
/**
 * @brief      La direction de la tête au prochain coup
 *
 * @param      snake  le snake 
 *
 * @return     Renvoie la direction du prochain coup
 */
Direction snake_direction(Snake *snake);
/**
 * @brief      Change la direction futur du snake
 *
 * @param      snake  le snake à changer
 * @param[in]  dir    La direction du déplacement
 */
void snake_set_direction(Snake *snake, Direction dir);

/**
 * @brief      Déplace l'intégralité du snake
 *
 * @param      snake  le snake à déplacer
 * @param[in]  dir    La direction du déplacement
 */
void snake_forward(Snake *snake);

int main() {
	Coord coord = coord_from_xy(1,2);

	ListeSnake liste = add_liste_snake(coord,NULL);
	liste = add_liste_snake(coord,liste);
	print_liste_snake(liste);


	return 0;
}
