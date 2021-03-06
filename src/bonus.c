//
// Created by thibaut on 14/03/16.
//

#include "bonus.h"
#include "struc.h"
#include "affichage.h"

struct _bouf
{
    Coord coord;
};

struct _bouf_actor
{
    ClutterActor *parent;
    ClutterActor *bouf_c_actor;
    Bouf *bouf;
    ClutterColor *color;
};

/**
 * @brief   Génère un élèment de type bouf
 *
 * @param[in]    x  largeur de la map
 * @param[in]    y  hauteur de la map
 *
 * @return La bouf initialisé
 */
Bouf *bouf_new(int x, int y)
{
    srand(time(NULL));
    Bouf *new = malloc(sizeof(Bouf *));
    new->coord = coord_from_xy(rand()%x, rand()%y);
    return new;
}

/**
 * @brief   Change les coordonnés de la bouf
 *
 * @param[in]   bouf  La bouf à rafraichir
 * @param[in]   x     Largeur de la map
 * @param[in]   y     Hauteur de la map
 *
 * @return La bouf avec de nouvelles coordonnées
 */
void bouf_update(Bouf *bouf, int x, int y)
{
    bouf->coord = coord_from_xy(rand()%x, rand()%y);
}

/**
 * @brief   Permet d'accéder au coordonnées de la bouf
 *
 * @param[in]   bouf  La bouf dont on veut les coordonnées
 *
 * @return Les coordonnées de la bouf
 */
Coord bouf_coord(Bouf *bouf)
{
    return bouf->coord;
}

/**
 * @brief   Génère une BoufActor
 *
 * @param[in]   parent  Le parent du nouveau BoufActor
 * @param[in]   b       La bouf à afficher
 * @param[in]   color   La couleur de la bouf
 *
 * @return Le nouveau BoufActor initialisé
 */
BoufActor *create_bouf_actor(ClutterActor *parent, Bouf *b, ClutterColor *color)
{
    BoufActor *res;
    ClutterActor *bouf_c_actor;
    ClutterTransition *transition_x;
    ClutterTransition *transition_y;

    bouf_c_actor = clutter_actor_new();
    clutter_actor_set_size(bouf_c_actor, GRID_SIZE, GRID_SIZE);
    clutter_actor_set_pivot_point(bouf_c_actor, 0.5, 0.5);
    clutter_actor_set_scale(bouf_c_actor, 0.8, 0.8);
    //clutter_actor_set_background_color(bouf_c_actor, color);

    // SET IMAGE POMME/BOUF/bonus

    ClutterContent *imgpomme = generate_image(POMME_IMAGE );
    clutter_actor_set_content(bouf_c_actor,imgpomme);
    g_object_unref(imgpomme);

    transition_x = clutter_property_transition_new("scale-x");
    g_object_set(
        transition_x,
        "duration", 1000,
        "auto-reverse", TRUE,
        "repeat-count", -1,
        "progress-mode", CLUTTER_EASE_IN_OUT_SINE,
        NULL
    );
    clutter_transition_set_from(transition_x, G_TYPE_DOUBLE, 0.8);
    clutter_transition_set_to(transition_x, G_TYPE_DOUBLE, 1.2);
    clutter_actor_add_transition(bouf_c_actor, "tr-scale-x", transition_x);
    g_object_unref(transition_x);

    transition_y = clutter_property_transition_new("scale-y");
    g_object_set(
        transition_y,
        "duration", 1000,
        "auto-reverse", TRUE,
        "repeat-count", -1,
        "progress-mode", CLUTTER_EASE_IN_OUT_SINE,
        NULL
    );
    clutter_transition_set_from(transition_y, G_TYPE_DOUBLE, 0.8);
    clutter_transition_set_to(transition_y, G_TYPE_DOUBLE, 1.2);
    clutter_actor_add_transition(bouf_c_actor, "tr-scale-y", transition_y);
    g_object_unref(transition_y);

    clutter_actor_add_child(parent, bouf_c_actor);
    clutter_actor_set_position(bouf_c_actor, b->coord.x * GRID_SIZE, b->coord.y * GRID_SIZE);

    res = malloc(sizeof(BoufActor));
    res->parent = parent;
    res->bouf_c_actor = bouf_c_actor;
    res->color = color;
    res->bouf = b;

    return res;
}

void free_bouf_actor(BoufActor *b)
{
    clutter_color_free(b->color);
    free(b);
}

/**
 * @brief   Permet d'accéder à la bouf d'un BoufActor
 *
 * @param[in]   bouf_actor Le BoufActor
 *
 * @return La bouf du bouf_actor
 */
Bouf *bouf_actor_bouf(BoufActor *bouf_actor)
{
    return bouf_actor->bouf;
}

/**
 * @brief   Rafraîchit la bouf d'un BoufActor
 *
 * @param[in]   ba   Le BoufActor à rafraichir
 */
void bouf_actor_update(BoufActor *ba)
{
    clutter_actor_set_position(ba->bouf_c_actor, ba->bouf->coord.x * GRID_SIZE, ba->bouf->coord.y * GRID_SIZE);
}