//
// Created by thibaut on 16/03/16.
//

#include "partie.h"
#include "struc.h"
#include "bonus.h"
#include "affichage.h"
#include "ia.h"
#include "collisions.h"

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

Map *create_map(int width, int height)
{
    Map *res;

    res = malloc(sizeof(Map));
    res->width = width;
    res->height = height;

    return res;
}

void free_map(Map *map)
{
    free(map);
}

int map_width(Map *map)
{
    return map->width;
}

int map_height(Map *map)
{
    return map->height;
}

Partie *create_partie()
{
    Partie *res;

    res = malloc(sizeof(struct _Partie));
    res->snake = NULL;
    res->schlanga = NULL;
    res->nourriture = NULL;
    res->map = NULL;
    res->affichage = NULL;
    res->collisions = NULL;

    return res;
}

void free_partie(Partie *partie)
{
    free(partie->nourriture);
    free_affichage(partie->affichage);
    free_map(partie->map);
    free(partie);
}

Map *partie_map(Partie *partie)
{
    return partie->map;
}

static void collision_snake_vers_snake(Snake *snake, void *obj2, void *data)
{
    Partie *partie = data;
    ClutterScript *ui = affichage_ui(partie->affichage);
    ClutterActor *fin_partie;

    partie->en_cours = FALSE;

    clutter_text_set_text(
        CLUTTER_TEXT(clutter_script_get_object(ui, "fin_partie_texte")),
        (snake == partie->schlanga) ? "Gagné !" : "Perdu !"
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

static void collision_snake_vers_nourriture(Snake *snake, void *obj2, void *data)
{
    Bouf *nourriture = obj2;
    Partie *partie = data;

    snake_increase(snake);
    bouf_update(nourriture, partie->map->width, partie->map->height);
}

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
    );
    partie->snake = snake;
    schlanga = create_snake(
        10,
        coord_from_xy(22, 7),//L'ia commence 5 case en dessous du snake de base
        DROITE
    );
    partie->schlanga = schlanga;
    bouf = bouf_new(width, height);
    partie->nourriture = bouf;

    partie->collisions = create_gestion_collisions();
    co_snake = gestion_collision_add_object(partie->collisions, partie->snake,
                                            COLLISION_SNAKE);
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
        create_collision(partie->snake, collision_snake_vers_snake, partie)
    );
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

    partie->affichage = create_affichage();
    init_affichage(partie->affichage, ui, snake, width, height);

    affichage_add_snake(partie->affichage, snake, CLUTTER_COLOR_Red);
    affichage_add_snake(partie->affichage, schlanga, CLUTTER_COLOR_Blue);
    affichage_add_bonus(partie->affichage, bouf, CLUTTER_COLOR_Green);

    g_timeout_add(150, timeout_tick_cb, partie);
}

/**
 * @brief Fonction callback appelée à intervalles réguliers.
 *
 * @param[in]    data   Le SnakeActor du snake.
 */
gboolean timeout_tick_cb(gpointer data)
{
    Partie *partie = data;

    snake_forward(partie->snake);
    snake_forward_ia1(partie->schlanga, partie->snake,
                      bouf_coord(partie->nourriture));

    gestion_collisions_check(partie->collisions);

    affichage_update(partie->affichage);

    return partie->en_cours;
}
