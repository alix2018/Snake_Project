//
// Created by stef on 14/05/16.
//

#include "application.h"

Application init_application(Application *app){
    ClutterActor *menu_general, *stage;
    GError *err = NULL;

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
    clutter_script_get_objects(app->ui, "stage", &stage, "menu_general", &menu_general, NULL);

    clutter_actor_add_child(stage,menu_general);


    stage = CLUTTER_ACTOR(clutter_script_get_object(ui, "stage"));
    clutter_actor_set_size(stage, width * GRID_SIZE,
                           height * GRID_SIZE);

    app->partie = create_partie();// partie.c
    init_partie(app->partie, app->ui, 30, 30);// partie.c
    clutter_main();

    free_partie(app->partie);
};