#include <clutter/clutter.h>

gboolean clic_souris_cb(ClutterActor *actor, ClutterEvent *event, gpointer data) {
    /*
     * Cette fonction est appelée lorsque le bouton de la souris est enfoncé
     * dans la fenêtre
     */

    // cast automatique pour récuperer le rectangle.
    ClutterActor *rectangle = data;
    ClutterPoint *pos = clutter_point_alloc();

    /*
     * Le paramètre event contient toutes les données relatives au clic de
     * souris.
     */
    clutter_event_get_position(event, pos);

    /*
     * Là on fait une animation (parce que les animations c'est cool, surtout
     * en 3 lignes de code *_* )
     */
    clutter_actor_save_easing_state(rectangle);

    /*
     * La ligne du dessus peut paraître mystérieuse. En fait, dans clutter,
     * tous les acteurs sont animés par défaut. La durée d'animation est
     * simplement fixée à 0 par défaut. On sauvegarde d'abord les paramètres
     * d'animation du rectangle pour pouvoir les rétablir une fois que
     * l'animation sera lancée.
     * On peut noter que, puisque seule la position du rectangle est modifiée
     * dans ce programme, on aurait simplement pu définir les paramètres
     * d'animation du rectangle dans la fonction main et s'épargner les appels
     * aux fonctions clutter_actor_save/restore_easing_state.
     */

    clutter_actor_set_easing_duration(rectangle, 400);
    /*
     * Désormais, tous les changements de couleur, de taille, de position,
     * d'opacité, ... se feront en 400 ms.
     */
    clutter_actor_set_easing_mode(rectangle, CLUTTER_EASE_IN_OUT_CUBIC);
    clutter_actor_set_position(rectangle, pos->x - 25, pos->y - 25);

    clutter_actor_restore_easing_state(rectangle);

    clutter_point_free(pos);

    /*
     * On retourne TRUE pour signifier que le signal a été bien traité.
     * Lorsqu'on retourne FALSE, le signal sera émis dans les acteurs situés
     * en dessous du rectangle s'ils existent.
     */
    return TRUE;
}

int main(int argc, char **argv) {
    /*
     * ClutterStage : c'est la fenêtre du programme, elle hérite de
     * ClutterActor donc on peut aussi lui appliquer toutes les fonctions
     * de ClutterActor.
     */
    ClutterActor *stage;
    /*
     * Le rectangle est un ClutterActor. ClutterActor est l'objet de base dans
     * clutter, c'est en gros un truc rectangulaire qui peut contenir d'autres
     * acteurs, on peut lui changer sa taille, sa forme, son opacité, sa couleur
     * ... le tout avec des animations. Les ClutterActor peuvent aussi émettre
     * des signaux (voir plus bas).
     */
    ClutterActor *rectangle;

    // Pas de commentaire.
    clutter_init(&argc, &argv);
    /*
     * Clutter est orientée objet. En Java, cette fonction serait traduite par
     * stage = new ClutterStage();
     */
    stage = clutter_stage_new();
    // En Java, cette ligne donnerait stage.set_title("Essai Clutter");
    clutter_stage_set_title(CLUTTER_STAGE(stage), "Essai Clutter");
    clutter_actor_set_size(stage, 400, 400);
    clutter_actor_set_background_color(stage, CLUTTER_COLOR_Black);
    /*
     * Cette ligne signifie que le ClutterActor stage émettra des signaux
     * comme les clics de souris
     */
    clutter_actor_set_reactive(stage, TRUE);
    /*
     * Cette ligne est importante à comprendre, c'est de cette manière que les
     * évènements à la souris et au clavier sont gérés par Clutter (et toutes
     * les bibliothèques de gnome, la fonction utilisée ici n'est pas une
     * fonction nécessitant un ClutterActor, ni même un objet de Clutter
     * c'est une fonction qui prend un objet de type GObject, l'ancêtre
     * commun de toutes les classes de toutes les bibliothèques de GNOME; il
     * faut voir GObject comme l'équivalent de la classe Java.Lang.Object en
     * Java)
     * On connecte ici le signal "destroy" (émis losque l'on ferme la fenêtre)
     * de l'objet stage à la fonction clutter_main_quit. La fonction
     * clutter_main_quit() sera appelée lorsque la fenêtre sera fermée
     * Le fonctionnement est similaire à celui de la fonction signal() qu'on
     * a vu en IFN.
     */
    g_signal_connect(stage, "destroy", G_CALLBACK(clutter_main_quit), NULL);

    rectangle = clutter_actor_new();
    clutter_actor_set_size(rectangle, 50, 50);
    clutter_actor_set_position(rectangle, 50, 50);
    clutter_actor_set_background_color(rectangle, CLUTTER_COLOR_Blue);
    /*
     * Tout ClutterActor peut avoir des enfants. Les coordonnées de l'enfant
     * sont relatives au parent
     */
    clutter_actor_add_child(stage, rectangle);
    /*
     * Gestion des clics de souris. La fonction
     * clic_souris_cb(stage, <l'évènement>, rectangle)
     * est appelée dès que le bouton de la souris est enfoncé.
     * Si on veut utiliser un acteur comme un bouton, on peut utiliser l'objet
     * ClutterClickAction : il faut l'associer à un acteur (voir la doc) et
     * connecter le signal "clicked".
     */
    g_signal_connect(stage, "button-press-event", G_CALLBACK(clic_souris_cb), rectangle);

    // Cette fonction affiche un ClutterActor (ici la fenêtre) et tous ses enfants.
    clutter_actor_show(stage);

    /*
     * Pour définir les acteurs, on peut aussi utiliser le ClutterScript
     * (voir la doc) qui permet de décrire les acteurs, leurs propriétés et
     * leurs signaux en JSON.
     */

    clutter_main();

    return 0;
}
