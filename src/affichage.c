/**
 * @file      affichage.c
 * @author    alpha-snake
 * @version   1
 * @date      11/05/2016
 * @brief     Toutes les fonctions permettant d'afficher la fenêtre, les Snakes/Schlangà et les bonus.
 * @details   ---
 */

#include <clutter/clutter.h>
#include "affichage.h"
#include "partie.h"
#include "list.h"
#include "struc.h"
#include "ia.h"
#include <gdk/gdk.h>

/**
 * @brief      La structure de l'acteur Snake
 *
 * @use SnakeActor
 * @param[in]  actor        La liste des ClutterActor du snake.
 * @param[in]  parent       Le ClutterActor qui contient les acteurs du snake.
 * @param[in]  snake        Le Snake affiché par le SnakeActor.
 * @param[in]  color        La couleur.
 * @param[in]  images       Images du snake.
 * @param[in]  cur_size     La taille du snake _affiché_ (peut être différente de la taille du Snake).
 */
struct _snake_actor
{
    List *actors;
    ClutterActor *parent;
    Snake *snake;
    ClutterColor *color;
    SnakeImage *images;
    int cur_size;

};


/**
 * @brief      La structure de l'acteur image pour un snake
 *
 * @use SnakeImage
 * @param[in]  tete        Image de la tête du Snake.
 * @param[in]  queue       Image de la queue du Snake.
 * @param[in]  corps       Image du corps du Snake.
 * @param[in]  turnlight   Image du corps du Snake quand il tourne (angle plus clair).
 * @param[in]  turndark    Image du corps du Snake quand il tourne (angle plus sombre).
 */
struct _snake_image
{
    ClutterContent *tete;
    ClutterContent *queue;
    ClutterContent *corps;
    ClutterContent *turnlight;
    ClutterContent *turndark;
};



/**
 * @brief      La structure de l'acteur affichage
 *
 * @use Affichage
 * @param[in]  snake_actors     Liste des acteurs du snake.
 * @param[in]  bonus            Liste des acteurs du bonus.
 * @param[in]  images           Images du snake.
 * @param[in]  ui               Le fichier ui contenant la déclaration de la fenêtre du Snake.
 */
struct _Affichage
{
    List *snake_actors;
    List *bonus;
    SnakeImage *images;
    ClutterScript *ui;
};


/**
 * @brief   Crée un nouvel affichage. Il faut appeler init_affichage pour
 *          l'initialiser.
 *
 * @return L'affichage qui a été alloué.
 */
Affichage *create_affichage()
{
    Affichage *res;

    res = malloc(sizeof(Affichage));
    res->snake_actors = create_list();
    res->bonus = create_list();

    return res;
}


/**
 * @brief   Fonction qui sert à récupérer les bonus d'un affichage
 *
 * @param[in]    affichage  L'affichage.
 *
 * @return le bonus de l'affichage.
 */
List *affichage_list_bonus_actor(Affichage *a)
{
    return a->bonus;
}


/**
 * @brief   Fonction auxiliaire de free_affichage.
 */
static void free_affichage_snake_actors(void *elt)
{
    SnakeActor *sa = elt;

    free_snake_actor(sa);
}

static void free_affichage_bonus_actor(void *elt)
{
    BonusActor *actor = elt;

    free_bonus_actor(actor);
}

/**
 * @brief   Libère la mémoire consommée par un affichage.
 *
 * @param[in]    affichage  L'affichage à supprimer.
 */
void free_affichage(Affichage *affichage)
{
    g_object_unref(affichage->images->tete);
    g_object_unref(affichage->images->corps);
    g_object_unref(affichage->images->queue);
    g_object_unref(affichage->images->turndark);
    g_object_unref(affichage->images->turnlight);
    free(affichage->images);
    free_list_fn(affichage->snake_actors, free_affichage_snake_actors);
    free_list_fn(affichage->bonus, free_affichage_bonus_actor);
    free(affichage);
}


/**
 * @brief   Retourne le ClutterScript associé à un affichage.
 *
 * @param[in]    affichage  Un affichage.
 *
 * @return Le ClutterScript de Affichage.
 */
ClutterScript *affichage_ui(Affichage *affichage)
{
    return affichage->ui;
}

/**
 * @brief   Fonction callback appelée lorsqu'une touche du clavier est appuyée.
 *
 * @param[in]    actor  L'acteur qui a émis le signal.
 * @param[in]    event  Permet de récupérer des données sur la touche appuyée.
 * @param[in]    data   Un pointeur void* vers le snake contrôlé par le joueur.
 *
 * @return  Ce prototype est défini par Clutter, cf. la documentation de Clutter.
 */
gboolean zone_snake_key_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer data)
{
    Snake *s = data;
    guint key_symbol = clutter_event_get_key_symbol(event);

    switch (key_symbol)
    {
        case CLUTTER_KEY_Up:
            snake_set_direction(s, HAUT);
            break;
        case CLUTTER_KEY_Down:
            snake_set_direction(s, BAS);
            break;
        case CLUTTER_KEY_Left:
            snake_set_direction(s, GAUCHE);
            break;
        case CLUTTER_KEY_Right:
            snake_set_direction(s, DROITE);
        default:
            break;
    }

    return CLUTTER_EVENT_STOP;
}


/**
 * @brief      Renvoie true si le snake est contre un mur et veux avancer dans
 *             le mur. Appelée dans la fonction timeout_tich_cb
 *
 * @param[in]  p    La partie
 * @param[in]  sa   Un snake
 *
 * @return     Renvoie 1 si le snake est contre un mur et veux avancer dans le mur, 0 sinon
 */
int snake_border_map(Partie *p,SnakeActor *sa)
{
    int res = 0, l_w, l_h;
    float w, h;
    Config * config = partie_config(p);
    Snake *s = sa->snake;

    clutter_actor_get_size(sa->parent, &w, &h);
    l_w = (int) w/config->grid_size;
    l_h = (int) h/config->grid_size;

    if(snake_direction(s) == HAUT && snake_pos(s).y == 0)
    {
        res = 1;
    }
    if(snake_direction(s) == GAUCHE && snake_pos(s).x == 0)
    {
        res = 1;
    }
    if(snake_direction(s) == BAS && snake_pos(s).y == l_h-1)
    {
        res = 1;
    }
    if(snake_direction(s) == DROITE && snake_pos(s).x == l_w-1)
    {
        res = 1;
    }

    return res;
}

/**
 * @brief      Renvoie true si le snake va se mordre la queue. Appelée dans la fonction timeout_tich_cb.
 *
 * @param[in]  sa      Un Schlangà
 * @param[in]  sa_ia   Un snake
 *
 * @return     Renvoie 0 si le snake essaie de se mordre la queue, 1 sinon
*/
int snake_border_snake(SnakeActor *sa,SnakeActor *sa_ia)
{
    int res = 0;
    Snake *s = sa->snake;
    Node ls = snake_premier(s);
    Coord c_tete = snake_pos(s);

    if(snake_direction(s) == HAUT)
    {
        c_tete.y -= 1;
        while( ls != NULL )
        {
            if(coord_egales(c_tete,*((Coord *) node_elt(ls))))
            {
                res = 1;
            }
            ls = node_next(ls);
        }
    }

    if(snake_direction(s) == BAS)
    {
        c_tete.y += 1;
        while( ls != NULL )
        {
            if(coord_egales(c_tete,*((Coord *) node_elt(ls))))
            {
                res = 1;
            }
            ls = node_next(ls);
        }
    }

    if(snake_direction(s) == GAUCHE)
    {
        c_tete.x -= 1;
        while( ls != NULL )
        {
            if(coord_egales(c_tete,*((Coord *) node_elt(ls))))
            {
                res = 1;
            }
            ls = node_next(ls);
        }
    }

    if(snake_direction(s) == DROITE)
    {
        c_tete.x += 1;
        while( ls != NULL )
        {
            if(coord_egales(c_tete,*((Coord *) node_elt(ls))))
            {
                res = 1;
            }
            ls = node_next(ls);
        }
    }

    //ia sur lui même

    Snake *s_ia = sa_ia->snake;
    Node ls_ia = snake_premier(s_ia);
    Coord c_tete_ia = snake_pos(s_ia);

    if(snake_direction(s_ia) == HAUT)
    {
        c_tete_ia.y -= 1;
        while( ls_ia != NULL )
        {
            if(coord_egales(c_tete_ia,*((Coord *) node_elt(ls_ia))))
            {
                res = 1;
            }
            ls_ia = node_next(ls_ia);
        }
    }

    if(snake_direction(s_ia) == BAS)
    {
        c_tete_ia.y += 1;
        while( ls_ia != NULL )
        {
            if(coord_egales(c_tete_ia,*((Coord *) node_elt(ls_ia))))
            {
                res = 1;
            }
            ls_ia = node_next(ls_ia);
        }
    }

    if(snake_direction(s_ia) == GAUCHE)
    {
        c_tete_ia.x -= 1;
        while( ls_ia != NULL )
        {
            if(coord_egales(c_tete_ia,*((Coord *) node_elt(ls_ia))))
            {
                res = 1;
            }
            ls_ia = node_next(ls_ia);
        }
    }

    if(snake_direction(s_ia) == DROITE)
    {
        c_tete_ia.x += 1;
        while( ls_ia != NULL )
        {
            if(coord_egales(c_tete_ia,*((Coord *) node_elt(ls_ia))))
            {
                res = 1;
            }
            ls_ia = node_next(ls_ia);
        }
    }
    //fin ia
    return res;
}

/*
 * @brief        Indique si le snake mange la bonus
 *
 * @param[in]    s   Un snake
 * @param[in]    b   Un bonus
 *
 * @return       1 si et seulement si la tête du snake est au même endroit que la bonus
 */
int snake_eat(Snake *s, Bonus *b)
{
    if(coord_egales(snake_pos(s), bonus_coord(b)))
    {
        return 1; // O_o
    }
    else
    {
        return 0;
    }
}


/**
 * @brief   Initialise un SnakeActor.
 *
 * @param[in]    parent     Le ClutterActor qui contiendra le snake.
 * @param[in]    s          Le snake qui sera affiché par le SnakeActor.
 * @param[in]    color      Couleur du snake.
 * @param[in]    imgs       Images du snake.
 *
 * @return  Le SnakeActor initialisé.
 */
SnakeActor *create_snake_actor(ClutterActor *parent, Snake *s, ClutterColor *color, SnakeImage *imgs)
{
    SnakeActor *res;

    res = malloc(sizeof(struct _snake_actor));
    res->actors = create_list();
    res->parent = parent;
    res->cur_size = 0;
    res->color = color;
    res->snake = s;
    res->images = imgs;

    return res;
}


/**
 * @brief Fonction appliquée à chaque élément de la liste des acteurs de SnakeActor lors de la suppression de cette liste.
 *
 * @param[in]   elt     Le ClutterActor à suprimer.
 */
static void free_clutter_actor_fn(void * elt)
{
    ClutterActor *actor = elt;

    clutter_actor_destroy(actor);
}


/**
 * @brief Libère la mémoire consommée par un SnakeActor.
 *
 * @param[in]    sa     Le SnakeActor à libérer.
 */
void free_snake_actor(SnakeActor *sa)
{
    free_list_fn(sa->actors, free_clutter_actor_fn);
    clutter_color_free(sa->color);
    free(sa);
}

void affichage_remove_bonus_actor(Affichage *a, Bonus *b)
{
    Node n=NULL;
    for(n=list_first_node(affichage_list_bonus_actor(a)); bonus_actor_bonus(node_elt(n))!=b; n=node_next(n)){}
    //clutter_actor_destroy(node_elt(n));
    free_bonus_actor(node_elt(n));
    list_delete_node(affichage_list_bonus_actor(a), n);

}

/**
 * @brief   Met à jour la longueur et la position d'un SnakeActor.
 *
 * @param[in]    p      La partie.
 * @param[in]    sa     Le SnakeActor à mettre à jour.
 */
void snake_actor_update(Partie *p, SnakeActor *sa)
{
    int delta;
    ClutterActor *actor;
    Node node_sa;
    Node node_s;
    Coord *c;
    Config * config = partie_config(p);

    delta = snake_longueur(sa->snake) - sa->cur_size;

    if (delta > 0)
    {
        for (; delta > 0; delta--)
        {
            actor = clutter_actor_new();
            g_object_ref(actor);
            clutter_actor_set_size(actor, config->grid_size, config->grid_size);
           // clutter_actor_set_background_color(actor,sa->color );

            clutter_actor_add_effect (actor, clutter_colorize_effect_new (sa->color));

            clutter_actor_set_easing_duration(actor, 0);
            //clutter_actor_set_easing_mode(actor,CLUTTER_EASE_IN_QUART);
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

    for (node_sa = list_first_node(sa->actors), node_s = snake_premier(sa->snake);
         node_sa != NULL;
         node_sa = node_next(node_sa), node_s = node_next(node_s))
    {
        c = node_elt(node_s);
        actor = node_elt(node_sa);
        clutter_actor_set_position(
            actor,
            c->x * config->grid_size,
            c->y * config->grid_size
        );

        // TODO changer les images pendant le deplacement
        /** VERSION OPTIMALE On modifie que si il le faut
        if(node_sa == list_first_node(sa->actors) && clutter_actor_get_content(actor) == NULL)
        {
            clutter_actor_set_content(actor,sa->images->tete);
        }
        else if(node_sa == list_last_node(sa->actors) && clutter_actor_get_content(actor) == NULL)
        {
            if(clutter_actor_get_content(node_elt(node_prev(node_sa))) == sa->images->queue)
            {
                clutter_actor_set_content(node_elt(node_prev(node_sa)),sa->images->corps);
            }

            clutter_actor_set_content(actor,sa->images->queue);




        }else if(clutter_actor_get_content(actor) == NULL)
        {
            clutter_actor_set_content(actor,sa->images->corps);
        } */
        Coord *cnext;
        Coord *cprev;
        if(node_sa == list_first_node(sa->actors) )
        {
            cnext = node_elt(node_next(node_s));
            clutter_actor_set_content(actor,sa->images->tete);
            clutter_actor_set_pivot_point (actor,0.5, 0.5 );
            clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,0);

            if(cnext->x == c->x && c->y < cnext->y)
            {
                // TODO tête droite (pas de rotation)
            }
            else if (cnext->x == c->x && c->y > cnext->y)
            {
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,180);
            }
            else if (cnext->y == c->y && c->x > cnext->x )
            {
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,90);
            }
            else if(cnext->y == c->y && c->x < cnext->x)
            {
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,-90);
            }
        }
        else if(node_sa == list_last_node(sa->actors))
        {
            cprev = node_elt(node_prev(node_s));
            clutter_actor_set_content(actor,sa->images->queue);
            clutter_actor_set_pivot_point (actor,0.5, 0.5 );
            clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,0);
            if(cprev->x == c->x && c->y < cprev->y)
            {

                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,180);
            }
            else if (cprev->x == c->x && c->y > cprev->y)
            {


               // clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,-180);
            }
            else if (cprev->y == c->y && c->x > cprev->x )
            {

                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,-90);
            }
            else if(cprev->y == c->y && c->x < cprev->x)
            {

                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,90);
            }
        }
        else
        {

            cnext = node_elt(node_next(node_s)); // vers la queue
            cprev = node_elt(node_prev(node_s)); // vers la tête
            clutter_actor_set_content(actor,sa->images->corps);

            clutter_actor_set_pivot_point (actor,0.5, 0.5 );
            clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,0);
            if(c->x == cnext->x && c->x == cprev->x && cnext->y > cprev->y)
            {
                // nothing

            }
            else if (c->x == cnext->x && c->x == cprev->x && cnext->y < cprev->y)
            {
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,180);
            }
            else if (c->y == cnext->y && c->y == cprev->y && cprev->x < cnext->x)
            {
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,-90);
            }
            else if(c->y == cnext->y && c->y == cprev->y && cprev->x > cnext->x)
            {
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,90);
            }
            else if(c->x == cnext->x && c->y < cnext->y && c->y == cprev->y && c->x < cprev->x) // Les turnlight et turndark
            {
                clutter_actor_set_content(actor,sa->images->turnlight);
            }
            else if(c->x == cnext->x && c->y < cnext->y && c->y == cprev->y && c->x > cprev->x)
            {
                clutter_actor_set_content(actor, sa->images->turndark);
            }
            else if(c->x == cnext->x && c->y > cnext->y && c->y == cprev->y && c->x > cprev->x)
            {
                clutter_actor_set_content(actor,sa->images->turnlight);
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,180);
            }
            else if(c->x == cnext->x && c->y > cnext->y && c->y == cprev->y && c->x < cprev->x)
            {
                clutter_actor_set_content(actor,sa->images->turndark);
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,180);
            }
            else if(c->x == cprev->x && c->y > cprev->y && c->y == cnext->y && c->x < cnext->x)
            {
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,-90);
                clutter_actor_set_content(actor,sa->images->turnlight);
            }
            else if(c->x == cprev->x && c->y > cprev->y && c->y == cnext->y && c->x > cnext->x)
            {
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,90);
                clutter_actor_set_content(actor,sa->images->turndark);
            }
            else if(c->x == cprev->x && c->y < cprev->y && c->y == cnext->y && c->x > cnext->x)
            {
                clutter_actor_set_content(actor,sa->images->turnlight);
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,90);
            }
            else if(c->x == cprev->x && c->y < cprev->y && c->y == cnext->y && c->x < cnext->x)
            {
                clutter_actor_set_content(actor,sa->images->turndark);
                clutter_actor_set_rotation_angle(actor,CLUTTER_Z_AXIS,-90);
            }
        }

    }
}

/**
 * @brief   Charge une image
 *
 * @param[in] filename  Nom de l'image à importer.
 *
 * @return  Un ClutterContent avec l'image chargée.
 */
ClutterContent *generate_image(char *filename)
{

    ClutterContent *image = clutter_image_new();

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

    clutter_image_set_data (CLUTTER_IMAGE(image),
                            gdk_pixbuf_get_pixels(pixbuf),
                            gdk_pixbuf_get_has_alpha(pixbuf)
                              ? COGL_PIXEL_FORMAT_RGBA_8888
                              : COGL_PIXEL_FORMAT_RGB_888,
                            gdk_pixbuf_get_width (pixbuf),
                            gdk_pixbuf_get_height (pixbuf),
                            gdk_pixbuf_get_rowstride (pixbuf),
                            NULL);

    g_object_unref(pixbuf);
    return image;
}

/**
 * @brief   Crée un élément de type SnakeImage
 *
 * @return  Le SnakeImage initialisé.
 */
SnakeImage *snake_generate_image()
{
    SnakeImage *res;
    res = malloc(sizeof(struct _snake_image));
    res->tete = generate_image(TETE_IMAGE_SRC);
    res->queue = generate_image(QUEUE_IMAGE_SRC);
    res->corps = generate_image(CORPS_IMAGE_SRC);
    res->turnlight = generate_image(TURNLIGHT_IMAGE_SRC);
    res->turndark = generate_image(TURNDARK_IMAGE_SRC);

    return res;
}


/**
 * @brief   Ajoute un Snake à un affichage.
 *
 * @param[in]    affichage  Un affichage.
 * @param[in]    snake      Le snake à ajouter.
 * @param[in]    color      La couleur du snake.
 */
void affichage_add_snake(Affichage *affichage, Snake *snake,
                         const ClutterColor *color)
{
    SnakeActor *sa;
    // change l'alpha :
    ClutterColor *rcolor= clutter_color_alloc();
    clutter_color_shade(color, 1.6, rcolor);
    sa  = create_snake_actor(
        CLUTTER_ACTOR(clutter_script_get_object(affichage->ui, "zone_snake")),
        snake,
        rcolor,
        affichage->images
    );
    list_add_last(affichage->snake_actors, sa);
}


/**
 * @brief   Ajoute un bonus à un affichage.
 *
 * @param[in]    affichage  Un affichage.
 * @param[in]    bonus      Le bonus à ajouter.
 * @param[in]    color      La couleur du bonus.
 * @param[in]    config     Les configurations.
 */
void affichage_add_bonus(Affichage *affichage, Bonus *bonus,
                         const ClutterColor *color,Config * config)
{
    BonusActor *ba;
    ba = create_bonus_actor(
        CLUTTER_ACTOR(clutter_script_get_object(affichage->ui, "zone_snake")),
        bonus,
        clutter_color_new(0, 255, 0, 255),
        config,
        BONUS_BLIND
    );
    list_add_last(affichage->bonus, ba);
}

/**
 * @brief   Met à jour un affichage.
 *
 * @param[in]    p          La partie.
 * @param[in]    affichage  L'affichage à mettre à jour.
 */
void affichage_update(Partie * p,Affichage *affichage)
{
    SnakeActor *sa;
    BonusActor *ba;
    Node cur;

    for (cur = list_first_node(affichage->snake_actors);
         cur != NULL;
         cur = node_next(cur))
    {
        sa = node_elt(cur);

        snake_actor_update(p,sa);
    }

    for (cur = list_first_node(affichage->bonus);
         cur != NULL;
         cur = node_next(cur))
    {
        ba = node_elt(cur);

        bonus_actor_update(ba,partie_config(p));
    }
}


/**
 * @brief   Crée la fenêtre du snake.
 *
 * @param[in]    affichage  L'affichage de la fenêtre.
 * @param[in]    ui         Le fichier ui contenant la déclaration de
 *                          la fenêtre du Snake.
 * @param[in]    partie     La partie affichée.
 * @param[in]    width      La largeur de la fenêtre.
 * @param[in]    height     La longueur de la fenêtre.

 */
void init_affichage(Affichage *affichage, ClutterScript *ui, Partie *p,
                    int width, int height)
{
    ClutterActor *zone_snake;
    ClutterActor *stage;
    Snake *snk, *snk_ia;
    SnakeActor *sa, *sa_ia;
    Bonus *bonus;
    BonusActor *ba;
    Config * config = partie_config(p);
    affichage->ui = ui;

    stage = CLUTTER_ACTOR(clutter_script_get_object(ui, "stage"));
    clutter_actor_set_size(stage, width * config->grid_size,
                           height * config->grid_size);

    zone_snake = CLUTTER_ACTOR(clutter_script_get_object(ui, "zone_snake"));
    clutter_stage_set_key_focus(CLUTTER_STAGE(stage), zone_snake);
    affichage->images = snake_generate_image();

    g_signal_connect(zone_snake, "key-press-event", G_CALLBACK(zone_snake_key_press_cb), partie_player(p)); // TODO génraliser

    // SET IMAGE BACKGROUND
    ClutterContent *image = generate_image(BACKGROUND_IMAGE_SRC);
    clutter_actor_set_content(zone_snake,image);
    g_object_unref(image);

}
