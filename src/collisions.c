
#include <stddef.h>
#include <stdlib.h>
#include "collisions.h"

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

GestionCollisions *create_gestion_collisions()
{
    GestionCollisions *res;

    res = malloc(sizeof(GestionCollisions));
    res->nb_objs = 0;
    res->taille_objs = 8;
    res->objs = malloc(res->taille_objs * sizeof(GestionCollisions *));
    return res;
}

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


static void check_collisions_for_snake(CollisionObject *obj) {
    int i;
    List *liste_snake = snake_liste_snake((Snake *) obj->obj2);
    Node cur;
    Coord *cur_coord;

    for (cur = list_first_node(liste_snake);
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
            }
        }
    }
}

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

void collision_object_set_enabled(CollisionObject *object, int enabled)
{
    object->enabled = enabled;
}

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

void free_collision(Collision *collision)
{
    free(collision);
}

void collision_set_enabled(Collision *collision, int enabled)
{
    collision->enabled = enabled;
}

void collision_trigger(Collision *collision)
{
    collision->cb(collision->obj1, collision->obj2, collision->data);
}
