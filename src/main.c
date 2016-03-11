#include <clutter/clutter.h>
#include "affichage.h"

int main(int argc, char **argv)
{
	ClutterScript *ui;
    GObject *stage;
    GError *err = NULL;
    ClutterInitError cie;
    cie = clutter_init(NULL, NULL);



    struct _snake_actor *sa;

    // ClutterContent *image = generate_image("data/menu.jpg");


    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "src/gui/menu.json", &err);
    
    if (err != NULL) {
        printf("%s\n", err->message);
        err=NULL;
    }

    stage = clutter_script_get_object(ui, "stage");

    /* Relie les signaux définis dans le fichier JSON */
    clutter_script_connect_signals(ui, ui);

    /* connecter à la main :
    g_signal_connect (bouton,
                   "Clicked",
                   G_CALLBACK (_button_clicked_cb),
                   actor_to_move);  */

    clutter_actor_show(CLUTTER_ACTOR(stage));

    clutter_main();


    return 0;
	 
}
 /*
    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "src/gui/stage.json", NULL);
    clutter_script_load_from_file(ui, "src/gui/menu.json", NULL);
*/