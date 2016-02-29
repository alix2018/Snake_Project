#include <glib.h>
#include <stdio.h>
#include "../src/struc.h"

void tests_struc()
{
	/* Test de la fonction coord_from_xy */
	printf("Test de la fonction coord_from_xy : \n");
	Coord coord;
	int x,y;
	coord = coord_from_xy(x,y);

	if ( coord.x == x && coord.y == y )
	{
		printf("Fonction coord_from_xy OK\n");
	}

	else
	{
		printf("Problème fonction coord_from_xy\n");
	}


	/* Test de la fonction cons_liste_snake_debut */
	printf("Test de la fonction cons_liste_snake_debut : \n");
	ListeSnake test = NULL;
	ListeSnake res = cons_liste_snake_debut(coord,test);

	if ( liste_snake_coord(res).x == coord.x && liste_snake_coord(res).y == coord.y && liste_snake_precedent(res) == NULL && liste_snake_suivant(res) == test )
	{
		printf("Fonction cons_liste_snake_debut OK\n");
	}

	else
	{
		printf("Problème fonction cons_liste_snake_debut\n");
	}
	free_liste_snake(res);


	/* Test de la fonction cons_liste_snake_fin */
	printf("Test de la fonction cons_liste_snake_fin : \n");
	res = cons_liste_snake_fin(coord,test);

	if ( liste_snake_coord(res).x == coord.x && liste_snake_coord(res).y == coord.y && liste_snake_precedent(res) == NULL && liste_snake_suivant(res) == NULL )
	{
		printf("Fonction cons_liste_snake_fin OK\n");
	}

	else
	{
		printf("Problème fonction cons_liste_snake_fin\n");
	}

	/* Test de la fonction liste_snake_coord */
	printf("Test de la fonction liste_snake_coord : \n");

	if ( liste_snake_coord(res).x == x && liste_snake_coord(res).y == y)
	{
		printf("Fonction liste_snake_coord OK\n");
	}

	else
	{
		printf("Problème fonction liste_snake_coord\n");
	}


	/* Test de la fonction liste_snake_suivant */
	printf(" Test de la fonction liste_snake_suivant : \n");

	if ( liste_snake_suivant(res) == NULL ){
		printf("Fonction liste_snake_suivant OK\n");
	}

	else
	{
		printf("Problème fonction liste_snake_suivant\n");
	}


	/* Test de la fonction liste_snake_precedent */
	printf(" Test de la fonction liste_snake_precedent : \n");


	if ( liste_snake_precedent(res) == NULL )
	{
		printf("Fonction liste_snake_precedent OK\n");
	}

	else
	{
		printf("Problème fonction liste_snake_precendent\n");
	}
	free_liste_snake(res);


	/* Test de la fonction create_snake */
	printf(" Test de la fonction create_snake : \n");
	Coord coordSnake = coord_from_xy(0,0);
	Snake * nvSnake = create_snake(2,coordSnake,GAUCHE);

	if ( liste_snake_coord(snake_premier(nvSnake)).x == 0 && liste_snake_coord(snake_premier(nvSnake)).y == 0 && liste_snake_coord(snake_dernier(nvSnake)).x == 1 && liste_snake_coord(snake_dernier(nvSnake)).y == 0 )
	{
		printf("Fonction create_snake OK\n");
	}

	else
	{
		printf("Problème fonction create_snake\n");
	}


	/* Test de la fonction snake_pos */
	printf(" Test de la fonction snake_pos : \n");

	g_assert_cmpint(snake_pos(nvSnake).x,==,0);
	g_assert_cmpint(snake_pos(nvSnake).y,==,0);

	if (snake_pos(nvSnake).x == 0 && snake_pos(nvSnake).y == 0 )
	{
		printf("Fonction snake_pos OK\n");
	}

	else
	{
		printf("Problème fonction snake_pos\n");
	}

	/* Test de la fonction snake_longueur */
	printf(" Test de la fonction snake_longueur :");
	g_assert_cmpint(snake_longueur(nvSnake),==,2);

	/* Test de la fonction snake_dernier */

	/* Test de la fonction snake_premier */

	/* Test de la fonction snake_direction */
}

int main(int argc, char **argv)
{
	g_test_init(&argc, &argv);

	g_test_add_func("/struc/liste_snake", tests_struc);

	return g_test_run();
}