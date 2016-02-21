#include <clutter/clutter.h>

gboolean clic_souris_cb(ClutterActor *actor, ClutterEvent *event, gpointer data) {
    ClutterActor *rectangle = data;
    ClutterPoint *pos = clutter_point_alloc();

    clutter_event_get_position(event, pos);

    clutter_actor_save_easing_state(rectangle);

    clutter_actor_set_easing_duration(rectangle, 400);
    clutter_actor_set_easing_mode(rectangle, CLUTTER_EASE_IN_OUT_CUBIC);
    clutter_actor_set_position(rectangle, pos->x - 25, pos->y - 25);

    clutter_actor_restore_easing_state(rectangle);

    clutter_point_free(pos);

    return TRUE;
}

int main(int argc, char **argv) {
    ClutterActor *stage;
    ClutterActor *rectangle;

    clutter_init(&argc, &argv);

    stage = clutter_stage_new();
    clutter_stage_set_title(CLUTTER_STAGE(stage), "Essai Clutter");
    clutter_actor_set_size(stage, 400, 400);
    clutter_actor_set_background_color(stage, CLUTTER_COLOR_Black);
    clutter_actor_set_reactive(stage, TRUE);
    g_signal_connect(stage, "destroy", G_CALLBACK(clutter_main_quit), NULL);

    rectangle = clutter_actor_new();
    clutter_actor_set_size(rectangle, 50, 50);
    clutter_actor_set_position(rectangle, 50, 50);
    clutter_actor_set_background_color(rectangle, CLUTTER_COLOR_Blue);
    clutter_actor_add_child(stage, rectangle);
    g_signal_connect(stage, "button-press-event", G_CALLBACK(clic_souris_cb), rectangle);

    clutter_actor_show(stage);

    clutter_main();

    return 0;
}
