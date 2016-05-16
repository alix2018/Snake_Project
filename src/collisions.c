/**
 * @file      collisions.c
 * @author    alpha-snake
 * @version   1
 * @date      25/02/2016
 * @brief     Toutes les fonctions permettant de créer les collisions et les détecter.
 * @details   ---
 */


#include <stddef.h>
#include <stdlib.h>
#include "collisions.h"
#include "partie.h"
#include "struc.h"

/**
 * @brief      La structure d'un object collision
 *
 * @use CollisionObject
 * @param[in]  type                  Le type de l'object collision.
 * @param[in]  enabled               Si la collision est active.
 * @param[in]  obj2                  Objet de la collision.
 * @param[in]  collisions            Le tableau des collisions.
 * @param[in]  nb_collisions         Le nombre de collisions.
 * @param[in]  taille_collisions     La taille des collisions.
 */
struct _CollisionObject
{
    CollisionType type;
    int enabled;
    void *obj2;
    Collision **collisions;
    int nb_collisions;
    int taille_collisions;
};


/**
 * @brief      La structure d'une collision
 *
 * @use Collision
 * @param[in]  cb                  Le type de l'object collision.
 * @param[in]  enabled             Si la collision est active.
 * @param[in]  obj1                Le snake.
 * @param[in]  obj2                La collision.
 * @param[in]  data
 */
struct _Collision
{
    CollisionCb cb;
    int enabled;
    Snake *obj1;
    void *obj2;
    void *data;
};



/**
 * @brief      La structure d'un gestionnaire de collisions.
 *
 * @use GestionCollisions
 * @param[in]  objs             L'objet de collision.
 * @param[in]  nb_objs          Le nombre de CollisionObject.
 * @param[in]  taille_objs      La taille du CollisionObject.
 */
struct _GestionCollisions
{
    CollisionObject **objs;
    int nb_objs;
    int taille_objs;
};


CollisionObject * get_collision_from_gestion_and_object(GestionCollisions *gs, void * obj)
{
    int i;
    for (i = 0; i < gs->nb_objs; i++)
    {
        if(gs->objs[i]->obj2 == obj)
        {
            return gs->objs[i];
        }
    }
    return NULL;
}

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
    int i;

    for (i = 0; i < gc->nb_objs; i++)
    {
        if (gc->objs[i]->obj2 == obj)
        {
            free_collision_object(gc->objs[i]);
            gc->objs[i] = gc->objs[gc->nb_objs - 1];
            gc->objs[gc->nb_objs - 1] = NULL;
            gc->nb_objs--;
        }
        else
        {
            collision_object_remove_object(gc->objs[i], obj);
        }
    }
}

/**
 * @brief Teste une collision pour un snake. La fonction callback
 *        n'est pas appelée.
 *
 * @param[in]   collision  La collision à tester.
 *
 * @return    1 si le snake obj1 est en collision avec le snake obj2,
 *            0 sinon.
 */
int check_collision_for_snake(Collision *collision)
{
    int i;
    Coord pos_snake = snake_pos((Snake *) collision->obj1);
    Node cur_cible = snake_premier((Snake *) collision->obj2);
    Coord *cur_cible_coord;

    if (collision->obj1 != collision->obj2) {
	cur_cible_coord = node_elt(cur_cible);
	if (coord_egales(pos_snake, *cur_cible_coord))
	    return 1;
    }

    for (cur_cible = node_next(cur_cible);
	 cur_cible != NULL;
         cur_cible = node_next(cur_cible))
    {
	cur_cible_coord = node_elt(cur_cible);
	if (coord_egales(pos_snake, *cur_cible_coord))
	    return 1;
    }

    return 0;
}

/**
 * @brief   Vérifie toutes les collisions activées pour un CollisionObject de
 *          type snake.
 *
 * @param[in]    obj    Le CollisionObject dont il faut vérifier les collisions.
 */
static void check_all_collisions_for_snake(CollisionObject *obj) {
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
 * @brief Teste une collision pour un bonus. La fonction callback
 *        n'est pas appelée.
 *
 * @param[in]   collision  La collision à tester.
 *
 * @return    1 si le snake est en collision avec le bonus, 0 sinon.
 */
int check_collision_for_bonus(Collision *collision)
{
    Bonus *bonus = collision->obj2;

    return collision->enabled && coord_egales(bonus_coord(bonus), snake_pos(collision->obj1));
}

/**
 * @brief Vérifie les collisions activées  pour un CollisionObject de type
 *        bonus.
 *
 * @param[in]    obj    Le CollisionObject dont il faut vérifier les collisions.
 */
static void check_all_collisions_for_bonus(CollisionObject *obj)
{
    int i;

    for (i = 0; i < obj->nb_collisions; i++)
    {
        if (check_collision_for_bonus(obj->collisions[i]))
	    collision_trigger(obj->collisions[i]);
    }
}

/**
 * @brief Teste une collision pour le plateau. La fonction callback
 *        n'est pas appelée.
 *
 * @param[in]   collision  La collision à tester.
 *
 * @return    1 si le snake sort du plateau, 0 sinon.
 */
static int check_collision_for_map(Collision *collision)
{
    Map *map = collision->obj2;
    Coord coord = snake_pos((Snake *) collision->obj1);

    return coord.x > map_width(map)-1
                || coord.y > map_height(map)-1
                || coord.x < 0
                || coord.y < 0;
}

/**
 * @brief Vérifie les collisions activées  pour un CollisionObject de type
 *        map.
 *
 * @param[in]    obj    Le CollisionObject dont il faut vérifier les collisions.
 */
static void check_all_collisions_for_map(CollisionObject *obj)
{
    int i;

    for (i = 0; i < obj->nb_collisions; i++)
    {
        if (obj->collisions[i]->enabled)
        {
            if (check_collision_for_map(obj->collisions[i]))
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
                    check_all_collisions_for_snake(obj);
                    break;
                case COLLISION_BONUS:
                    check_all_collisions_for_bonus(obj);
                    break;
                case COLLISION_MAP:
                    check_all_collisions_for_map(obj);
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

void collision_object_remove_object(CollisionObject *obj, void *obj1)
{
    int i;

    for (i = 0; i < obj->nb_collisions; i++)
    {
        if (obj->collisions[i]->obj1 == obj1)
        {
            free_collision(obj->collisions[i]);
            obj->collisions[i] = obj->collisions[obj->nb_collisions - 1];
            obj->nb_collisions--;
            return;
        }
    }
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
