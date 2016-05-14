int main(int argc, char **argv)
{
    ClutterScript *ui;
    Partie *partie;
    GObject *stage;
    GError *err = NULL;
    ClutterInitError cie;
    cie = clutter_init(NULL, NULL);


    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "src/gui/menu.json", &err);
    if (err != NULL)
    {
        printf("%s\n", err->message);
        err = NULL;
    }

    stage = clutter_script_get_object(ui, "stage");

    // TODO : Relier les signaux

    partie = create_partie();// partie.c
    init_partie(partie, ui, 30, 30);// partie.c

    clutter_actor_show(CLUTTER_ACTOR(stage));
    clutter_main();
}
