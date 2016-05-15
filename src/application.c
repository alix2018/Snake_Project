//
// Created by stef on 14/05/16.
//

#include "application.h"
#include "widgets/utils.h"

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

    app->partie = create_partie();
    partie_set_config(app->partie, app->config);
    init_partie(app->partie, app->ui); // TODO récupérer depuis app
    init_pseudo(app->partie, 0, NULL);

    return CLUTTER_EVENT_STOP;
}

/**
 * @brief        Fonction callback appelée lorsque la fenêtre est fermée.
 *
 * @param[in]    actor   L'acteur qui a émis le signal.
 * @param[in]    data    Le SnakeActor du snake.
 *
 * @return       Ce prototype est défini par Clutter, cf. la documentation de Clutter.
 */
void stage_destroy_cb(ClutterActor *actor, gpointer data)
{
    clutter_main_quit();
}

void init_application(Application *app)
{
    GError *err = NULL;
    ClutterImage *bg_menus;

    app->partie = NULL;
    app->config = init_config();

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
    
    bg_menus = create_clutter_image("data/fond_menu.png");
    clutter_actor_set_content(app->menu_general, CLUTTER_CONTENT(bg_menus));
    clutter_actor_set_content(app->menu_partie, CLUTTER_CONTENT(bg_menus));

    clutter_actor_add_child(app->stage, app->menu_general);
    clutter_actor_set_size(app->stage, app->config->width * app->config->grid_size,
                           app->config->height * app->config->grid_size);

    g_signal_connect(app->stage, "destroy", G_CALLBACK(stage_destroy_cb), NULL);

    clutter_actor_show(app->stage);
    clutter_main();

    if (app->partie != NULL)
        free_partie(app->partie);

    free_config(app->config);
};