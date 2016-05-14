//
// Created by stef on 14/05/16.
//

#include "application.h"

gboolean jouer_clicked_cb(ClutterClickAction *action,
                          ClutterActor *actor,
                          gpointer data)
{
    Application *app = data;

    clutter_actor_remove_child(app->stage, app->menu_general);
    clutter_actor_add_child(app->stage, app->menu_partie);

    return CLUTTER_EVENT_STOP;
}

gboolean bouton_partie_simple_clicked_cb(ClutterClickAction *action,
                                         ClutterActor *actor,
                                         gpointer data)
{
    Application *app = data;
    Partie * partie;

    clutter_actor_remove_child(app->stage, app->menu_partie);

    partie = create_partie();
    init_partie(partie, app->ui, 30, 30); // TODO récupérer depuis app
    init_pseudo(partie, 0, NULL);

    return CLUTTER_EVENT_STOP;
}

void init_application(Application *app, int width, int height){
    GError *err = NULL;

    app->partie = NULL;

    app->ui = clutter_script_new();
    clutter_script_load_from_file(app->ui, "src/gui/stage.json", &err);
    if (err != NULL)
    {
        perror(err->message);
        err = NULL;
    }
    clutter_script_load_from_file(app->ui, "src/gui/menu.json", &err);
    if (err != NULL)
    {
        perror(err->message);
        err = NULL;
    }

    clutter_script_connect_signals(app->ui, app);
    clutter_script_get_objects(
        app->ui,
        "menu_partie", &app->menu_partie,
        "menu_general", &app->menu_general,
        "stage", &app->stage,
        NULL
    );

    clutter_actor_add_child(app->stage, app->menu_general);
    clutter_actor_set_size(app->stage, width * GRID_SIZE, height * GRID_SIZE);

    g_signal_connect(app->stage, "destroy", G_CALLBACK(stage_destroy_cb), NULL);

    //app->partie = create_partie();
    //init_partie(app->partie, app->ui, 30, 30);
    clutter_actor_show(app->stage);
    clutter_main();

    if (app->partie != NULL)
        free_partie(app->partie);
};