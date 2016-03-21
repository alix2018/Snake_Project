#include <clutter/clutter.h>
#include "struc.h"
#include "bonus.h"


#ifndef ALPHA_SNAKE_AFFICHAGE_H
#define ALPHA_SNAKE_AFFICHAGE_H

#define GRID_SIZE 23
#define BACKGROUND_IMAGE_SRC "data/fond.jpg"
#define TETE_IMAGE_SRC "data/tetev1.png"
#define QUEUE_IMAGE_SRC "data/queuev1.png"
#define CORPS_IMAGE_SRC "data/corpsv1.png"
#define TURNLIGHT_IMAGE_SRC "data/corpsturnlightside.png"
#define TURNDARK_IMAGE_SRC "data/corpsturndarkside.png"
#define POMME_IMAGE "data/pommeapple.png"

typedef struct _Affichage Affichage;

/**
 * Cette structure contient les ClutterActor nécessaires à l'affichage du snake.
 */
typedef struct _snake_actor SnakeActor;
typedef struct _snake_image SnakeImage;

Affichage *create_affichage();
void free_affichage(Affichage * affichage);
ClutterScript *affichage_ui(Affichage *affichage);

//???
gboolean zone_snake_key_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer data);

//Renvoie 1 si le snake se prend un mur
int snake_border_map(SnakeActor *sa);

//Renvoie 0 si le snake essaie de se mordre la queu, 1 sinon
int snake_border_snake(SnakeActor *sa,SnakeActor * sa_ia);

//Renvoie 1 si et seulement si les coords de la te et de la queue sont égales
int snake_eat(Snake *s, Bouf *b);

//Libere mémoir
void stage_destroy_cb(ClutterActor *actor, gpointer data);


SnakeActor *create_snake_actor(ClutterActor *parent, Snake *s, ClutterColor *color, SnakeImage *imgs);

static void free_clutter_actor_fn(void * elt);
void free_snake_actor(SnakeActor *sa);
void snake_actor_update(SnakeActor *sa);
ClutterContent *generate_image(char * filename);
SnakeImage *snake_generate_image();

void affichage_add_snake(Affichage *affichage, Snake *snake,
                         ClutterColor *color);
void affichage_add_bonus(Affichage *affichage, Bouf *bonus,
                         ClutterColor *color);

void affichage_update(Affichage *affichage);

void init_affichage(Affichage *affichage, ClutterScript *ui, Snake *snake,
                    int width, int height);

#endif //ALPHA_SNAKE_AFFICHAGE_H
