
#include <stddef.h>
#include <stdlib.h>
#include "collisions.h"
#include "partie.h"
#include "struc.h"

struct _CollisionObject
{
    CollisionType type;
    int enabled;
    void *obj2;
    Collision **collisions;
    int nb_collisions;
    int taille_collisions;
};

struct _Collision
{
    CollisionCb cb;
    int enabled;
    Snake *obj1;
    void *obj2;
    void *data;
};

struct _GestionCollisions
{
    CollisionObject **objs;
    int nb_objs;
    int taille_objs;
};


/**
 * @brief   Crée un gestionnaire de collisions.
 *
 * @return  Le gestionnaire de collisions.
 */
GestionCollisions *create_gestion_collisions()
{
    GestionCollisions *res;

    res = malloc(sizeof(GestionCollisions));
    res->nb_objs = 0;
    res->taille_objs = 8;
    res->objs = malloc(res->taille_objs * sizeof(GestionCollisions *));
    return res;
}

/**
 * @brief   Libère la mémoire consommée par un gestionnaire de collisions.
 *
 * @param[in]    collision  Le gestionnaire de collisions à supprimer.
 */
void free_gestion_collisions(GestionCollisions *collisions)
{
    int i;

    for (i = 0; i < collisions->nb_objs; i++)
    {
        free_collision_object(collisions->objs[i]);
    }

    free(collisions->objs);
    free(collisions);
}


/**
 * @brief   Ajoute un objet à un gestionnaire de collisions.
 *
 * @param[in]    gc     Le gestionnaire de collisions.
 * @param[in]    obj    L'objet à ajouter.
 * @param[in]    type   Le type de l'objet à ajouter.
 *
 * @return  Le CollisionObject crée lors de l'ajout de obj, ou le
 *          CollisionObject qui gère obj si obj est déjà géré par gc.
 */
CollisionObject *gestion_collision_add_object(GestionCollisions *gc, void *obj,
                                              CollisionType type)
{
    int i;

    for (i = 0; i < gc->nb_objs; i++)
    {
        if (gc->objs[i]->obj2 == obj)
            return gc->objs[i];
    }

    gc->nb_objs++;
    if (gc->nb_objs > gc->taille_objs)
    {
        gc->taille_objs *= 2;
        gc->objs = realloc(gc->objs,
                           gc->taille_objs * sizeof(CollisionObject *));
    }

    gc->objs[gc->nb_objs - 1] = create_collision_object(obj, type);

    return gc->objs[gc->nb_objs - 1];
}


/**
 * @brief   Supprime le CollisionObject qui gère obj de gc.
 *
 * @param[in]    gc     Le gestionnaire de collisions.
 * @param[in]    obj    L'objet à supprimer.
 */
void gestion_collision_remove_object(GestionCollisions *gc, void *obj)
{
    int i = 0;

    while (i < gc->nb_objs)
    {
        if (gc->objs[i]->obj2 == obj)
        {
            free_collision_object(gc->objs[i]);
            gc->objs[i] = gc->objs[gc->nb_objs - 1];
            gc->objs[gc->nb_objs - 1] = NULL;
            gc->nb_objs--;
            return;
        }
    }
}


/**
 * @brief   Vérifie toutes les collisions activées pour un CollisionObject de
 *          type snake.
 *
 * @param[in]    obj    Le CollisionObject dont il faut vérifier les collisions.
 */
static void check_collisions_for_snake(CollisionObject *obj) {
    int i;
    Snake *snake = (Snake *) obj->obj2;
    List *liste_snake = snake_liste_snake(snake);
    Node cur;
    Coord *cur_coord;

    cur = list_first_node(liste_snake);
    for (i = 0; i < obj->nb_collisions; i++)
    {
        cur_coord = node_elt(cur);
        for (i = 0; i < obj->nb_collisions; i++)
        {
            if (obj->collisions[i]->obj1 != snake
                && obj->collisions[i]->enabled
                && coord_egales(*cur_coord,
                                snake_pos(obj->collisions[i]->obj1)))
            {
                collision_trigger(obj->collisions[i]);
            }
        }
    }

    for (cur = node_next(cur);
         cur != NULL;
         cur = node_next(cur))
    {
        cur_coord = node_elt(cur);
        for (i = 0; i < obj->nb_collisions; i++)
        {
            if (obj->collisions[i]->enabled
                && coord_egales(*cur_coord,
                                snake_pos(obj->collisions[i]->obj1)))
            {
                collision_trigger(obj->collisions[i]);
            }
        }
    }
}


/**
 * @brief Vérifie les collisions activées  pour un CollisionObject de type
 *        bonus.
 *
 * @param[in]    obj    Le CollisionObject dont il faut vérifier les collisions.
 */
static void check_collisions_for_bonus(CollisionObject *obj)
{
    Bouf *bonus = obj->obj2;
    int i;

    for (i = 0; i < obj->nb_collisions; i++)
    {
        if (obj->collisions[i]->enabled
            && coord_egales(bouf_coord(bonus),
                         snake_pos(obj->collisions[i]->obj1)))
        {
            collision_trigger(obj->collisions[i]);
        }
    }
}

/**
 * @brief Vérifie les collisions activées  pour un CollisionObject de type
 *        map.
 *
 * @param[in]    obj    Le CollisionObject dont il faut vérifier les collisions.
 */
static void check_collisions_for_map(CollisionObject *obj)
{
    Map *map = obj->obj2;
    int i;

    for (i = 0; i < obj->nb_collisions; i++)
    {
        if (obj->collisions[i]->enabled)
        {
            Coord coord = snake_pos(obj->collisions[i]->obj1);
            if (coord.x > map_width(map)-1
                || coord.y > map_height(map)-1
                || coord.x < 0
                || coord.y < 0)
            {
                collision_trigger(obj->collisions[i]);
            }
        }
    }
}

/**
 * @brief Vérifie toutes les collisions activées pour tous les CollisionObject
 *        activés.
 *
 * @param[in]    collisions Le gestionnaire de collisions qui contient les
 *                          CollisionObject.
 */
void gestion_collisions_check(GestionCollisions *collisions)
{
    int i;
    CollisionObject *obj;

    for (i = 0; i < collisions->nb_objs; i++)
    {
        obj = collisions->objs[i];
        if (obj->enabled && obj->nb_collisions > 0)
        {
            switch (obj->type)
            {
                case COLLISION_SNAKE:
                    check_collisions_for_snake(obj);
                    break;
                case COLLISION_BONUS:
                    check_collisions_for_bonus(obj);
                    break;
                case COLLISION_MAP:
                    check_collisions_for_map(obj);
                    break;
            }
        }
    }
}


/**
 * @brief   Crée un nouveau CollisionObject.
 *
 * @param[in]    obj    L'objet qui pourra recevoir des collisions.
 * @param[in]    type   Le type de obj.
 *
 * @return Le CollisionObject crée.
 */
CollisionObject *create_collision_object(void *obj, CollisionType type)
{
    CollisionObject *res;

    res = malloc(sizeof(CollisionObject));
    res->enabled = 1;
    res->nb_collisions = 0;
    res->taille_collisions = 8;
    res->collisions = malloc(res->taille_collisions * sizeof(Collision *));
    res->obj2 = obj;
    res->type = type;

    return res;
}


/**
 * @brief   Libère la mémoire consommée par un CollisionObject.
 *
 * @param[in]    object Le CollisionObject à libérer.
 */
void free_collision_object(CollisionObject *object)
{
    int i;

    for (i = 0; i < object->nb_collisions; i++)
    {
        free_collision(object->collisions[i]);
    }

    free(object->collisions);
    free(object);
}


/**
 * @brief   Active ou désactive une collision.
 */
void collision_object_set_enabled(CollisionObject *object, int enabled)
{
    object->enabled = enabled;
}


/**
 * @brief   Ajoute une collision à un CollisionObject.
 *
 * @param[in]    obj        Le CollisionObject qui sera modifié.
 * @param[in]    collision  La collision à ajouter.
 */
void collision_object_add_collision(CollisionObject *obj, Collision *collision)
{
    collision->obj2 = obj->obj2;

    obj->nb_collisions++;

    if (obj->nb_collisions > obj->taille_collisions)
    {
        obj->taille_collisions *= 2;
        obj->collisions = realloc(obj->collisions,
                                  obj->taille_collisions * sizeof(Collision *));
    }

    obj->collisions[obj->nb_collisions - 1] = collision;
}


/**
 * @brief   Crée une nouvelle condition.
 *
 * @param[in]    snake  Le Snake qui entrera en collision avec les autres
 *                      objets présents sur le plateau.
 * @param[in]    cb     La fonction callback qui sera appelée lorsque le snake
 *                      passé en paramètre entrera en collision avec un objet.
 * @param[in]    data   Un pointeur qui sera passé en paramètre à la fonction
 *                      callback.
 *
 * @return La collision crée.
 */
Collision *create_collision(Snake *snake, CollisionCb cb, void *data)
{
    Collision *res;

    res = malloc(sizeof(Collision));
    res->obj1 = snake;
    res->obj2 = NULL;
    res->cb = cb;
    res->data = data;
    res->enabled = 1;

    return res;
}

/**
 * @brief   Libère la mémoire consommée par une collision.
 *
 * @param[in]    collision  La Collision à libérer.
 */
void free_collision(Collision *collision)
{
    free(collision);
}


/**
 * @brief   Active ou désactive une collision. Les collisions désactivées ne
 *          déclenchent plus la fonction callback qui leur est associée.
 *
 * @param[in]    collision  La collision à modifier.
 * @param[in]    enabled    1 si la collision doit être activée, 0 sinon.
 */
void collision_set_enabled(Collision *collision, int enabled)
{
    collision->enabled = enabled;
}


/**
 * Appelle la fonction callback définie dans collision.
 *
 * @param[in]    collision  La collision qui a été déclenchée.
 */
void collision_trigger(Collision *collision)
{
    collision->cb(collision->obj1, collision->obj2, collision->data);
}
