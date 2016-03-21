
#include "struc.h"
#include "bonus.h"
#include "struc.h"

#ifndef ALPHA_SNAKE_COLLISIONS_H
#define ALPHA_SNAKE_COLLISIONS_H

typedef enum
{
    COLLISION_SNAKE,
    COLLISION_BONUS,
    COLLISION_MAP
} CollisionType;

typedef void (*CollisionCb)(Snake *obj1, void *obj2, void *data);
typedef struct _GestionCollisions GestionCollisions;
typedef struct _CollisionObject CollisionObject;
typedef struct _Collision Collision;

GestionCollisions *create_gestion_collisions();
void free_gestion_collisions(GestionCollisions *collisions);
CollisionObject * gestion_collision_add_object(GestionCollisions *gc, void *obj,
                                               CollisionType type);
void gestion_collision_remove_object(GestionCollisions *gc, void *obj);
void gestion_collisions_check(GestionCollisions *collisions);

CollisionObject *create_collision_object(void *obj, CollisionType type);
void free_collision_object(CollisionObject *object);
void collision_object_set_enabled(CollisionObject *object, int enabled);
void collision_object_add_collision(CollisionObject *obj, Collision *collision);

Collision *create_collision(Snake *snake, CollisionCb cb, void *data);
void free_collision(Collision *collision);
void collision_set_enabled(Collision *collision, int enabled);
void collision_trigger(Collision *collision);

#endif //ALPHA_SNAKE_COLLISIONS_H
