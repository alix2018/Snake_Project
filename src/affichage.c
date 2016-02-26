#include <clutter/clutter.h>
#include "affichage.h"

gboolean zone_snake_key_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer data)
{
    struct snake_actor *sa = data;
    guint key_symbol = clutter_event_get_key_symbol(event);

    switch (key_symbol)
    {
        case CLUTTER_KEY_Up:
            sa->direction = HAUT;
            break;
        case CLUTTER_KEY_Down:
            sa->direction = BAS;
            break;
        case CLUTTER_KEY_Left:
            sa->direction = GAUCHE;
            break;
        case CLUTTER_KEY_Right:
            sa->direction = DROITE;
        default:
            break;
    }

    return TRUE;
}

gboolean timeout_tick_cb(gpointer data)
{
    struct snake_actor *sa = data;
    gfloat x, y;

    clutter_actor_get_position(sa->rectangle, &x, &y);

    switch (sa->direction)
    {
        case HAUT:
            y -= 10;
            break;
        case BAS:
            y += 10;
            break;
        case GAUCHE:
            x -= 10;
            break;
        case DROITE:
            x += 10;
            break;
        default:
            break;
    }

    clutter_actor_set_position(sa->rectangle, x, y);

    return G_SOURCE_CONTINUE;
}

void init_affichage(ClutterScript *ui, struct snake_actor *sa)
{
    ClutterActor *zone_snake;
    ClutterStage *stage;

    sa->direction = DROITE;
    zone_snake = CLUTTER_ACTOR(clutter_script_get_object(ui, "zone_snake"));
    stage = CLUTTER_STAGE(clutter_script_get_object(ui, "stage"));
    clutter_stage_set_key_focus(stage, zone_snake);

    sa->rectangle = clutter_actor_new();
    clutter_actor_set_size(sa->rectangle, 10, 10);
    clutter_actor_set_position(sa->rectangle, 10, 10);
    clutter_actor_set_background_color(sa->rectangle, CLUTTER_COLOR_Blue);
    clutter_actor_set_easing_duration(sa->rectangle, 150);
    clutter_actor_add_child(zone_snake, sa->rectangle);
    g_signal_connect(zone_snake, "key-press-event", G_CALLBACK(zone_snake_key_press_cb), sa);

    g_timeout_add(300, timeout_tick_cb, sa);
}
