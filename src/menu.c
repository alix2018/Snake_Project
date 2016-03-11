#include "affichage.h"
#include "menu.h"

gboolean jouer_clicked_cb(ClutterClickAction *action,
                            ClutterActor *actor,
                            gpointer data)
{
	//clutter_actor_hide(actor);
	printf("Jouer\n");

    ClutterScript *ui;


    GError *err = NULL;
    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "src/gui/stage.json", &err);

    if (err != NULL) {
        printf("%s\n", err->message);
        err=NULL;
    }

    init_view(ui, 30, 30, DROITE, 20, coord_from_xy(22, 2));
    
	return TRUE;
}

gboolean quitter_clicked_cb(ClutterClickAction *action,
                            ClutterActor *actor,
                            gpointer data)
{

	printf("Quitter\n");
    clutter_main_quit();
	return TRUE;
}

gboolean button_enter_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data) {
    ClutterColor color = { 150, 234, 254, 255 };

    clutter_actor_set_background_color(actor, &color);
    return TRUE;
}

gboolean button_leave_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data) {

    clutter_actor_set_background_color(actor, CLUTTER_COLOR_White);
    return TRUE;
}
