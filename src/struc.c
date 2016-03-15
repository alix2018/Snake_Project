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


/* Fonctions de base de ListeSnake */

/**
 * @brief      construit en mémoire une liste représentant le snake
 *
 * @param[in]  c      la position du couple de type coordonnée
 * @param[in]  liste  la suite de la liste (NULL si on crée une liste à un seul élément)
 *
 * @return     une liste augmentée avec le nouvel élement en début de liste
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
 * @return     une liste augmentée avec le nouvel élement en fin de liste
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
 * @brief      Affiche sur la console la liste complète sous la forme (x1,y1) - (x2,y2) - (x3,y3) ...
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


/**
 * @brief      Récupère le pointeur sur la liste suivante
 *
 * @param[in]  l     la liste avec une tête
 *
 * @return     Renvoie la liste suivante
 */
ListeSnake liste_snake_suivant(ListeSnake liste)
{
	return liste->suivant;
}


int coord_is_in_liste_snake( Coord c,ListeSnake liste)
{
	int res = 0;
	while(liste != NULL)
	{
		if( coord_egales(liste->coord,c))
		{
			res = 1;
		}
		liste = liste->suivant;
	}
	return res;
}


/**
 * @brief      Récupère le pointeur sur la liste précédent
 *
 * @param[in]  l     la liste avec une tête
 *
 * @return     Renvoie la liste précédente
 */
ListeSnake liste_snake_precedent(ListeSnake liste)
{
	return liste->precedent;
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

void snake_set_liste(Snake *snake, ListeSnake *ls)
{
	ListeSnake l = *ls;
	
	snake->tete = l;

	while(l->suivant != NULL)
	{
		l = l->suivant;
	}
	
	snake->dernier = l;
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
// Verifie le prochain mouvement du snake
void snake_verif_ia(Snake *snake_ia,Snake *snake,Direction futurdir) // TODO déplacement défensif
{
	Coord ctete_ia = snake_pos(snake_ia);
	ListeSnake ls = snake->tete;
	srand(time(NULL));
	/**switch (futurdir)
	{
		case HAUT :
			if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x-1,ctete_ia.y),ls))
			{
				// TODO change direction
				int i = rand()%3;  // 0
				if(i == 0) // GAUCHE
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x,ctete_ia.y-1),snake_ia->tete))
					{

						snake_verif_ia(snake_ia,snake,DROITE);
					}
					else
					{
						snake_set_direction(snake_ia,GAUCHE);
					}

				}
				else if( i == 1 )// DROITE
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x,ctete_ia.y+1),snake->tete))
					{

						snake_verif_ia(snake_ia,snake,GAUCHE);
					}
					else
					{
						snake_set_direction(snake_ia,DROITE);
					}
				}
				else if(i == 2) // BAS
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x+1,ctete_ia.y),snake->tete))
					{

						snake_verif_ia(snake_ia,snake,DROITE);
					}
					else
					{
						snake_set_direction(snake_ia,BAS);
					}
				}
			}
			break;
		case BAS :
			if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x+1,ctete_ia.y),ls))
			{
				// TODO change direction
				int i = rand()%3;  // 0
				if(i == 0) // GAUCHE
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x,ctete_ia.y-1),snake_ia->tete))
					{

						snake_verif_ia(snake_ia,snake,DROITE);
					}
					else
					{
						snake_set_direction(snake_ia,GAUCHE);
					}

				}
				else if( i == 1 )// DROITE
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x,ctete_ia.y+1),snake->tete))
					{

						snake_verif_ia(snake_ia,snake,GAUCHE);
					}
					else
					{
						snake_set_direction(snake_ia,DROITE);
					}
				}
				else if(i == 2) // HAUT
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x-1,ctete_ia.y),snake->tete))
					{

						snake_verif_ia(snake_ia,snake,GAUCHE);
					}
					else
					{
						snake_set_direction(snake_ia,HAUT);
					}
				}
			}

			break;
		case DROITE:
			if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x,ctete_ia.y+1),ls))
			{
				// TODO change direction
				int i = rand()%3;  // 0
				if(i == 0) // GAUCHE
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x,ctete_ia.y-1),snake_ia->tete))
					{

						snake_verif_ia(snake_ia,snake,HAUT);
					}
					else
					{
						snake_set_direction(snake_ia,GAUCHE);
					}

				}
				else if( i == 1 )// HAUT
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x-1,ctete_ia.y),snake->tete))
					{

						snake_verif_ia(snake_ia,snake,GAUCHE);
					}
					else
					{
						snake_set_direction(snake_ia,HAUT);
					}
				}
				else if(i == 2) // BAS
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x+1,ctete_ia.y),snake->tete))
					{

						snake_verif_ia(snake_ia,snake,DROITE);
					}
					else
					{
						snake_set_direction(snake_ia,BAS);
					}
				}
			};

			break;
		case GAUCHE :
			if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x,ctete_ia.y-1),ls))
			{
				// TODO change direction
				int i = rand()%3;  // 0
				if(i == 0) // HAUT
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x-1,ctete_ia.y),snake_ia->tete))
					{

						snake_verif_ia(snake_ia,snake,DROITE);
					}
					else
					{
						snake_set_direction(snake_ia,HAUT);
					}

				}
				else if( i == 1 )// DROITE
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x,ctete_ia.y+1),snake->tete))
					{

						snake_verif_ia(snake_ia,snake,GAUCHE);
					}
					else
					{
						snake_set_direction(snake_ia,DROITE);
					}
				}
				else if(i == 2) // BAS
				{
					if(coord_is_in_liste_snake(coord_from_xy(ctete_ia.x+1,ctete_ia.y),snake->tete))
					{

						snake_verif_ia(snake_ia,snake,DROITE);
					}
					else
					{
						snake_set_direction(snake_ia,BAS);
					}
				}
			}

			break;
	}*/
}

void snake_forward_ia1(Snake *snake_ia, Snake *snake, Coord bouf)
{
	Coord tete = snake_pos(snake_ia);


	printf("dir debut : %d\n", snake_direction(snake_ia));
	if(tete.x < bouf.x)
	{	
		if(snake_direction(snake_ia) == GAUCHE)
		{
			if(tete.y < bouf.y)
			{
				snake_set_direction(snake_ia, BAS);
				snake_verif_ia(snake_ia,snake,BAS);
			}
			else
			{
				snake_set_direction(snake_ia, HAUT);
				snake_verif_ia(snake_ia,snake,HAUT);
			}
		}
		else
		{
			snake_set_direction(snake_ia, DROITE);
			snake_verif_ia(snake_ia,snake,DROITE);
		}
	}
	if(tete.x > bouf.x)
	{
		if(snake_direction(snake_ia) == DROITE)
		{
			if(tete.y < bouf.y)
			{
				snake_set_direction(snake_ia, BAS);
				snake_verif_ia(snake_ia,snake,BAS);
			}
			else
			{
				snake_set_direction(snake_ia, HAUT);
				snake_verif_ia(snake_ia,snake,HAUT);
			}
		}
		else
		{
			snake_set_direction(snake_ia, GAUCHE);
			snake_verif_ia(snake_ia,snake,GAUCHE);
		}
	}
	
	if(tete.y < bouf.y)
	{
		if(snake_direction(snake_ia) == HAUT)
		{
			if(tete.x < bouf.x)
			{
				snake_set_direction(snake_ia, DROITE);
				snake_verif_ia(snake_ia,snake,DROITE);
			}
			else
			{
				snake_set_direction(snake_ia, GAUCHE);
				snake_verif_ia(snake_ia,snake,GAUCHE);
			}
		}
		else
		{
			snake_set_direction(snake_ia, BAS);
			snake_verif_ia(snake_ia,snake,BAS);
		}
	}

	if(tete.y > bouf.y)
	{
		if(snake_direction(snake_ia) == BAS)
		{
			if(tete.x < bouf.x)
			{
				snake_set_direction(snake_ia, DROITE);
				snake_verif_ia(snake_ia,snake,DROITE);
			}
			else
			{
				snake_set_direction(snake_ia, GAUCHE);
				snake_verif_ia(snake_ia,snake,GAUCHE);
			}
		}
		else
		{
			snake_set_direction(snake_ia, HAUT);
			snake_verif_ia(snake_ia,snake,HAUT);
		}
	}

	snake_forward(snake_ia);

	printf("dir  : %d\n\n", snake_direction(snake_ia));
}

void snake_increase(Snake *snake)
{
	Coord c = snake->dernier->coord;
	cons_liste_snake_fin(c, snake_dernier(snake));
	snake->dernier = snake_dernier(snake)->suivant;
}
