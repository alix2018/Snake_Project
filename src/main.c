#include <clutter/clutter.h>
#include "partie.h"
#include "config.h"

int main(int argc, char **argv)
{
	
    ClutterScript *ui;
    Partie *partie;

    ClutterInitError err = clutter_init(&argc, &argv);

    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "src/gui/stage.json", NULL);
    clutter_script_connect_signals(ui, ui);

    partie = create_partie();// partie.c
    Config * conf = init_config(); // TODO ajouter param en fonction du menu etc..
    partie_set_config(partie,conf);
    init_partie(partie, ui);// partie.c
    init_pseudo(partie, argc, argv);
    clutter_main();

    free_partie(partie);

    return 0;
	 
}
	
