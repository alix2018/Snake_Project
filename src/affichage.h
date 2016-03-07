#include <clutter/clutter.h>
#include "struc.h"

#ifndef ALPHA_SNAKE_AFFICHAGE_H
#define ALPHA_SNAKE_AFFICHAGE_H

/**
 * Cette structure contient les ClutterActor nécessaires à l'affichage du snake.
 */
typedef struct _snake_actor SnakeActor;
typedef struct _snake_image SnakeImage;

typedef struct _bouf Bouf;
typedef struct _bouf_actor BoufActor;
typedef struct _uplet_actor UpletActor;

//Créé un nouvelle emplacement mémoir pour la bouf
Bouf *bouf_new(int x, int y);

//Créé les coordonnés de la bouf sur la map
void bouf_update(Bouf *bouf, int x, int y);

//Créé le BoufActor
BoufActor *create_bouf_actor(ClutterActor *parent, Bouf *b, ClutterColor *color);

//Met à jour les coord du clutteractor
void bouf_actor_update(BoufActor *ba);

//Génère un 3-uplet, utile pour la fonction timeout
UpletActor uplet_actor_new(SnakeActor *a1, SnakeActor *a2, BoufActor *bouf);

//???
gboolean zone_snake_key_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer data);

//Renvoie 1 si le snake se prend un mur
int snake_border_map(SnakeActor *sa);

//Renvoie 0 si le snake essaie de se mordre la queu, 1 sinon
int snake_border_snake(SnakeActor *sa);

//Renvoie 1 si et seulement si les coords de la te et de la queue sont égales
int snake_eat(Snake *s, Bouf *b);

//Fonction appelé en boule, on doit lui passer un arg de type UpletActor
gboolean timeout_tick_cb(gpointer data);

//Libere mémoir
void stage_destroy_cb(ClutterActor *actor, gpointer data);


SnakeActor *create_snake_actor(ClutterActor *parent, Snake *s, ClutterColor *color);

static void free_clutter_actor_fn(void * elt);
void free_snake_actor(SnakeActor *sa);
void snake_actor_update(SnakeActor *sa);
ClutterContent *generate_image(char * filename);
SnakeImage *snake_generate_image();

void init_view(ClutterScript *ui, int width, int height, Direction direction, int size, Coord pos);

#endif //ALPHA_SNAKE_AFFICHAGE_H
