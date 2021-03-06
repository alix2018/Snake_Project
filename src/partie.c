//
// Created by thibaut on 16/03/16.
//

#include "partie.h"
#include "struc.h"
#include "bonus.h"
#include "affichage.h"
#include "ia.h"
#include "collisions.h"
#include "score.h"
#include <string.h>
#include <glib.h>

struct _Map
{
    int width;
    int height;
};

struct _Partie
{
    Snake *snake;
    Snake *schlanga;
    Bouf *nourriture;

    GestionCollisions *collisions;

    Affichage *affichage;

    gboolean en_cours;

    Map *map;
};

Snake * partie_snake(Partie *p)
{
    return p->snake;
}
void partie_set_snake(Partie *p,Snake *s)
{
    p->snake = s;
}
Snake * partie_schlanga(Partie *p)
{
    return p->schlanga;
}
void partie_set_schlanga(Partie *p,Snake *s)
{
    p->schlanga = s;
}

Bouf * partie_bouf(Partie *p)
{
    return p->nourriture;
}
void partie_set_bouf(Partie *p,Bouf *s)
{
    p->nourriture = s;
}

/**
 * @brief   Crée le plateau d'une partie.
 *
 * @param[in]    width  La largeur du plateau en cases.
 * @param[in]    height La hauteur du plateau en cases.
 *
 * @return  Le plateau alloué.
 */
Map *create_map(int width, int height)
{
    Map *res;

    res = malloc(sizeof(Map));
    res->width = width;
    res->height = height;

    return res;
}

/**
 * @brief   Libère la mémoire consommée par un plateau.
 *
 * @param[in]    map    Le plateau à libérer.
 */
void free_map(Map *map)
{
    free(map);
}

/**
 * @brief   Permet d'obtenir la largeur d'un plateau.
 *
 * @param[in]    map    Un plateau.
 *
 * @return  La largeur du plateau.
 */
int map_width(Map *map)
{
    return map->width;
}

/**
 * @brief   Permet d'obtenir la hauteur d'un plateau.
 *
 * @param[in]    map    Un plateau.
 *
 * @return  La hauteur du plateau.
 */
int map_height(Map *map)
{
    return map->height;
}


/**
 * @brief   Crée une partie. Il faut appeler init_partie pour initaliser la
 *          partie.
 *
 * @return  La partie allouée.
 */
Partie *create_partie()
{
    Partie *res;

    res = malloc(sizeof(Partie));
    res->snake = NULL;
    res->schlanga = NULL;
    res->nourriture = NULL;
    res->map = NULL;
    res->affichage = NULL;
    res->collisions = NULL;

    return res;
}


/**
 * @brief   Libère la mémoire consommée par une partie.
 *
 * @param[in]    partie La partie à supprimer.
 */
void free_partie(Partie *partie)
{
    free(partie->nourriture);
    free_affichage(partie->affichage);
    free_map(partie->map);
    free_gestion_collisions(partie->collisions);
    free_snake(partie->snake);
    free_snake(partie->schlanga);
    free(partie);
}


/**
 * @brief   Permet d'obtenir le plateau d'une partie.
 *
 * @param[in]   partie  Une partie.
 *
 * @return  Le plateau de la partie.
 */
Map *partie_map(Partie *partie)
{
    return partie->map;
}

/**
 * @brief   Retourne le GStrind à afficher lorsqu'on demande les scores
 *
 * @return  etourne le GStrind à afficher lorsqu'on demande les scores
 */
GString *get_gstring_score()
{
    GString *res = g_string_new("\n\n === Top 10 === \n");
    List *tab_scores = get_table_scores();
    List *tab_trie = tri_max(tab_scores);
    Node n;
    void *s;
    char c_sc[100];
    char c_ligne[10];
    char c[2];
    int i=0;
    for(n = list_first_node(tab_trie); n != NULL && i<10; n = node_next(n))
    {
        sprintf(c_ligne, "%d) ", i+1);
        g_string_append(res, c_ligne);

        s = (Score *)node_elt(n);
        sprintf(c_sc, "%d", get_score(s));
        g_string_append(res, c_sc);
        g_string_append(res, " ");

        g_string_append(res, get_pseudo(s));
        g_string_append(res, " ");

        c[0] = get_gagnant(s);
        c[1] = '\0';
        g_string_append(res, c);
        g_string_append(res, "\n");
        i ++;
    }
    free_table_scores(tab_trie);
    return res;
}

/**
 * @brief   Fonction callback appelée quand un Snake entre en collision avec
 *          un Snake.
 *
 * @param[in]    snake  Le coupable.
 * @param[in]    obj2   La victime.
 * @param[in]    data   Un pointeur vers la partie en cours.
 */
static void collision_snake_vers_snake(Snake *snake, void *obj2, void *data)
{
    Partie *partie = data;
    ClutterScript *ui = affichage_ui(partie->affichage);
    ClutterActor *fin_partie;

    partie->en_cours = FALSE;

    GString *out = g_string_new(snake_pseudo(partie->snake));

    if(snake == partie->schlanga)
    {
        g_string_append(out, "\n Gagné !");
        score_enregistre(partie->snake, 'G');
    }
    else
    {
        g_string_append(out, "\n Perdu !");
        score_enregistre(partie->snake, 'P');
    }

    GString *gscore = get_gstring_score();
    g_string_append(out, gscore->str);

    clutter_text_set_text(
        CLUTTER_TEXT(clutter_script_get_object(ui, "fin_partie_texte")),
        out->str
    );

    fin_partie = CLUTTER_ACTOR(clutter_script_get_object(ui, "fin_partie"));
    clutter_actor_add_child(
        CLUTTER_ACTOR(clutter_script_get_object(ui, "stage")),
        fin_partie
    );

    clutter_actor_save_easing_state(fin_partie);
    clutter_actor_set_easing_duration(fin_partie, 250);
    clutter_actor_set_opacity(fin_partie, 255);
    clutter_actor_restore_easing_state(fin_partie);
}

/**
 * @brief   Fonction callback appelée quand un Snake entre en collision avec
 *          la nourriture.
 *
 * @param[in]    snake  Le coupable.
 * @param[in]    obj2   La victime.
 * @param[in]    data   Un pointeur vers la partie en cours.
 */
static void collision_snake_vers_nourriture(Snake *snake, void *obj2, void *data)
{
    Bouf *nourriture = obj2;
    Partie *partie = data;

    snake_increase(snake);
    bouf_update(nourriture, partie->map->width, partie->map->height);
}


/**
 * @brief   Initialise une partie déjà allouée.
 *
 * @param[in]    partie Une partie.
 * @param[in]    ui     Le ClutterScript de la fenêtre.
 * @param[in]    width  La largeur du plateau.
 * @param[in]    height La hauteur du plateau.
 */
void init_partie(Partie *partie, ClutterScript *ui, int width, int height)
{
    Snake *snake;
    Snake *schlanga;
    Bouf *bouf;

    CollisionObject *co_snake;
    CollisionObject *co_schlanga;
    CollisionObject *co_nourriture;
    CollisionObject *co_map;

    partie->map = create_map(width, height);
    partie->en_cours = TRUE;

    snake = create_snake(
        10,
        coord_from_xy(22, 2),
        DROITE
    );// struc.c
    partie->snake = snake;
    schlanga = create_snake(
        10,
        coord_from_xy(22, 7),//L'ia commence 5 case en dessous du snake de base
        DROITE
    );// struc.c
    partie->schlanga = schlanga;
    bouf = bouf_new(width, height);// bonus.c
    partie->nourriture = bouf;

    partie->collisions = create_gestion_collisions();// collisions.c
    co_snake = gestion_collision_add_object(partie->collisions, partie->snake,
                                            COLLISION_SNAKE);// collisions.c
    co_schlanga = gestion_collision_add_object(partie->collisions,
                                               partie->schlanga,
                                               COLLISION_SNAKE);
    co_nourriture = gestion_collision_add_object(partie->collisions,
                                                 partie->nourriture,
                                                 COLLISION_BONUS);
    co_map = gestion_collision_add_object(partie->collisions, partie->map,
                                          COLLISION_MAP);

    collision_object_add_collision(
        co_snake,
        create_collision(partie->snake, collision_snake_vers_snake, partie)// collisions.c
    );// collisions.c
    collision_object_add_collision(
        co_snake,
        create_collision(partie->schlanga, collision_snake_vers_snake, partie)
    );

    collision_object_add_collision(
        co_schlanga,
        create_collision(partie->snake, collision_snake_vers_snake, partie)
    );
    collision_object_add_collision(
        co_schlanga,
        create_collision(partie->schlanga, collision_snake_vers_snake, partie)
    );

    collision_object_add_collision(
        co_nourriture,
        create_collision(partie->snake, collision_snake_vers_nourriture, partie)
    );
    collision_object_add_collision(
        co_nourriture,
        create_collision(partie->schlanga, collision_snake_vers_nourriture, partie)
    );

    collision_object_add_collision(
        co_map,
        create_collision(partie->snake, collision_snake_vers_snake, partie)
    );
    collision_object_add_collision(
        co_map,
        create_collision(partie->schlanga, collision_snake_vers_snake, partie)
    );

    partie->affichage = create_affichage();// affichage.c
    init_affichage(partie->affichage, ui, snake, width, height);// affichage.c

    affichage_add_snake(partie->affichage, snake,  CLUTTER_COLOR_Blue);// affichage.c
    affichage_add_snake(partie->affichage, schlanga,  CLUTTER_COLOR_Red);
    affichage_add_bonus(partie->affichage, bouf,  CLUTTER_COLOR_Green);

    g_timeout_add(150, timeout_tick_cb, partie);
}

void init_pseudo(Partie *p, int argc, char **argv)
{
    if(argc == 2)
    {
        snake_set_pseudo(p->snake, argv[1]); 
    }
    else
    {
        snake_set_pseudo(p->snake, "Anonyme");
    }
}

/**
 * @brief Fonction callback appelée à intervalles réguliers.
 *
 * @param[in]    data   Le SnakeActor du snake.
 */
gboolean timeout_tick_cb(gpointer data)
{
    Partie *partie = data;

    snake_forward(partie->snake);// struct.c

    snake_set_direction_ia(partie,"ia1");// ia.c
    snake_forward(partie->schlanga);

    gestion_collisions_check(partie->collisions);

    if (partie->en_cours)
        affichage_update(partie->affichage);

    return partie->en_cours;
}
