#include <clutter/clutter.h>
#include "affichage.h"

int main(int argc, char **argv)
{
    ClutterScript *ui;
    struct snake_actor *sa;

    ClutterInitError err = clutter_init(&argc, &argv);

    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "src/gui/stage.json", NULL);
    clutter_script_connect_signals(ui, ui);

    sa = g_malloc(sizeof(struct snake_actor));
    init_affichage(ui, sa);

    clutter_main();

    g_free(sa);

    return 0;
}
