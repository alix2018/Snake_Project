#include <glib.h>
#include <stdio.h>
#include "../src/affichage.h"
#include "../src/struc.h"
#include "../src/list.h"
#include "../src/ia.h"
#include "../src/bonus.h"


void fct_affichage()
{
	/* Test de la fonction create_snake_actor */

	/** Les constant non définie **/
	SnakeActor * sa;
	ClutterActor * zone_snake;
	Snake *s;
	ClutterColor *color;
	SnakeImage *imgs;

	/** On défini les constant avec beaucoup d'autres fonction */
	Coord coordSnake = coord_from_xy(0,0);
	s = create_snake(2,coordSnake,GAUCHE);

	ClutterScript* ui = clutter_script_new();
	clutter_script_load_from_file(ui, "src/gui/stage.json", NULL);
	clutter_script_connect_signals(ui, ui);
	zone_snake = CLUTTER_ACTOR(clutter_script_get_object(ui, "zone_snake"));

	//clutter_stage_set_key_focus(CLUTTER_STAGE(stage), zone_snake);

	sa = create_snake_actor(zone_snake,s,color,imgs);


	// TODO LES TEST !

	//snake_border_map(sa);



}

void fct_ia()
{
	// Test de la fonction snake_forward_ia1
	Coord coordSnake = coord_from_xy(2,2);
	Snake* s = create_snake(2,coordSnake,BAS);
	Coord coordSnake_ia = coord_from_xy(0,2);
	Snake* s_ia = create_snake(2,coordSnake_ia,GAUCHE);
	Coord bouf = coord_from_xy(0,0);
	// Schema simple
	//  b . . .
	//	. . . .				// Schema p = player
	//	i . p .				// i = ia et b = bonus
	//  i . p .
	snake_forward_ia1(s_ia,s,bouf);

	// TODO les fonctions
}


void fct_bonus()
{

	/* Test de la fonction bouf_new et bouf_coord */
	Coord coord;
	int x,y;
	Bouf * bouf = bouf_new(x,y);
	coord = bouf_coord(bouf);
	// On test si le bonus est dans le cadre
	g_assert_cmpint(coord.x,<=,x);
	g_assert_cmpint(coord.x,>=,0);
	g_assert_cmpint(coord.y,<=,y);
	g_assert_cmpint(coord.y,>=,0);

	// Test de la fonction bouf_update

	int x1,y1;
	bouf_update(bouf,x1,y1);
	Coord coord1 = bouf_coord(bouf);
	// On test si le bonus est dans le cadre
	g_assert_cmpint(coord1.x,<=,x1);
	g_assert_cmpint(coord1.x,>=,0);
	g_assert_cmpint(coord1.y,<=,y1);
	g_assert_cmpint(coord1.y,>=,0);


 	//TODO actor test
}
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


	/* Test de la fonction list_foreach */

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


	/* Test de la fonction list_pop_first ?*/
	//Coord *co2 = list_pop_first(nvList);
	//g_assert_cmpint(list_size(nvList),==,0);



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


	/* Test de la fonction egalite_node */
    i = malloc(sizeof(int));
    *i = 1;
	Node node3 = cons_node(i,NULL);
    i = malloc(sizeof(int));
    *i = 1;
	Node node4 = cons_node(i,NULL);

	g_assert_true(egalite_node(node3,node4));

    free(node_elt(node3));
    free(node_elt(node4));

	free_list(nvList);


}

int main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);

	/* ajouter fonctions par catégorie de tests unitaires */
	g_test_add_func("/struc/fct_snake", fct_snake);
	g_test_add_func("/list/fct_list", fct_list);
	g_test_add_func("/bonus/fct_bonus", fct_bonus);

	return g_test_run();
}
