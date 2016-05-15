//
// Created by thibaut on 14/03/16.
//

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <glib.h>
#include "ia.h"

/**
 * @brief   Retourne la future coordonnée après avoir appliqué le déplacement à c
 *
 * @param[in]   c       La coord
 * @param[in]   dir     La direction à appliquer
 *
 */
Coord future_pos(Coord c, Direction dir)
{
    switch(dir){
        case HAUT:
            c.y -= 1;
            break;
        case BAS:
            c.y += 1;
            break;
        case GAUCHE:
            c.x -= 1;
            break;
        case DROITE:
            c.x += 1;
            break;
        default:
            printf("Erreur snake_verif_ia dans le switch de ia.c\n");
    }
    return c;
}


/**
 * @brief   verifie que la direction désignée par snake_forward_ia1 est pertinente et la corrige sinon
 *
 * @param[in]   snake_ia   Schlanglà
 * @param[in]   snake      Le joueur
 * @param[in]   futurdir   La direction qui va être appliquée à snake_ia
 *
 */
static void snake_verif_ia(Snake * snake_ia, Partie * p, Direction futurdir, int nb_appel) // TODO déplacement défensif
{
    // TODO à réimplémenter avec le type list
    TabSnakes *t = partie_tab(p);
    //On parcours le snake les coordonnés du snake du joueur
    Coord futur_tete = snake_pos(snake_ia), *cur_element_queue_ia, *cur_element_queue;
    Node ls_ia = list_first_node(snake_liste_snake(snake_ia));

    switch (futurdir) {
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

    while (ls_ia != NULL) {
        cur_element_queue_ia = (Coord *) node_elt(ls_ia);
        if (coord_egales(futur_tete, *cur_element_queue_ia)) {
            if (nb_appel <= 3) {
                switch (futurdir) {
                    case HAUT:
                        snake_set_direction(snake_ia, DROITE);
                        snake_verif_ia(snake_ia, p, DROITE, nb_appel + 1);
                        break;
                    case BAS:
                        snake_set_direction(snake_ia, GAUCHE);
                        snake_verif_ia(snake_ia, p, GAUCHE, nb_appel + 1);
                        break;
                    case GAUCHE:
                        snake_set_direction(snake_ia, HAUT);
                        snake_verif_ia(snake_ia, p, HAUT, nb_appel + 1);
                        break;
                    case DROITE:
                        snake_set_direction(snake_ia, BAS);
                        snake_verif_ia(snake_ia, p, BAS, nb_appel + 1);
                        break;
                    default:
                        printf("Erreur snake_verif_ia dans le switch de ia.c\n");
                }
            }
            else {
                switch (futurdir) {
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
    Snake * snake;
    int i;
    for (i = 0; i < t->nb_snakes; ++i)
    {
        snake = t->snakes[i];

        if (snake_ia != snake)
        {

            Node ls = list_first_node(snake_liste_snake(snake));

            while (ls != NULL)
            {
                cur_element_queue = (Coord *) node_elt(ls);
                if (coord_egales(futur_tete, *cur_element_queue))
                {
                    if (nb_appel <= 3)
                    {
                        switch (futurdir) {
                            case HAUT:
                                snake_set_direction(snake_ia, DROITE);
                                snake_verif_ia(snake_ia, p, DROITE, nb_appel + 1);
                                break;
                            case BAS:
                                snake_set_direction(snake_ia, GAUCHE);
                                snake_verif_ia(snake_ia, p, GAUCHE, nb_appel + 1);
                                break;
                            case GAUCHE:
                                snake_set_direction(snake_ia, HAUT);
                                snake_verif_ia(snake_ia, p, HAUT, nb_appel + 1);
                                break;
                            case DROITE:
                                snake_set_direction(snake_ia, BAS);
                                snake_verif_ia(snake_ia, p, BAS, nb_appel + 1);
                                break;
                            default:
                                printf("Erreur snake_verif_ia dans le switch de ia.c\n");
                        }
                    } else {
                        switch (futurdir) {
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
}

void snake_verif_wall(Snake *snake_ia, Partie * p, Direction futurdir,Map *m)
{
    Coord futur_tete = snake_pos(snake_ia), *cur_element_queue_ia, *cur_element_queue;


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
    printf("(%i | %i)\n",futur_tete.x,futur_tete.y);
    if(futur_tete.y == -1 )
    {
        switch(futurdir)
        {
            case HAUT:
                snake_set_direction(snake_ia,BAS);
                snake_verif_ia(snake_ia,p,BAS,0);
                break;
            case GAUCHE:
                snake_set_direction(snake_ia,DROITE);
                snake_verif_ia(snake_ia,p,DROITE,0);
                break;
            case DROITE:
                snake_set_direction(snake_ia,GAUCHE);
                snake_verif_ia(snake_ia,p,GAUCHE,0);
                break;
            default:
                printf("Erreur snake_verif_ia dans le switch de ia.c\n");
        }
    }
    else if(futur_tete.y == map_height(m))
    {
        switch(futurdir)
        {
            case BAS:
                snake_set_direction(snake_ia,HAUT);
                snake_verif_ia(snake_ia,p,HAUT,0);
                break;
            case GAUCHE:
                snake_set_direction(snake_ia,DROITE);
                snake_verif_ia(snake_ia,p,DROITE,0);
                break;
            case DROITE:
                snake_set_direction(snake_ia,GAUCHE);
                snake_verif_ia(snake_ia,p,GAUCHE,0);
                break;
            default:
                printf("Erreur snake_verif_ia dans le switch de ia.c\n");
        }
    }
    else if(futur_tete.x == map_width(m))
    {
        switch(futurdir)
        {
            case DROITE:
                snake_set_direction(snake_ia,GAUCHE);
                snake_verif_ia(snake_ia,p,GAUCHE,0);
                break;
            case BAS:
                snake_set_direction(snake_ia,HAUT);
                snake_verif_ia(snake_ia,p,HAUT,0);
                break;
            case HAUT:
                snake_set_direction(snake_ia,BAS);
                snake_verif_ia(snake_ia,p,BAS,0);
                break;
            default:
                printf("Erreur snake_verif_ia dans le switch de ia.c\n");
        }
    }
    else if(futur_tete.x == -1)
    {
        switch(futurdir)
        {
            case GAUCHE:
                snake_set_direction(snake_ia,DROITE);
                snake_verif_ia(snake_ia,p,DROITE,0);
                break;
            case BAS:
                snake_set_direction(snake_ia,HAUT);
                snake_verif_ia(snake_ia,p,HAUT,0);
                break;
            case HAUT:
                snake_set_direction(snake_ia,BAS);
                snake_verif_ia(snake_ia,p,BAS,0);
                break;
            default:
                printf("Erreur snake_verif_ia dans le switch de ia.c\n");
        }
    }
}

/**
 * @brief   Fait avancer le Schlanglà : déetermine la direction logique à prendre 
            pour accéder à la bonus, l'applique et appelle snake_forward
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia1(Snake *snake_ia, Partie *p)
{
    //Snake *snake_ia = partie_schlanga(p);
    //Snake *snake = partie_player(p);

    Coord bonus =bonus_coord(bonus_near_from_snake(partie_tab_bonus(p),snake_ia));
    int indic = 1;
	Coord tete = snake_pos(snake_ia);

	if(tete.x < bonus.x)
	{
		if(snake_direction(snake_ia) == GAUCHE)
		{
			if(tete.y < bonus.y)
			{
				snake_set_direction(snake_ia, BAS);
                indic = 0;
				snake_verif_ia(snake_ia, p, BAS, 0);
			}
			else
			{
				snake_set_direction(snake_ia, HAUT);
                indic = 0;
				snake_verif_ia(snake_ia, p, HAUT, 0);
			}
		}
		else
		{
			snake_set_direction(snake_ia, DROITE);
            indic = 0;
			snake_verif_ia(snake_ia, p, DROITE, 0);
		}
	}
	if(tete.x > bonus.x)
	{
		if(snake_direction(snake_ia) == DROITE)
		{
			if(tete.y < bonus.y)
			{
				snake_set_direction(snake_ia, BAS);
                indic = 0;
				snake_verif_ia(snake_ia, p, BAS, 0);
			}
			else
			{
				snake_set_direction(snake_ia, HAUT);
                indic = 0;
				snake_verif_ia(snake_ia, p, HAUT, 0);
			}
		}
		else
		{
			snake_set_direction(snake_ia, GAUCHE);
            indic = 0;
			snake_verif_ia(snake_ia, p, GAUCHE, 0);
		}
	}

	if(tete.y < bonus.y && indic)
	{
		if(snake_direction(snake_ia) == HAUT)
		{
			if(tete.x < bonus.x)
			{
				snake_set_direction(snake_ia, DROITE);
				snake_verif_ia(snake_ia, p, DROITE, 0);
			}
			else
			{
				snake_set_direction(snake_ia, GAUCHE);
				snake_verif_ia(snake_ia, p, GAUCHE, 0);
			}
		}
		else
		{
			snake_set_direction(snake_ia, BAS);
			snake_verif_ia(snake_ia, p, BAS, 0);
		}
	}

	if(tete.y > bonus.y && indic)
	{
		if(snake_direction(snake_ia) == BAS)
		{
			if(tete.x < bonus.x)
			{
				snake_set_direction(snake_ia, DROITE);
				snake_verif_ia(snake_ia, p, DROITE, 0);
			}
			else
			{
				snake_set_direction(snake_ia, GAUCHE);
				snake_verif_ia(snake_ia, p, GAUCHE, 0);
			}
		}
		else
		{
			snake_set_direction(snake_ia, HAUT);
			snake_verif_ia(snake_ia, p, HAUT, 0);
		}
	}



}

/**
 * @brief   Fait avancer le Schlanglà : ia qui tourne sur elle même dans le sens directe
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia3(Snake *snake_ia, Partie *p)
{
    Direction dir = snake_direction(snake_ia);
    Coord tete = snake_pos(snake_ia);
    Direction futur_dir;
    switch(dir)
    {
        case HAUT:
            futur_dir = GAUCHE;
            break;
        case GAUCHE:
            futur_dir = BAS;
            break;
        case BAS:
            futur_dir = DROITE;
            break;
        case DROITE:
            futur_dir = HAUT;
            break;

    }

    Coord futur_tete = future_pos(tete, futur_dir);

    int no_collision = 1;
    Node s = NULL;
    for(s = snake_premier(snake_ia); s != NULL; s = node_next(s))
    {
        if(coord_egales(*(Coord *)node_elt(s), futur_tete))
        {
            no_collision = 0;
        }
    }
    if(no_collision)
    {
        snake_set_direction(snake_ia, futur_dir);
    }
}

/**
 * @brief   Fait avancer le Schlanglà : ia qui tourne sur elle même dans le sens horraire
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia4(Snake *snake_ia, Partie *p)
{
    Direction dir = snake_direction(snake_ia);
    Coord tete = snake_pos(snake_ia);
    Direction futur_dir;
    switch(dir)
    {
        case HAUT:
            futur_dir = DROITE;
            break;
        case DROITE:
            futur_dir = BAS;
            break;
        case BAS:
            futur_dir = GAUCHE;
            break;
        case GAUCHE:
            futur_dir = HAUT;
            break;

    }

    Coord futur_tete = future_pos(tete, futur_dir);

    int no_collision = 1;
    Node s = NULL;
    for(s = snake_premier(snake_ia); s != NULL; s = node_next(s))
    {
        if(coord_egales(*(Coord *)node_elt(s), futur_tete))
        {
            no_collision = 0;
        }
    }
    if(no_collision)
    {
        snake_set_direction(snake_ia, futur_dir);
    }
}

void snake_ia_border(Snake *snake_ia, Partie *p)
{
    Map *m = partie_map(p);
    Direction futurdir = snake_direction(snake_ia);
    Coord futur_tete = future_pos(snake_pos(snake_ia), futurdir);
    if(futur_tete.y == 0 || futur_tete.y == map_height(m)-1)
    {
        if(futur_tete.x < map_width(m)/2)
        {
            snake_set_direction(snake_ia, DROITE);
        }
        else
        {
            snake_set_direction(snake_ia, GAUCHE);
        }
    }
    if(futur_tete.x == 0 || futur_tete.x == map_width(m)-1)
    {
        if(futur_tete.y < map_height(m)/2)
        {
            snake_set_direction(snake_ia, HAUT);
        }
        else
        {
            snake_set_direction(snake_ia, BAS);
        }
    }
}

/**
 * @brief   Fait avancer le Schlanglà : ia qui avance en accordéon
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia5(Snake *snake_ia, Partie *p)
{
    int duree = partie_duree(p);
    if(duree%4<2)
    {
        snake_forward_ia3(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else
    {
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
}

/**
 * @brief   Fait avancer le Schlanglà : ia qui fait des losanges de rayon R dans le sens horraire
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia6(Snake *snake_ia, Partie *p, int R)
{
    int duree = partie_duree(p);
    if(duree%(2*R)<=R)
    {
        if(duree%2)
        {
            snake_forward_ia3(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
        else
        {
            snake_forward_ia4(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
        if(duree%(2*R)==R)
        {
            snake_forward_ia3(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
    }
    else
    {
        if(duree%2==0)
        {
            snake_forward_ia3(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
        else
        {
            snake_forward_ia4(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
    }
}

/**
 * @brief   Fait avancer le Schlanglà : ia qui fait des losanges de rayon R dans le sens direct
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia6bis(Snake *snake_ia, Partie *p, int R)
{
    int duree = partie_duree(p);
    if(duree%(2*R)<=R)
    {
        if(duree%2==0)
        {
            snake_forward_ia3(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
        else
        {
            snake_forward_ia4(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
        if(duree%(2*R)==R)
        {
            snake_forward_ia3(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
    }
    else
    {
        if(duree%2)
        {
            snake_forward_ia3(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
        else
        {
            snake_forward_ia4(snake_ia, p);
            snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
            snake_ia_border(snake_ia, p);
            snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
        }
    }
}

/**
 * @brief   Fait avancer le Schlanglà : ia qui fait des rectangles lxL
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia7(Snake *snake_ia, Partie *p, int l, int L)
{
    int duree = partie_duree(p);

    if(duree%(l+L) <= l)
    {      
        if(duree%(l+L) == l)
        {
            snake_forward_ia3(snake_ia, p);
        }
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else
    {
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
}

/**
 * @brief   Fait avancer le Schlanglà : ia qui dessine une croix de taille l
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia8(Snake *snake_ia, Partie *p, int l)
{
    int duree = partie_duree(p);
    int m = 2*l+1;
    if(duree%m < l)
    {      
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);

    }
    else if( duree%m == l || duree%m == l+1)
    {
        snake_forward_ia3(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m > l+1 && duree%m < 2*l)
    {
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else
    {
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
}

/**
 * @brief   Fait avancer le Schlanglà : ia qui dessine un petit penis ELLE EST CASSE...
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia9(Snake *snake_ia, Partie *p)
{
    int duree = partie_duree(p);
    int l = snake_longueur(snake_ia);
    unsigned dl = (unsigned)(l/2)+1;
    int m = 4*l+7+4*dl;
    if(duree%m < l)//On monte tout droit
    {
        printf("1 ) On monte tout droit\n");
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m ==  l || duree%m ==  l+1)//On tourne à droite deux fois
    {
        printf("2-3) On tourne à droite\n");
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    //On descend de l et on doit avancer en plus de dl pour ne pas se mordre la queue lors du demi tour
    else if( duree%m > l+1 && duree%m < 2*l+1+dl)//On va tout droit
    {
        printf("4) On descend\n");
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m ==  2*l+1+dl || duree%m ==  2*l+2+dl)//On tourne à gauche 2 fois
    {
        printf("5-6) On tourne à gauche\n");
        snake_forward_ia3(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m > 2*l+2+dl && duree%m < 2*l+2+2*dl)//On remonte de dl
    {
        printf("7) On monte tout droit\n");
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m == 2*l+2+2*dl)//On tourne à gauche
    {
        printf("8) On tourne à gauche\n");
        snake_forward_ia3(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m == 2*l+3+2*dl)//On tourne à droite
    {
        printf("9) On tourne à droite\n");
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m > 2*l+3+2*dl && duree%m < 3*l+3+2*dl)//On remonte de l
    {
        printf("10) On monte tout droit\n");
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m == 3*l+3+2*dl || duree%m == 3*l+4+2*dl)//On tourne à gauche deux fois
    {
        printf("11-12) On tourne à gauche\n");
        snake_forward_ia3(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m > 3*l+4+2*dl && duree%m < 4*l+4+3*dl)//On descend de l
    {
        printf("13) On descend tout droit\n");
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m == 4*l+4+3*dl || duree%m == 4*l+5+3*dl)//On tourne à droite deux fois
    {
        printf("14-15) On tourne à droite\n");
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m > 4*l+5+3*dl && duree%m < 4*l+5+4*dl)//On remonte de dl
    {
        printf("16) On monte tout droit\n");
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m == 4*l+5+4*dl)//On tourne à droite
    {
        printf("17) On tourne à droite\n");
        snake_forward_ia4(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
    else if( duree%m == 4*l+6+4*dl)//On tourne à gauche
    {
        printf("18) On tourne à gauche\n");
        snake_forward_ia3(snake_ia, p);
        snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
        snake_ia_border(snake_ia, p);
        snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
    }
}

/**
 * @brief   Fait avancer le Schlanglà : fait faire des formes de manière aléatoire au schlangà
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   p        La partie
 */
void snake_forward_ia10(Snake * snake_ia,Partie *p)
{
    GRand * r = g_rand_new();
    gint32  rint = g_rand_int_range(r,1,9);

    int d = partie_duree(p), indic = snake_indic_duree(snake_ia), num_ia = snake_num_ia(snake_ia);


    if(indic==0)
    {
        num_ia = rint;
        snake_set_num_ia(snake_ia, num_ia);
        switch(rint)
        {
            case 1:
                snake_set_indic_duree(snake_ia, 100);
                break;
            case 2:
                snake_set_indic_duree(snake_ia, 10);
                break;
            case 3: 
            case 4:
                snake_set_indic_duree(snake_ia,10);
                break;
            case 5:
                snake_set_indic_duree(snake_ia,10);
                break;
            case 6:
                snake_set_indic_duree(snake_ia,10);
                break;
            case 7:
                snake_set_indic_duree(snake_ia,10);
                break;
            case 8:
                snake_set_indic_duree(snake_ia,10);
                break;
            case 9:
                snake_set_indic_duree(snake_ia,10);
                break;
            default:
                snake_set_indic_duree(snake_ia, 10);

        }
    }

    switch(num_ia)
    {
            case 1:
                snake_forward_ia1(snake_ia, p);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            case 2:
                snake_forward_ia2(snake_ia, p);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            case 3:
                snake_forward_ia3(snake_ia, p);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            case 4:
                snake_forward_ia4(snake_ia, p);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            case 5:
                snake_forward_ia5(snake_ia, p);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            case 6:
                snake_forward_ia6(snake_ia, p, 6);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            case 7:
                snake_forward_ia7(snake_ia, p, 5, 5);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            case 8:
                snake_forward_ia8(snake_ia, p, 8);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            case 9:
                snake_forward_ia1(snake_ia, p);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
            default:
                snake_forward_ia1(snake_ia, p);
                snake_set_indic_duree(snake_ia, snake_indic_duree(snake_ia)-1);
                break;
    }
    snake_verif_wall(snake_ia, p, snake_direction(snake_ia), partie_map(p));
    snake_ia_border(snake_ia, p);
    snake_verif_ia(snake_ia, p, snake_direction(snake_ia), 0);
}

/**
 * @brief   Fait avancer le Schlanglà : déetermine la direction aleatoire
            pour accéder à la bonus, l'applique et appelle snake_forward
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 */
void snake_forward_ia2(Snake * snake_ia,Partie *p)
{
    //Snake *snake_ia = partie_schlanga(p);
    //Snake *snake = partie_snake(p);
    Coord bonus =bonus_coord(bonus_near_from_snake(partie_tab_bonus(p),snake_ia));
    GRand * r = g_rand_new();
    gint32  rint = g_rand_int_range(r,0,4);
    Direction rdir;
    if(rint == 0)
    {
        rdir = BAS;
    }
    else if(rint == 1)
    {
        rdir = HAUT;
    }
    else if( rint == 2)
    {
        rdir = GAUCHE;
    }
    else
    {
        rdir = DROITE;
    }
    snake_set_direction(snake_ia,rdir);

    snake_verif_wall(snake_ia, p, rdir, partie_map(p));

    snake_verif_ia(snake_ia,p, rdir, 0);
    g_rand_free(r);
}

/**
 * @brief   Change la direction de l'ia en fonction de l'IA demandé (ia_name)
 *
 * @param[in]   snake_ia Le Schlanglà
 * @param[in]   snake    Le joueur
 * @param[in]   bonus     La bonus
 * @param[in]   ia_name  Le nom de l'ia
 */
void snake_set_direction_ia(Snake * snake_ia, Partie *p, char * ia_name)
{
    if(strcmp(ia_name,"ia1") == 0)
    {
        snake_forward_ia1(snake_ia, p);
    }
    else if(strcmp(ia_name,"ia2") == 0)
    {
        // TODO ia2
        snake_forward_ia2(snake_ia, p);
    }
    else if(strcmp(ia_name,"ia3") == 0)
    {
        // TODO ia2
        snake_forward_ia3(snake_ia, p);
    }
    else if(strcmp(ia_name,"ia4") == 0)
    {
        // TODO ia2
        snake_forward_ia4(snake_ia, p);
    }
    else if(strcmp(ia_name,"ia5") == 0)
    {
        // TODO ia2
        snake_forward_ia5(snake_ia, p);
    }
    else if(strcmp(ia_name,"ia6") == 0)
    {
        // TODO ia2
        snake_forward_ia6(snake_ia, p, 6);
    }
    else if(strcmp(ia_name,"ia7") == 0)
    {
        // TODO ia2
        snake_forward_ia7(snake_ia, p, 4, 8);
    }
    else if(strcmp(ia_name,"ia8") == 0)
    {
        // TODO ia2
        snake_forward_ia8(snake_ia, p, 4);
    }
    else if(strcmp(ia_name,"ia9") == 0)
    {
        // TODO ia2
        snake_forward_ia9(snake_ia, p);
    }
    else
    {
        // L'IA de base est l'ia1
        snake_forward_ia10(snake_ia, p);
    }
}