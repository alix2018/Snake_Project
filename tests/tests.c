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

	g_assert_cmpint(c1->x,==,1);
	g_assert_cmpint(c1->y,==,0);
	g_assert_cmpint(c2->x,==,2);
	g_assert_cmpint(c2->y,==,0);


	/* Test de la fonction snake_pos */
	g_assert_cmpint(snake_pos(nvSnake).x,==,c1->x);
	g_assert_cmpint(snake_pos(nvSnake).y,==,c1->y);


	/* Test de la fonction snake_longueur */
	g_assert_cmpint(snake_longueur(nvSnake),==,2);


	/* Test de la fonction snake_dernier */
	g_assert_cmpint(egalite_node(list_last_node(snake_liste_snake(nvSnake)),snake_dernier(nvSnake)),==,1);


	/* Test de la fonction snake_premier */
	g_assert_cmpint(egalite_node(list_first_node(snake_liste_snake(nvSnake)),snake_premier(nvSnake)),==,1);


	/* Test de la fonction snake_direction */
	g_assert_cmpint(snake_direction(nvSnake),==,180);

	/* Test de la fonction snake_set_direction */
	snake_set_direction(nvSnake,BAS);
	g_assert_cmpint(snake_direction(nvSnake),==,270);

	/* Test de la fonction snake_set_liste */
	List *liste = create_list();
	Coord c = coord_from_xy(1,2);
	Coord *coord2 = malloc(sizeof(Coord));
	*coord2 = c;
	list_add_last(liste,coord2);
	snake_set_liste(nvSnake,liste);

	c1 = node_elt(list_first_node(snake_liste_snake(nvSnake)));
	c2 = node_elt(list_last_node(snake_liste_snake(nvSnake)));

	g_assert_cmpint(c1->x,==,1);
	g_assert_cmpint(c1->y,==,2);

	free_snake(nvSnake);
}

void fct_list()
{
	/* Test de la focntion cons_node */
	Coord c = coord_from_xy(3,4);
	Coord *coord = malloc(sizeof(Coord));
	*coord = c;

	Node node = cons_node(coord,NULL);
	Coord *c1 = node_elt(node);

	g_assert_null(node_next(node));
	g_assert_null(node_prev(node));
	g_assert_cmpint(c1->x,==,3);
	g_assert_cmpint(c1->y,==,4);

}

int main(int argc, char **argv)
{
	g_test_init(&argc, &argv);

	/* ajouter fonctions par catégorie de tests unitaires */
	g_test_add_func("/struc/fct_snake", fct_snake);
	g_test_add_func("/list/fct_list", fct_list);

	return g_test_run();
}