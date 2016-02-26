
#include <clutter/clutter.h>
#include "struc.h"


int dx,dy,direction;
    float vitesse = 20;

gboolean paint( gpointer data) 
{

    ClutterActor *rectangle = data;

   clutter_actor_save_easing_state(rectangle);
    float x = clutter_actor_get_x(rectangle);
    float y = clutter_actor_get_y(rectangle);
    //printf("pos %f, %f \n", x,y);
    switch (direction)  
    {
     
    case 90:  // haut 
        dx = 0;
        dy = -vitesse;

    break;

    case 180: //bas 
        dx = 0;
        dy = vitesse;
    break;

    case 270: // gauche  
        dx = -vitesse;
        dy = 0;

    break;

    case 0: //droite
        dx = vitesse;
        dy = 0;
    break;
    default:;
    }
    clutter_actor_set_position(rectangle,  x+dx,  y+dy);

    clutter_actor_restore_easing_state(rectangle);

}
gboolean clic_souris_cb(ClutterActor *actor, ClutterEvent *event, gpointer data) 
{
        // cast automatique pour récuperer le rectangle.
    ClutterActor *rectangle = data;




  printf("button %i\n",clutter_event_get_key_symbol(event));

  /*
   * Les boutons haut sont : 65362 / 122 /90
   * Les boutons bas sont : 65364 / 115 / 83
   * Les boutons gauche sont : 65361 / 113 /81
   * Les boutons droite sont : 65363 / 100 / 68
   */
   clutter_actor_save_easing_state(rectangle);


    //clutter_actor_set_easing_duration(rectangle, 400);
    /*
     * Désormais, tous les changements de couleur, de taille, de position,
     * d'opacité, ... se feront en 400 ms.
     */
   // clutter_actor_set_easing_mode(rectangle, CLUTTER_EASE_IN_QUAD);
    float x = clutter_actor_get_x(rectangle);
    float y = clutter_actor_get_y(rectangle);
    printf("pos %f, %f \n", x,y);
    float dx,dy;

    int keypressed = clutter_event_get_key_symbol(event);
    switch (keypressed) 
    {
        case 65362:;case 90:;
    case 122: printf("haut\n"); 
        direction = 90;
    break;
        case 65364:;case 83:;
    case 115: printf("bas\n"); 
        direction = 180;
    break;
        case 65361:;case 81:;
    case 113: printf("gauche\n"); 
        direction = 270;

    break;
        case 65363:;case 68:;
    case 100: printf("droite\n"); 
        direction = 0;
    break;
    default: printf("not\n");
    }
   // clutter_actor_set_position(rectangle,  x+dx,  y+dy);

    //clutter_actor_restore_easing_state(rectangle);

}

/*ClutterActor** generate_form_snake(Snake *snake)
{

    ClutterActor ** res = (ClutterActor**)malloc( 5*sizeof(ClutterActor * )); 
    printf("Snake longueur : %i \n",snake_longueur(snake));


    return res;
}
*/

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
    coord_from_xy(3,2);
//    Snake *snake_struc = create_snake(3,coord_from_xy(2,3),GAUCHE);
 //   ClutterActor ** snake_forme = generate_form_snake(snake_struc);
    // Pas de commentaire.
    clutter_init(&argc, &argv);
    /*
     * Clutter est orientée objet. En Java, cette fonction serait traduite par
     * stage = new ClutterStage();
     */
    stage = clutter_stage_new();
    // En Java, cette ligne donnerait stage.set_title("Essai Clutter");
    clutter_stage_set_title(CLUTTER_STAGE(stage), "The title xD ");
    clutter_actor_set_size(stage, 800, 800);
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
    clutter_actor_set_size(rectangle, 20, 20);
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
     * ClutterCli  


     }ckAction   : il faut l'associer à un acteur (voir la doc) et
     * connecter le signal "clicked".
     */
    g_signal_connect(stage, "key-press-event", G_CALLBACK(clic_souris_cb), rectangle);

    g_timeout_add(30,G_CALLBACK(paint),rectangle); 
    clutter_actor_show(stage);

    /*
     * Pour définir les acteurs, on peut aussi utiliser le ClutterScript
     * (voir la doc) qui permet de décrire les acteurs, leurs propriétés et
     * leurs signaux en JSON.
     */

    clutter_main();

    return 0;
}
