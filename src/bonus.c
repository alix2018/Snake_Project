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
 * @brief   Genere un élèment de type bouf
 *
 * @param[in]    x  largeur de la map
 * @param[in]    y  hauteur de la mao
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
 * @param[in]   bouf la bouf à rafraichir
 * @param[in]   x  largeur de la map
 * @param[in]   y  hauteur de la mao
 *
 * @return La bouf avec de nouvelles coordonnés
 */
void bouf_update(Bouf *bouf, int x, int y)
{
    bouf->coord = coord_from_xy(rand()%x, rand()%y);
}

/**
 * @brief   Permet d'acceder au coord de la bouf
 *
 * @param[in]   bouf la bouf dont on veut les coord
 *
 * @return Les coord de la bouf
 */
Coord bouf_coord(Bouf *bouf)
{
    return bouf->coord;
}

/**
 * @brief   Génère une BoufActor
 *
 * @param[in]   parent Le parent du nouveau BoufActor
 * @param[in]   b  la bouf à afficher
 * @param[in]   color  la couleur de la bouf
 *
 * @return Le nouveau BoufActor initialisé
 */
BoufActor *create_bouf_actor(ClutterActor *parent, Bouf *b, ClutterColor *color)
{
    BoufActor *res;
    ClutterActor *bouf_c_actor;

    bouf_c_actor = clutter_actor_new();
    clutter_actor_set_size(bouf_c_actor, GRID_SIZE, GRID_SIZE);
    //clutter_actor_set_background_color(bouf_c_actor, color);

    // SET IMAGE POMME/BOUF/bonus

    ClutterContent *imgpomme = generate_image(POMME_IMAGE );
    clutter_actor_set_content(bouf_c_actor,imgpomme);

    clutter_actor_add_child(parent, bouf_c_actor);
    clutter_actor_set_position(bouf_c_actor, b->coord.x * GRID_SIZE, b->coord.y * GRID_SIZE);

    res = malloc(sizeof(BoufActor));
    res->parent = parent;
    res->bouf_c_actor = bouf_c_actor;
    res->color = color;
    res->bouf = b;

    return res;
}

/**
 * @brief   Permet d'acceder à la bouf d'un BoufActor
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
 * @brief   Rafraichie la bouf d'un BoufActor
 *
 * @param[in]   ba Le BoufActor à rafraichir
 */
void bouf_actor_update(BoufActor *ba)
{
    clutter_actor_set_position(ba->bouf_c_actor, ba->bouf->coord.x * GRID_SIZE, ba->bouf->coord.y * GRID_SIZE);
}