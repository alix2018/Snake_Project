#include <glib.h>
#include <stdio.h>
#include "../src/struc.h"
#include "../src/list.h"

void fct_snake()
{
	/* Test de la fonction coord_from_xy */
	Coord coord;
	int x,y;
	coord = coord_from_xy(x,y);
	g_assert_cmpint(coord.x,==,x);
	g_assert_cmpint(coord.y,==,y);


	/* Test de la fonction coord_egales */
	int a=x,b=y;
	Coord testcoord = coord_from_xy(a,b);
	g_assert_cmpint(coord_egales(testcoord,coord),==,1); 


	/* Test de la fonction create_snake & snake_liste_snake */
	Coord coordSnake = coord_from_xy(0,0);
	Snake *nvSnake = create_snake(2,coordSnake,GAUCHE);
	Coord *c1 = node_elt(list_first_node(snake_liste_snake(nvSnake)));
	Coord *c2 = node_elt(list_last_node(snake_liste_snake(nvSnake)));

	g_assert_cmpint(c1->x,==,0);
	g_assert_cmpint(c1->y,==,0);
	g_assert_cmpint(c2->x,==,1);
	g_assert_cmpint(c2->y,==,0);


	/* Test de la fonction snake_pos */
	g_assert_cmpint(snake_pos(nvSnake).x,==,c1->x);
	g_assert_cmpint(snake_pos(nvSnake).y,==,c1->y);


	/* Test de la fonction snake_longueur */
	g_assert_cmpint(snake_longueur(nvSnake),==,2);


	/* Test de la fonction snake_dernier */
	//g_assert_cmpint(egalite_node(list_last_node(snake_liste_snake(nvSnake)),snake_dernier(nvSnake)),==,1);


	/* Test de la fonction snake_premier */
	//g_assert_cmpint(egalite_node(list_last_node(snake_liste_snake(nvSnake)),snake_premier(nvSnake)),==,1);


	/* Test de la fonction snake_direction */


	/* Test de la fonction snake_set_direction */


	/* Test de la fonction snake_set_liste */


	/* Test de la fonction snake_forward */


	/* Test de la fonction snake_increase */

	free_snake(nvSnake);
}

int main(int argc, char **argv)
{
	g_test_init(&argc, &argv);

	g_test_add_func("/struc/fct_snake", fct_snake);

	return g_test_run();
}