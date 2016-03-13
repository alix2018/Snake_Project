#include <clutter/clutter.h>
#include "affichage.h"
#include "list.h"
#include "struc.h"

#define GRID_SIZE 10

struct _snake_actor
{
    List *actors;           /// La liste des ClutterActor du snake.
    ClutterActor *parent;   /// Le ClutterActor qui contient les acteurs du snake.
    Snake *snake;           /// Le Snake affiché par le SnakeActor.
    int cur_size;           /// La taille du snake _affiché_ (peut être différente de la taille du Snake)
};


/**
 * @brief   Fonction callback appelée lorsqu'une touche du clavier est appuyée.
 *
 * @param[in]    actor  L'acteur qui a émis le signal.
 * @param[in]    event  Permet de récupérer des données sur la touche appuyée.
 * @param[in]    data   Un pointeur void* vers le snake contrôlé par le joueur.
 * 
 * @return ce prototype est défini par Clutter, cf. la documentation de Clutter.
 */
gboolean zone_snake_key_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer data)
{
    Snake *s = data;
    guint key_symbol = clutter_event_get_key_symbol(event);

    switch (key_symbol)
    {
        case CLUTTER_KEY_Up:
            if (snake_direction(s) != BAS)
                snake_set_direction(s, HAUT);
            break;
        case CLUTTER_KEY_Down:
            if (snake_direction(s) != HAUT)
                snake_set_direction(s, BAS);
            break;
        case CLUTTER_KEY_Left:
            if (snake_direction(s) != DROITE)
                snake_set_direction(s, GAUCHE);
            break;
        case CLUTTER_KEY_Right:
            if (snake_direction(s) != GAUCHE)
                snake_set_direction(s, DROITE);
        default:
            break;
    }

    return TRUE;
}


/**
 * @brief Fonction callback appelée à intervalles réguliers.
 * 
 * @param[in]    data   Le SnakeActor du snake.
 */
gboolean timeout_tick_cb(gpointer data)
{
    SnakeActor *sa = data;
    gfloat x, y;

    snake_forward(sa->snake);
    snake_actor_update(sa);

    return G_SOURCE_CONTINUE;
}

/**
 * @brief   Fonction callback appelée lorsque la fenêtre est fermée.
 * 
 * @param[in]    data   Le SnakeActor du snake.
 * 
 * Ce prototype est défini par Clutter, cf. la documentation de Clutter.
 */
void stage_destroy_cb(ClutterActor *actor, gpointer data)
{
    SnakeActor *sa = data;

    free_snake_actor(sa);
    clutter_main_quit();
}


/**
 * @brief   Initialise un SnakeActor.
 * 
 * @param[in]    parent Le ClutterActor qui contiendra le snake.
 * @param[in]    s      Le snake qui sera affiché par le SnakeActor.
 *
 * @return  Le SnakeActor initialisé.
 */
SnakeActor *create_snake_actor(ClutterActor *parent, Snake *s)
{
    SnakeActor *res;

    res = malloc(sizeof(struct _snake_actor));
    res->actors = create_list();
    res->parent = parent;
    res->cur_size = 0;
    res->snake = s;

    return res;
}


/**
 * Fonction appliquée à chaque élément de la liste des acteurs de SnakeActor
 * lors de la suppression de cette liste.
 */
static void free_clutter_actor_fn(void * elt)
{
    ClutterActor *actor = elt;

    clutter_actor_destroy(actor);
}


/**
 * Libère la mémoire consommée par un SnakeActor.
 *
 * @param[in]    sa     Le SnakeActor à libérer.
 */
void free_snake_actor(SnakeActor *sa)
{
    free_list_fn(sa->actors, free_clutter_actor_fn);
    free(sa);
}


/**
 * Met à jour la longueur et la position d'un SnakeActor.
 *
 * @param[in]    sa     Le SnakeActor à mettre à jour.
 */
void snake_actor_update(SnakeActor *sa)
{
    int delta;
    ClutterActor *actor;
    Node node_sa;
    Node node_s;
    Coord *c;

    delta = snake_longueur(sa->snake) - sa->cur_size;
    if (delta > 0)
    {
        for (; delta > 0; delta--)
        {
            actor = clutter_actor_new();
            g_object_ref(actor);
            clutter_actor_set_size(actor, GRID_SIZE, GRID_SIZE);
            clutter_actor_set_background_color(actor, CLUTTER_COLOR_Blue);
            clutter_actor_set_easing_duration(actor, 300);
            clutter_actor_add_child(sa->parent, actor);


            list_add_last(sa->actors, actor);
        }

        sa->cur_size = snake_longueur(sa->snake);
    }
    else if (delta < 0)
    {
        for (; delta < 0; delta++)
        {
            actor = list_pop_last(sa->actors);
            clutter_actor_destroy(actor);
        }

        sa->cur_size = snake_longueur(sa->snake);
    }

    node_s = snake_premier(sa->snake);
    for (node_sa = list_first_node(sa->actors);
         node_sa != NULL;
         node_sa = node_next(node_sa), node_s = node_next(node_s))
    {
        c = node_elt(node_s);
        actor = node_elt(node_sa);
        clutter_actor_set_position(
            actor,
            c->x * GRID_SIZE,
            c->y * GRID_SIZE
        );
    }
}


/**
 * @brief   Crée la fenêtre du snake.
 *
 * @param[in]    ui         Le fichier ui contenant la déclaration de
 *                          la fenêtre du Snake.
 * @param[in]    width      La largeur de la fenêtre en nombre de cases de la
 *                          grille.
 * @param[in]    height     La hauteur de la fenêtre.
 * @param[in]    direction  La direction de départ du snake.
 * @param[in]    size       La longueur du snake.
 * @param[in]    pos        La position de départ du snake.
 */
void init_view(ClutterScript *ui, int width, int height, Direction direction,
               int size, Coord pos)
{
    ClutterActor *zone_snake;
    ClutterActor *stage;
    Snake *snk;
    SnakeActor *sa;

    stage = CLUTTER_ACTOR(clutter_script_get_object(ui, "stage"));
    clutter_actor_set_size(stage, width * GRID_SIZE, height * GRID_SIZE);

    snk = create_snake(
        size,
        pos,
        direction
    );

    zone_snake = CLUTTER_ACTOR(clutter_script_get_object(ui, "zone_snake"));
    clutter_stage_set_key_focus(CLUTTER_STAGE(stage), zone_snake);
    sa = create_snake_actor(zone_snake, snk);
    snake_actor_update(sa);

    g_signal_connect(zone_snake, "key-press-event", G_CALLBACK(zone_snake_key_press_cb), snk);
    g_signal_connect(stage, "destroy", G_CALLBACK(stage_destroy_cb), sa);
    g_timeout_add(200, timeout_tick_cb, sa);

    clutter_actor_show(stage);

    clutter_main();

    free_snake(snk);
}
