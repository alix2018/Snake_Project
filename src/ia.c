//
// Created by thibaut on 14/03/16.
//

#include <stdio.h>
#include <time.h>
#include "ia.h"
#include "struc.h"


/**
 * @brief   verifie que la direction désigné par snake_forward_ia1 est pertinante et la corrige sinon
 *
 * @param[in]   snake_ia Schlanglà
 * @param[in]   snake  Le joueur
 * @param[in]   futurdir  La direction qui va être appliqué à snake_ia
 *
 */
static void snake_verif_ia(Snake *snake_ia,Snake *snake,Direction futurdir) // TODO déplacement défensif
{
    // TODO à réimplémenter avec le type list
    Coord ctete_ia = snake_pos(snake_ia);
    Node ls = snake_premier(snake);
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

/**
 * @brief   Fait avancé le Schlanglà : determine la direction logique à prendre pour acceder à la bouf, l'applique et appel snake_forward
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake  Le joueur
 * @param[in]   bouf  La bouf
 */
void snake_forward_ia1(struct snake *snake_ia, struct snake *snake, Coord bouf)
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
				snake_verif_ia(snake_ia, snake, BAS);
			}
			else
			{
				snake_set_direction(snake_ia, HAUT);
				snake_verif_ia(snake_ia, snake, HAUT);
			}
		}
		else
		{
			snake_set_direction(snake_ia, DROITE);
			snake_verif_ia(snake_ia, snake, DROITE);
		}
	}
	if(tete.x > bouf.x)
	{
		if(snake_direction(snake_ia) == DROITE)
		{
			if(tete.y < bouf.y)
			{
				snake_set_direction(snake_ia, BAS);
				snake_verif_ia(snake_ia, snake, BAS);
			}
			else
			{
				snake_set_direction(snake_ia, HAUT);
				snake_verif_ia(snake_ia, snake, HAUT);
			}
		}
		else
		{
			snake_set_direction(snake_ia, GAUCHE);
			snake_verif_ia(snake_ia, snake, GAUCHE);
		}
	}

	if(tete.y < bouf.y)
	{
		if(snake_direction(snake_ia) == HAUT)
		{
			if(tete.x < bouf.x)
			{
				snake_set_direction(snake_ia, DROITE);
				snake_verif_ia(snake_ia, snake, DROITE);
			}
			else
			{
				snake_set_direction(snake_ia, GAUCHE);
				snake_verif_ia(snake_ia, snake, GAUCHE);
			}
		}
		else
		{
			snake_set_direction(snake_ia, BAS);
			snake_verif_ia(snake_ia, snake, BAS);
		}
	}

	if(tete.y > bouf.y)
	{
		if(snake_direction(snake_ia) == BAS)
		{
			if(tete.x < bouf.x)
			{
				snake_set_direction(snake_ia, DROITE);
				snake_verif_ia(snake_ia, snake, DROITE);
			}
			else
			{
				snake_set_direction(snake_ia, GAUCHE);
				snake_verif_ia(snake_ia, snake, GAUCHE);
			}
		}
		else
		{
			snake_set_direction(snake_ia, HAUT);
			snake_verif_ia(snake_ia, snake, HAUT);
		}
	}

	snake_forward(snake_ia);

	printf("dir  : %d\n\n", snake_direction(snake_ia));
}