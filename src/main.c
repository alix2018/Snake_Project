#include <clutter/clutter.h>
#include "affichage.h"

int main(int argc, char **argv)
{
    ClutterScript *ui;
    struct _snake_actor *sa;

    ClutterInitError err = clutter_init(&argc, &argv);

    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "src/gui/stage.json", NULL);
    clutter_script_connect_signals(ui, ui);

    init_view(ui, 30, 30, DROITE, 20, coord_from_xy(22, 2));

    return 0;
}
	