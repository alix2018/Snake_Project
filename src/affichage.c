#include <clutter/clutter.h>
#include "affichage.h"
#include "list.h"
#include "struc.h"

#define GRID_SIZE 10

struct _snake_actor
{
    List *actors;
    ClutterActor *parent;
    Snake *snake;
    int cur_size;
};

gboolean zone_snake_key_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer data)
{
    Snake *s = data;
    guint key_symbol = clutter_event_get_key_symbol(event);

    switch (key_symbol)
    {
        case CLUTTER_KEY_Up:
            if (snake_direction(s) != BAS)
                snake_set_direction(s, HAUT);
            break;
        case CLUTTER_KEY_Down:
            if (snake_direction(s) != HAUT)
                snake_set_direction(s, BAS);
            break;
        case CLUTTER_KEY_Left:
            if (snake_direction(s) != DROITE)
                snake_set_direction(s, GAUCHE);
            break;
        case CLUTTER_KEY_Right:
            if (snake_direction(s) != GAUCHE)
                snake_set_direction(s, DROITE);
        default:
            break;
    }

    return TRUE;
}

gboolean timeout_tick_cb(gpointer data)
{
    SnakeActor *sa = data;
    gfloat x, y;

    snake_forward(sa->snake);
    snake_actor_update(sa);

    return G_SOURCE_CONTINUE;
}


SnakeActor *create_snake_actor(ClutterActor *parent, Snake *s)
{
    SnakeActor *res;

    res = malloc(sizeof(struct _snake_actor));
    res->actors = create_list();
    res->parent = parent;
    res->cur_size = 0;
    res->snake = s;

    return res;
}

static void free_clutter_actor_fn(void * elt)
{
    ClutterActor *actor = elt;

    clutter_actor_destroy(actor);
}

void free_snake_actor(SnakeActor *sa)
{
    free_list_fn(sa->actors, free_clutter_actor_fn);
    free(sa);
}

void snake_actor_update(SnakeActor *sa)
{
    int delta;
    ClutterActor *actor;
    Node node_sa;
    ListeSnake node_s;
    Coord c;

    delta = snake_longueur(sa->snake) - sa->cur_size;
    if (delta > 0)
    {
        for (; delta > 0; delta--)
        {
            actor = clutter_actor_new();
            clutter_actor_set_size(actor, GRID_SIZE, GRID_SIZE);
            clutter_actor_set_background_color(actor, CLUTTER_COLOR_Blue);
            clutter_actor_set_easing_duration(actor, 300);
            clutter_actor_add_child(sa->parent, actor);

            list_add_last(sa->actors, actor);
        }

        sa->cur_size = snake_longueur(sa->snake);
    }
    else if (delta < 0)
    {
        for (; delta < 0; delta++)
        {
            actor = list_pop_last(sa->actors);
            clutter_actor_destroy(actor);
        }

        sa->cur_size = snake_longueur(sa->snake);
    }

    node_s = snake_premier(sa->snake);
    for (node_sa = list_first_node(sa->actors);
         node_sa != NULL;
         node_sa = node_next(node_sa), node_s = liste_snake_suivant(node_s))
    {
        c = liste_snake_coord(node_s);
        actor = node_elt(node_sa);
        clutter_actor_set_position(
            actor,
            c.x * GRID_SIZE,
            c.y * GRID_SIZE
        );
    }
}

void init_view(ClutterScript *ui, int width, int height, Direction direction,
               int size, Coord pos)
{
    ClutterActor *zone_snake;
    ClutterActor *stage;
    Snake *snk;
    SnakeActor *sa;

    stage = CLUTTER_ACTOR(clutter_script_get_object(ui, "stage"));
    clutter_actor_set_size(stage, width * GRID_SIZE, height * GRID_SIZE);

    snk = create_snake(
        size,
        pos,
        direction
    );

    zone_snake = CLUTTER_ACTOR(clutter_script_get_object(ui, "zone_snake"));
    clutter_stage_set_key_focus(CLUTTER_STAGE(stage), zone_snake);
    sa = create_snake_actor(zone_snake, snk);
    snake_actor_update(sa);

    g_signal_connect(zone_snake, "key-press-event", G_CALLBACK(zone_snake_key_press_cb), snk);
    g_timeout_add(200, timeout_tick_cb, sa);

    clutter_actor_show(stage);

    clutter_main();

    free_snake(snk);
    free_snake_actor(sa);
}
