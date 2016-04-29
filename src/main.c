#include <clutter/clutter.h>
#include "partie.h"
#include "multi.h"
/*
    Usage :
    Partie simple local :
        ./alphasnake <pseudo>
    Partie multijoueur réseau :
        ./alphasnake <pseudo> H         Pour l'hôte
        ./alphasnake <pseudo> <IP hôte> Pour un joueur
*/
int main(int argc, char **argv)
{
	
    ClutterScript *ui;
    Partie *partie;

    ClutterInitError err = clutter_init(NULL, NULL);

    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "src/gui/stage.json", NULL);
    clutter_script_connect_signals(ui, ui);

    partie = create_partie();// partie.c
    if(argc <= 2)
    {
        init_partie(partie, ui, 3, 30, 30);// partie.c
        init_pseudo(partie, argc, argv);
    }
    else
    {
        init_multi(argc, argv, partie, ui, 30, 30);
    }
    
    clutter_main();

    free_partie(partie);

    return 0;
	 
}
	
