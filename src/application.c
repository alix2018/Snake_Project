/**
 * @file      application.c
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Toutes les fonctions permettant d'afficher la fenpetre et le menu puis lancer le jeu
 * @details   ---
 */

#include <string.h>
#include "application.h"
#include "widgets/utils.h"
#include "widgets/alpha-field.h"
#include "widgets/alpha-check-box.h"


gboolean quitter_clicked_cb(ClutterClickAction *action,
                            ClutterActor *actor,
                            gpointer data)
{
    clutter_main_quit();
    return CLUTTER_EVENT_STOP;
}


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
    AlphaField *champPseudo = ALPHA_FIELD(clutter_script_get_object(app->ui, "pseudo"));

    clutter_actor_remove_child(app->stage, app->menu_partie);

    app->partie = create_partie();
    partie_set_config(app->partie, app->config);
    init_partie(app->partie, app->ui); // TODO récupérer depuis app
    init_pseudo(app->partie, alpha_field_get_text(champPseudo));
    return CLUTTER_EVENT_STOP;
}

gboolean bouton_partie_avancee_clicked_cb(ClutterClickAction *action,
                                          ClutterActor *actor,
                                          gpointer data)
{
    Application *app = data;

    clutter_actor_remove_child(app->stage, app->menu_partie);
    clutter_actor_add_child(app->stage, app->menu_avance);

    return CLUTTER_EVENT_STOP;
}

gboolean bouton_avance_jouer_clicked_cb(ClutterClickAction *action,
                                          ClutterActor *actor,
                                          gpointer data)
{
    Application *app = data;

    clutter_actor_remove_child(app->stage, app->menu_avance);
    Partie * partie;
    gboolean checked_invasion;
    gboolean checked_bonus;

    AlphaField *champPseudo = ALPHA_FIELD(clutter_script_get_object(app->ui, "pseudo"));
    AlphaField *champHplateau = ALPHA_FIELD(clutter_script_get_object(app->ui, "hauteur"));
    AlphaField *champLplateau = ALPHA_FIELD(clutter_script_get_object(app->ui, "largeur"));
    AlphaField *champTailleSnake = ALPHA_FIELD(clutter_script_get_object(app->ui, "taille_snake"));
    AlphaField *champNbSchlanga = ALPHA_FIELD(clutter_script_get_object(app->ui, "nb_schlanga"));
    AlphaField *champNbBonus = ALPHA_FIELD(clutter_script_get_object(app->ui, "nb_bonus"));
    AlphaCheckBox *boxInvasion = ALPHA_CHECK_BOX(clutter_script_get_object(app->ui, "mode_invasion"));
    AlphaCheckBox *boxBonus = ALPHA_CHECK_BOX(clutter_script_get_object(app->ui, "bonus_simples"));

    g_object_get(boxInvasion, "checked", &checked_invasion, NULL);
    g_object_get(boxBonus, "checked", &checked_bonus, NULL);


    app->partie = create_partie();
    if ( !strcmp(alpha_field_get_text(champNbSchlanga),"") == 0) {
        app->config->nb_snakes=atoi(alpha_field_get_text(champNbSchlanga));
    }
    if ( !strcmp(alpha_field_get_text(champNbBonus),"") == 0) {
        app->config->nb_bonus=atoi(alpha_field_get_text(champNbBonus));
    }
    if ( !strcmp(alpha_field_get_text(champHplateau),"") == 0) {
        app->config->height=atoi(alpha_field_get_text(champHplateau));
    }
    if ( !strcmp(alpha_field_get_text(champLplateau),"") == 0) {
        app->config->width=atoi(alpha_field_get_text(champLplateau));
    }
    if ( !strcmp(alpha_field_get_text(champTailleSnake),"") == 0) {
        app->config->taille_snake=atoi(alpha_field_get_text(champTailleSnake));
    }
    if ( !strcmp(alpha_field_get_text(champTailleSnake),"") == 0) {
        app->config->taille_bot=atoi(alpha_field_get_text(champTailleSnake));
    }
    if ( checked_invasion ) {
        app->config->type_partie=2;
    }
    if ( !checked_bonus ) {
        app->config->advanced_bonus=1;
    }
    config_grid_from_screen_and_window(app->config);
    partie_set_config(app->partie, app->config);
    init_partie(app->partie, app->ui); // TODO récupérer depuis app
    init_pseudo(app->partie, alpha_field_get_text(champPseudo));

    return CLUTTER_EVENT_STOP;
}

gboolean bouton_rejouer_clicked_cb(ClutterClickAction *action,
                                   ClutterActor *actor,
                                   gpointer data)
{
    Application *app=data;
    ClutterActor *fin_partie = CLUTTER_ACTOR(clutter_script_get_object(app->ui, "fin_partie"));
    
    free_partie(app->partie);
    app->partie = NULL;
    clutter_actor_remove_child(app->stage, fin_partie);
    clutter_actor_add_child(app->stage, app->menu_general);
    
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

/**
 * @brief        Fonction qui génère la fenetre principale c'est-à-dire le menu.
 *
 * @param[in]    app       L'application à initialiser.
 *
 * @return       L'application initialisée.
 */
void init_application(Application *app)
{
    GError *err = NULL;
    ClutterImage *bg_menus;
    ClutterImage *logo_menus;


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
        "image_alpha_snake", &app->image_alpha_snake,
        "image_alpha_snake2", &app->image_alpha_snake2,
        "image_alpha_snake3", &app->image_alpha_snake3,
        "menu_avance", &app->menu_avance,
        "stage", &app->stage,
        NULL
    );


    bg_menus = create_clutter_image("data/fond_menu.png");
    clutter_actor_set_content(app->menu_general, CLUTTER_CONTENT(bg_menus));
    clutter_actor_set_content(app->menu_partie, CLUTTER_CONTENT(bg_menus));
    clutter_actor_set_content(app->menu_avance, CLUTTER_CONTENT(bg_menus));

    logo_menus = create_clutter_image("logo.png");
    clutter_actor_set_content(app->image_alpha_snake, CLUTTER_CONTENT(logo_menus));
    clutter_actor_set_content(app->image_alpha_snake2, CLUTTER_CONTENT(logo_menus));
    clutter_actor_set_content(app->image_alpha_snake3, CLUTTER_CONTENT(logo_menus));


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