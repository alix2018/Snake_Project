#include <glib.h>
#include <stdio.h>
#include "../src/affichage.h"
#include "../src/struc.h"
#include "../src/list.h"
#include "../src/ia.h"
#include "../src/bonus.h"

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
	g_assert_cmpint(node_egalite(list_last_node(snake_liste_snake(nvSnake)),
                                 snake_dernier(nvSnake)), ==, 1);


	/* Test de la fonction snake_premier */
	g_assert_cmpint(node_egalite(list_first_node(snake_liste_snake(nvSnake)),
                                 snake_premier(nvSnake)), ==, 1);


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
	/* Test de la focntion cons_node & node_elt */
	Coord c = coord_from_xy(3,4);
	Coord *coord = malloc(sizeof(Coord));
    int *i;
	*coord = c;

	Node node = cons_node(coord,NULL);

	Coord *c1 = node_elt(node);

	g_assert_null(node_next(node));
	g_assert_null(node_prev(node));
	g_assert_cmpint(c1->x,==,3);
	g_assert_cmpint(c1->y,==,4);


	/* Test de la fonction create_list & list_is_empty */
	List *nvList = create_list();

	g_assert_cmpint(list_is_empty(nvList),==,1);


	/* Test de la fonction list_add_first & list_first_node */
	Coord c2 = coord_from_xy(2,5);
	Coord *coord1 = malloc(sizeof(Coord));
	*coord1 = c2;
	list_add_first(nvList,coord1);
	Node node1 = list_first_node(nvList);
	Coord *c3 = node_elt(node1);

	g_assert_cmpint(c3->x,==,2);
	g_assert_cmpint(c3->y,==,5);


	/* Test de la fonction list_add_last & list_last_node */
	list_add_last(nvList,coord);
	Node node2 = list_last_node(nvList);
	Coord *c4 = node_elt(node2);

	g_assert_cmpint(c4->x,==,3);
	g_assert_cmpint(c4->y,==,4);


	/* Test de la fonction node_next */
	Node nvNode = node_next(node1);
	Coord *c5 = node_elt(nvNode);

	g_assert_cmpint(c5->x,==,3);
	g_assert_cmpint(c5->y,==,4);


	/* Test de la fonction node_prev */
	Node nvNode2 = node_prev(node2);
	Coord *c6 = node_elt(nvNode2);

	g_assert_cmpint(c6->x,==,2);
	g_assert_cmpint(c6->y,==,5);


	/* Test de la fonction list_size */
	g_assert_cmpint(list_size(nvList),==,2);


	/* Test de la fonction list_pop_last */
	Coord *co1 = list_pop_last(nvList);
	g_assert_cmpint(list_size(nvList),==,1);

	list_add_first(nvList,coord);

	
	/* Test de la fonction list_pop_first */
	Coord *co2 = list_pop_first(nvList);
	g_assert_cmpint(list_size(nvList),==,1);



	/* Test de la fonction list_get_last */
	Coord *comp1 = node_elt(list_last_node(nvList));
	Coord *comp2 = list_get_last(nvList);

	g_assert_cmpint(comp1->x,==,comp2->x);
	g_assert_cmpint(comp1->y,==,comp2->y);


	/* Test de la fonction list_get_first */
	Coord *comp3 = node_elt(list_last_node(nvList));
	Coord *comp4 = list_get_last(nvList);

	g_assert_cmpint(comp3->x,==,comp4->x);
	g_assert_cmpint(comp3->y,==,comp4->y);


	/* Test de la fonction node_egalite */
    i = malloc(sizeof(int));
    *i = 1;
	Node node3 = cons_node(i,NULL);

	g_assert_cmpint(node_egalite(node3, node3),==,1);

    free(node_elt(node3));

	free_list(nvList);


}

int main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);

	/* ajouter fonctions par catégorie de tests unitaires */
	g_test_add_func("/struc/fct_snake", fct_snake);
	g_test_add_func("/list/fct_list", fct_list);

	return g_test_run();
}
