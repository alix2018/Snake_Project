//
// Created by thibaut on 14/03/16.
//

#include <stdio.h>
#include <time.h>
#include "ia.h"
#include "struc.h"

/**
 * @brief   verifie que la direction désignée par snake_forward_ia1 est pertinente et la corrige sinon
 *
 * @param[in]   snake_ia   Schlanglà
 * @param[in]   snake      Le joueur
 * @param[in]   futurdir   La direction qui va être appliquée à snake_ia
 *
 */
static void snake_verif_ia(Snake *snake_ia, Snake *snake, Direction futurdir, int nb_appel) // TODO déplacement défensif
{
    // TODO à réimplémenter avec le type list
    
    //On parcours le snake les coordonnés du snake du joueur
    Coord futur_tete = snake_pos(snake_ia), *cur_element_queue_ia, *cur_element_queue;
    Node ls_ia = list_first_node(snake_liste_snake(snake_ia));
    Node ls = list_first_node(snake_liste_snake(snake));

    switch(futurdir)
    {
        case HAUT:
            futur_tete.y -= 1;
            break;
        case BAS:
            futur_tete.y += 1;
            break;
        case GAUCHE:
            futur_tete.x -= 1;
            break;
        case DROITE:
            futur_tete.x += 1;
            break;
        default:
            printf("Erreur snake_verif_ia dans le switch de ia.c\n");
    }

    while(ls_ia!=NULL)
    {
        cur_element_queue_ia = (Coord *)node_elt(ls_ia);
        if(coord_egales(futur_tete, *cur_element_queue_ia))
        {
            if(nb_appel==0)
            {
                switch(futurdir)
                {
                    case HAUT:
                        snake_set_direction(snake_ia, DROITE);
                        snake_verif_ia(snake_ia, snake, DROITE, nb_appel+1);
                        break;
                    case BAS:
                        snake_set_direction(snake_ia, GAUCHE);
                        snake_verif_ia(snake_ia, snake, GAUCHE, nb_appel+1);
                        break;
                    case GAUCHE:
                        snake_set_direction(snake_ia, HAUT);
                        snake_verif_ia(snake_ia, snake, HAUT, nb_appel+1);
                        break;
                    case DROITE:
                        snake_set_direction(snake_ia, BAS);
                        snake_verif_ia(snake_ia, snake, BAS, nb_appel+1);
                        break;
                    default:
                        printf("Erreur snake_verif_ia dans le switch de ia.c\n");
                }
            }
            else
            {
                switch(futurdir)
                {
                    case DROITE:
                        snake_set_direction(snake_ia, GAUCHE);
                        break;
                    case GAUCHE:
                        snake_set_direction(snake_ia, DROITE);
                        break;
                    case HAUT:
                        snake_set_direction(snake_ia, BAS);
                        break;
                    case BAS:
                        snake_set_direction(snake_ia, HAUT);
                        break;
                    default:
                        printf("Erreur snake_verif_ia dans le switch de ia.c\n");
                }
            }
        }
        ls_ia = node_next(ls_ia);
    }

    while(ls!=NULL)
    {
        cur_element_queue = (Coord *)node_elt(ls);
        if(coord_egales(futur_tete, *cur_element_queue))
        {
            if(nb_appel==0)
            {
                switch(futurdir)
                {
                    case HAUT:
                        snake_set_direction(snake_ia, DROITE);
                        snake_verif_ia(snake_ia, snake, DROITE, nb_appel+1);
                        break;
                    case BAS:
                        snake_set_direction(snake_ia, GAUCHE);
                        snake_verif_ia(snake_ia, snake, GAUCHE, nb_appel+1);
                        break;
                    case GAUCHE:
                        snake_set_direction(snake_ia, HAUT);
                        snake_verif_ia(snake_ia, snake, HAUT, nb_appel+1);
                        break;
                    case DROITE:
                        snake_set_direction(snake_ia, BAS);
                        snake_verif_ia(snake_ia, snake, BAS, nb_appel+1);
                        break;
                    default:
                        printf("Erreur snake_verif_ia dans le switch de ia.c\n");
                }
            }
            else
            {
                switch(futurdir)
                {
                    case DROITE:
                        snake_set_direction(snake_ia, GAUCHE);
                        break;
                    case GAUCHE:
                        snake_set_direction(snake_ia, DROITE);
                        break;
                    case HAUT:
                        snake_set_direction(snake_ia, BAS);
                        break;
                    case BAS:
                        snake_set_direction(snake_ia, HAUT);
                        break;
                    default:
                        printf("Erreur snake_verif_ia dans le switch de ia.c\n");
                }
            }
        }
        ls = node_next(ls);
    }


    /*
    Coord ctete_ia = snake_pos(snake_ia);
    Node ls = snake_premier(snake);
    srand(time(NULL));
    switch (futurdir)
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
    }
    */
}

/**
 * @brief   Fait avancer le Schlanglà : déetermine la direction logique à prendre 
            pour accéder à la bouf, l'applique et appelle snake_forward
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bouf     La bouf
 */
void snake_forward_ia1(struct snake *snake_ia, struct snake *snake, Coord bouf)
{
    int indic = 1;
	Coord tete = snake_pos(snake_ia);

	printf("dir debut : %d\n", snake_direction(snake_ia));
	if(tete.x < bouf.x)
	{
		if(snake_direction(snake_ia) == GAUCHE)
		{
			if(tete.y < bouf.y)
			{
				snake_set_direction(snake_ia, BAS);
                indic = 0;
				snake_verif_ia(snake_ia, snake, BAS, 0);
			}
			else
			{
				snake_set_direction(snake_ia, HAUT);
                indic = 0;
				snake_verif_ia(snake_ia, snake, HAUT, 0);
			}
		}
		else
		{
			snake_set_direction(snake_ia, DROITE);
            indic = 0;
			snake_verif_ia(snake_ia, snake, DROITE, 0);
		}
	}
	if(tete.x > bouf.x)
	{
		if(snake_direction(snake_ia) == DROITE)
		{
			if(tete.y < bouf.y)
			{
				snake_set_direction(snake_ia, BAS);
                indic = 0;
				snake_verif_ia(snake_ia, snake, BAS, 0);
			}
			else
			{
				snake_set_direction(snake_ia, HAUT);
                indic = 0;
				snake_verif_ia(snake_ia, snake, HAUT, 0);
			}
		}
		else
		{
			snake_set_direction(snake_ia, GAUCHE);
            indic = 0;
			snake_verif_ia(snake_ia, snake, GAUCHE, 0);
		}
	}

	if(tete.y < bouf.y && indic)
	{
		if(snake_direction(snake_ia) == HAUT)
		{
			if(tete.x < bouf.x)
			{
				snake_set_direction(snake_ia, DROITE);
				snake_verif_ia(snake_ia, snake, DROITE, 0);
			}
			else
			{
				snake_set_direction(snake_ia, GAUCHE);
				snake_verif_ia(snake_ia, snake, GAUCHE, 0);
			}
		}
		else
		{
			snake_set_direction(snake_ia, BAS);
			snake_verif_ia(snake_ia, snake, BAS, 0);
		}
	}

	if(tete.y > bouf.y && indic)
	{
		if(snake_direction(snake_ia) == BAS)
		{
			if(tete.x < bouf.x)
			{
				snake_set_direction(snake_ia, DROITE);
				snake_verif_ia(snake_ia, snake, DROITE, 0);
			}
			else
			{
				snake_set_direction(snake_ia, GAUCHE);
				snake_verif_ia(snake_ia, snake, GAUCHE, 0);
			}
		}
		else
		{
			snake_set_direction(snake_ia, HAUT);
			snake_verif_ia(snake_ia, snake, HAUT, 0);
		}
	}

	snake_forward(snake_ia);

	printf("dir  : %d\n\n", snake_direction(snake_ia));
}