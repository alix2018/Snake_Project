/**
 * @file      bonus.c
 * @author    alpha-snake
 * @version   1
 * @date      11/05/2016
 * @brief     Toutes les fonctions permettant de créer et générer les bonus.
 * @details   ---
 */

#include "partie.h"
#include "bonus.h"
#include "affichage.h"


/**
 * @brief      La structure d'un bonus.
 *
 * @use Bonus
 * @param[in]   coord   Cordonnées du bonus.
 */
struct _bonus
{
    Coord coord;
    int rare;
    char * img;
    void (*callback_eat)(Partie *, Snake *, Bonus *);
    BonusEffect * be;
};

struct _bonus_effect
{
    void *callback_end;
};

struct _bonus_effect_apply
{
    int time_remaining;
    Bonus * bonus;
    Snake *snake;
};

BonusEffectApply * bonus_effect_apply_init(Snake *snake, Bonus *bs,int time)
{
    BonusEffectApply * new = malloc(sizeof(BonusEffectApply));
    new->bonus = bs;
    new->snake = snake;
    new->time_remaining = time;
}

BonusEffect * bonus_effect_init(Bonus *b,void * callback)
{
    BonusEffect *new = malloc(sizeof(BonusEffect));

    new->callback_end = callback;
    return new;
}
/**
 * @brief      La structure de l'acteur bonus
 *
 * @use BonusActor
 * @param[in]  parent            Le ClutterActor qui contient les acteurs du snake.
 * @param[in]  bonus_c_actor     Le ClutterActor qui contient les acteurs du bonus.
 * @param[in]  bonus             Le bonus
 * @param[in]  color             La couleur du bonus.
 */
struct _bonus_actor
{
    ClutterActor *parent;
    ClutterActor *bonus_c_actor;
    Bonus *bonus;
    ClutterColor *color;
};



void bonus_eat_basic(Partie *p, Snake *s, Bonus *b)
{
    snake_increase(s);
    bonus_update(b, map_width(partie_map(p)), map_height(partie_map(p)));

    //remove_advanced_bonus(p,b);
}

void bonus_eat_maxi5(Partie *p, Snake *s, Bonus *b)
{
    snake_increase(s);
    snake_increase(s);
    snake_increase(s);
    snake_increase(s);
    snake_increase(s);
    bonus_advanced_update(b, map_width(partie_map(p)), map_height(partie_map(p)));
}

void bonus_eat_speed(Partie *p, Snake *s, Bonus *b)
{
    snake_set_vitesse(s, 3);
    bonus_advanced_update(b, map_width(partie_map(p)), map_height(partie_map(p)));
}

void bonus_eat_speed_others(Partie *p, Snake *s, Bonus *b)
{
    TabSnakes *ts = partie_tab(p);
    int i;
    for (i = 0; i < ts->nb_snakes ; ++i)
    {
        if(ts->snakes[i] != s)
        {
            snake_set_vitesse(ts->snakes[i], 3);
        }
    }
    
    bonus_advanced_update(b, map_width(partie_map(p)), map_height(partie_map(p)));
}

void bonus_eat_slow(Partie *p, Snake *s, Bonus *b)
{
    snake_set_vitesse(s, 1);
    bonus_advanced_update(b, map_width(partie_map(p)), map_height(partie_map(p)));
}

void bonus_eat_slow_others(Partie *p, Snake *s, Bonus *b)
{
    TabSnakes *ts = partie_tab(p);
    int i;
    for (i = 0; i < ts->nb_snakes ; ++i)
    {
        if(ts->snakes[i] != s)
        {
            snake_set_vitesse(ts->snakes[i], 1);
        }
    }
    
    bonus_advanced_update(b, map_width(partie_map(p)), map_height(partie_map(p)));
}

void *bonus_end_basic(Partie *p, Snake *s, Bonus *b)
{

    //nothing
}
void *bonus_end_speedslow(Partie *p,Snake *s,Bonus *b)
{
    snake_set_vitesse(s,2);
}

void *bonus_end_speedslowother(Partie *p,Snake *s,Bonus *b)
{
    TabSnakes *ts = partie_tab(p);
    int i;
    for (i = 0; i < ts->nb_snakes ; ++i)
    {
        if(ts->snakes[i] != s)
        {
            snake_set_vitesse(ts->snakes[i], 2);
        }
    }
}
Bonus *bonus_init(int x, int y)
{
    Bonus *new = malloc(sizeof(Bonus));
    new->coord = coord_from_xy(x, y);
    new->callback_eat = bonus_eat_basic;
    new->be = bonus_effect_init(new,&bonus_end_basic);
    new->img = BONUS_BASE;
    return new;
}

/**
 * @brief   Génère un élèment de type bonus
 *
 * @param[in]    x  largeur de la map
 * @param[in]    y  hauteur de la map
 *
 * @return La bonus initialisé
 */
Bonus *bonus_new(int x, int y)
{
    srand(time(NULL));
    GRand * randg = g_rand_new();
    gint32  rx = g_rand_int_range(randg,0,x-1);
    gint32  ry = g_rand_int_range(randg,0,y-1);
    Bonus *new = malloc(sizeof(Bonus));
    new->coord = coord_from_xy(rx, ry);
    new->callback_eat = &bonus_eat_basic;
    new->be = bonus_effect_init(new,&bonus_end_basic);
    new->img = BONUS_BASE;
    return new;
}
int bonus_time_remaining(BonusEffectApply *b)
{
    return b->time_remaining;
}
void bonus_decrease_time_remaining(BonusEffectApply *b)
{
    if(b->time_remaining >0 )
    {
        b->time_remaining--;
    }
}
BonusEffect * bonus_effect(Bonus *b)
{
    return b->be;
}

BonusEffect * bonus_effect_apply_bonus_effect(BonusEffectApply * bea)
{
    return bonus_effect(bea->bonus);
}
Bonus * bonus_effect_apply_bonus(BonusEffectApply *bea)
{
    return bea->bonus;
}
Snake * bonus_effect_apply_snake(BonusEffectApply *bea)
{
    return bea->snake;
}


int choose_advancer_bonus()
{
    GRand * randg = g_rand_new();
    int res = 0;
    int interval = BONUS_F_MAXI5 + BONUS_F_SPEED + BONUS_F_SPEED_OTHER + BONUS_F_SLOW + BONUS_F_SLOW_OTHER;
    gint32  rint = g_rand_int_range(randg, 1, interval);

    if(1 <= rint && rint < BONUS_F_MAXI5)
    {
        res = 1;
    }
    else if(BONUS_F_MAXI5 <= rint && rint < BONUS_F_SPEED+BONUS_F_MAXI5)
    {
        res = 2; 
    }
    else if(BONUS_F_SPEED+BONUS_F_MAXI5 <= rint && rint < BONUS_F_SPEED_OTHER+BONUS_F_SPEED+BONUS_F_MAXI5)
    {
        res = 3; 
    }
    else if(BONUS_F_SPEED_OTHER+BONUS_F_SPEED+BONUS_F_MAXI5 <= rint && rint < BONUS_F_SLOW+BONUS_F_SPEED_OTHER+BONUS_F_SPEED+BONUS_F_MAXI5)
    {
        res = 4; 
    }
    else if(BONUS_F_SLOW+BONUS_F_SPEED_OTHER+BONUS_F_SPEED+BONUS_F_MAXI5 <= rint && rint < interval)
    {
        res = 5;
    }

    return res;
}

/**
 * @brief   Génère un bonus rare
 *
 * @param[in]    x  largeur de la map
 * @param[in]    y  hauteur de la map
 *
 * @return La bonus initialisé
 */
Bonus *bonus_advanced_new(int x, int y)
{
    srand(time(NULL));
    GRand * randg = g_rand_new();
    gint32  rx = g_rand_int_range(randg,0,x-1);
    gint32  ry = g_rand_int_range(randg,0,y-1);
    Bonus *new = malloc(sizeof(Bonus));
    new->coord = coord_from_xy(rx, ry);

	new->img = BONUS_BASE;

    int cab = choose_advancer_bonus();

    switch(cab)
    {
        case 1:
            new->callback_eat = bonus_eat_maxi5;

            new->be = bonus_effect_init(new,&bonus_end_basic);

    		new->img = BONUS_GOLDEN;
            break;
	    case 2:
            new->callback_eat = bonus_eat_speed;

            new->be = bonus_effect_init(new,&bonus_end_speedslow);
    		new->img = BONUS_SPEED;
            break;
        case 3:
            new->callback_eat = bonus_eat_speed_others;

            new->be = bonus_effect_init(new,&bonus_end_speedslowother);
    		new->img = BONUS_SPEEDRED;
        case 4:
            new->callback_eat = bonus_eat_slow;

            new->be = bonus_effect_init(new,&bonus_end_speedslow);
    		new->img = BONUS_TURTLE;
            break;
        case 5:
            new->callback_eat = bonus_eat_slow_others;

            new->be = bonus_effect_init(new,&bonus_end_speedslowother);
            new->img = BONUS_TURTLERED;
            break;
        default:
            new->callback_eat = bonus_eat_maxi5;

            new->be = bonus_effect_init(new,&bonus_end_basic);
            new->img = BONUS_GOLDEN;
    }

    return new;
}

/**
 * @brief   Change les coordonnées de le bonus
 *
 * @param[in]   bonus  Le bonus à rafraichir
 * @param[in]   x      Largeur de la map
 * @param[in]   y      Hauteur de la map
 *
 * @return Le bonus avec de nouvelles coordonnées
 */
void bonus_update(Bonus *bonus, int x, int y)
{
    GRand * randg = g_rand_new();
    gint32  rx = g_rand_int_range(randg,0,x-1);
    gint32  ry = g_rand_int_range(randg,0,y-1);
    bonus->coord = coord_from_xy(rx, ry);
}

/**
 * @brief   Change les coordonnées de le bonus
 *
 * @param[in]   bonus  Le bonus à rafraichir
 * @param[in]   x      Largeur de la map
 * @param[in]   y      Hauteur de la map
 *
 * @return Le bonus avec de nouvelles coordonnées
 */
void bonus_advanced_update(Bonus *bonus, int x, int y)
{
    GRand * randg = g_rand_new();
    gint32  rx = g_rand_int_range(randg,0,x-1);
    gint32  ry = g_rand_int_range(randg,0,y-1);
    bonus->coord = coord_from_xy(rx, ry);
/**
 *  TODO FIX LE BUG DES IMAGES SINON CODE COMMENTER
 *  Ceci permet de changer le type de bonus uen fois mangé
    int cab = choose_advancer_bonus();

    switch(cab)
    {
        case 1:
            bonus->callback_eat = &bonus_eat_maxi5;
            bonus->img = BONUS_GOLDEN;
            break;
        case 2:
            bonus->callback_eat = &bonus_eat_speed;
            bonus->img = BONUS_SPEED;
            break;
        case 3:
            bonus->callback_eat = &bonus_eat_speed_others;
            bonus->img = BONUS_SPEEDRED;
        case 4:
            bonus->callback_eat = &bonus_eat_slow;
            bonus->img = BONUS_TURTLE;
            break;
        case 5:
            bonus->callback_eat = &bonus_eat_slow_others;
            bonus->img = BONUS_TURTLERED;
            break;
        default:
            bonus->callback_eat = &bonus_eat_maxi5;
            bonus->img = BONUS_GOLDEN;
    }
    **/
}

/**
 * @brief   Permet d'accéder aux coordonnées du bonus
 *
 * @param[in]   bonus  Le bonus dont on veut les coordonnées
 *
 * @return Les coordonnées du bonus
 */
Coord bonus_coord(Bonus *bonus)
{
    Coord res;
    if(bonus != NULL)
    {
        res = bonus->coord;
    }
    return res;
}

/**
 * @brief   Génère un BonusActor
 *
 * @param[in]   parent  Le parent du nouveau BonusActor
 * @param[in]   b       La bonus à afficher
 * @param[in]   color   La couleur de la bonus
 * @param[in]   config  Les configurations
 *
 * @return Le nouveau BonusActor initialisé
 */
BonusActor *create_bonus_actor(ClutterActor *parent, Bonus *b, ClutterColor *color,Config * config,char * image)
{
    BonusActor *res;
    ClutterActor *bonus_c_actor;
    ClutterTransition *transition_x;
    ClutterTransition *transition_y;


    bonus_c_actor = clutter_actor_new();
    clutter_actor_set_size(bonus_c_actor, config->grid_size, config->grid_size);
    clutter_actor_set_pivot_point(bonus_c_actor, 0.5, 0.5);
    clutter_actor_set_scale(bonus_c_actor, 0.8, 0.8);
    //clutter_actor_set_background_color(bonus_c_actor, color);

    // SET IMAGE POMME/BOUF/bonus

    ClutterContent *imgpomme = generate_image(b->img );
    clutter_actor_set_content(bonus_c_actor,imgpomme);
    g_object_unref(imgpomme);

    transition_x = clutter_property_transition_new("scale-x");
    g_object_set(
        transition_x,
        "duration", 1000,
        "auto-reverse", TRUE,
        "repeat-count", -1,
        "progress-mode", CLUTTER_EASE_IN_OUT_SINE,
        NULL
    );
    clutter_transition_set_from(transition_x, G_TYPE_DOUBLE, 0.8);
    clutter_transition_set_to(transition_x, G_TYPE_DOUBLE, 1.2);
    clutter_actor_add_transition(bonus_c_actor, "tr-scale-x", transition_x);
    g_object_unref(transition_x);

    transition_y = clutter_property_transition_new("scale-y");
    g_object_set(
        transition_y,
        "duration", 1000,
        "auto-reverse", TRUE,
        "repeat-count", -1,
        "progress-mode", CLUTTER_EASE_IN_OUT_SINE,
        NULL
    );
    clutter_transition_set_from(transition_y, G_TYPE_DOUBLE, 0.8);
    clutter_transition_set_to(transition_y, G_TYPE_DOUBLE, 1.2);
    clutter_actor_add_transition(bonus_c_actor, "tr-scale-y", transition_y);
    g_object_unref(transition_y);

    clutter_actor_add_child(parent, bonus_c_actor);
    clutter_actor_set_position(bonus_c_actor, b->coord.x * config->grid_size, b->coord.y * config->grid_size);

    res = malloc(sizeof(BonusActor));
    res->parent = parent;
    res->bonus_c_actor = bonus_c_actor;
    res->color = color;
    res->bonus = b;

    return res;
}

/**
 * @brief   Libérer un bonus de la mémoire
 *
 * @param[in]   bonus_actor     Le BonusActor à libérer
**/
void free_bonus_actor(BonusActor *b)
{
    clutter_actor_destroy(b->bonus_c_actor);
    clutter_color_free(b->color);
    free(b);
}

/**
 * @brief   Permet d'accéder au bonus d'un BonusActor
 *
 * @param[in]   bonus_actor Le BonusActor
 *
 * @return Le bonus du bonus_actor
 */
Bonus *bonus_actor_bonus(BonusActor *bonus_actor)
{
    return bonus_actor->bonus;
}

/**
 * @brief   Rafraîchit le bonus d'un BonusActor
 *
 * @param[in]   ba       Le BonusActor à rafraichir
 * @param[in]   config   Les configurations
 */
void bonus_actor_update( BonusActor *ba,Config * config)
{
    clutter_actor_set_position(ba->bonus_c_actor, ba->bonus->coord.x * config->grid_size, ba->bonus->coord.y * config->grid_size);
}


/*********************************/
/* Fonctions de base de tabbonus */
/********************************/
/**
 * @brief   Crée un tableau de snake.
 *
 * @return  Le tableau de snake.
 */
TabBonus *create_tab_bonus()
{
    TabBonus *res;

    res = malloc(sizeof(TabBonus));
    res->nb_bonus = 0;
    res->taille_bonus = 2;
    res->bonus = malloc(res->taille_bonus * sizeof(TabBonus *));
    return res;
}


/**
 * @brief   Libère la mémoire le tableau de snake.
 *
 * @param[in]    ts  Le tableau de snake à supprimer.
 */
void free_tab_bonus(TabBonus *ts)
{
    int i;

    for (i = 0; i < ts->nb_bonus; i++)
    {
        free(ts->bonus[i]);
    }

    free(ts->bonus);
    free(ts);
}



/**
 * @brief   Ajout d'un snake dans le tableau de snake.
 *
 * @param[in]    ts     Le tableau de bonus.
 * @param[in]    obj    L'objet à ajouter.
 *
 * @return  Le Bonus crée lors de l'ajout de obj
 */
Bonus *tab_bonus_add_object(TabBonus *ts, Bonus *obj)
{
    int i;

    for (i = 0; i < ts->nb_bonus; i++)
    {
        if (ts->bonus[i] == obj) // pas de duplicat
            return ts->bonus[i];
    }

    ts->nb_bonus++;
    if (ts->nb_bonus > ts->taille_bonus)
    {
        ts->taille_bonus *= 2;
        ts->bonus = realloc(ts->bonus,
                             ts->taille_bonus * sizeof(Bonus *));
    }

    ts->bonus[ts->nb_bonus - 1] = obj;

    return ts->bonus[ts->nb_bonus - 1];
}


/**
 * @brief   Supprime le snake dans le tableau de bonus.
 *
 * @param[in]    ts     Le tableau de bonus.
 * @param[in]    obj    L'objet à supprimer.
 */
void tab_bonus_remove_object(TabBonus *ts, Bonus *obj)
{
    int i = 0;

    while (i < ts->nb_bonus)
    {
        if (ts->bonus[i] == obj)
        {
            free(ts->bonus[i]);
            ts->bonus[i] = ts->bonus[ts->nb_bonus - 1];
            ts->bonus[ts->nb_bonus - 1] = NULL;
            ts->nb_bonus--;
            return;
        }
    }
}
/**
 * @brief   Le nombre de bonus (obselète)
 *
 * @param[in]    ts    Le tableau de bonus.
 *
 * @return Renvoie le nombre de bonus contenus dans le tableau ts.
 */
int tab_bonus_length(TabBonus *ts)
{
    return ts->nb_bonus;
}


/**
 * @brief   La place en mémoire (obselete).
 *
 * @param[in]    ts    Le tableau de bonus.
 *
 * @return  Renvoie la taille du tableau en mémoire.
 */
int tab_bonus_memory_length(TabBonus *ts)
{
    return ts->taille_bonus;
}


/**
 * @brief   Permet de récupérer le bonus le plus proche du Snake.
 *
 * @param[in]    tb    Le tableau de bonus.
 * @param[in]    s     Un snake.
 *
 *
 * @return  Renvoie le bonus qui est le plus proche du snake concerné.
 */
Bonus * bonus_near_from_snake(TabBonus *tb, Snake * s)
{
    if(tb->nb_bonus >= 1 )
    {
        Bonus * res = tb->bonus[0];

        int i;
        for (i = 0; i < tb->nb_bonus; ++i)
        {
            if(coord_distance(bonus_coord(tb->bonus[i]),snake_pos(s))  < coord_distance(bonus_coord(res),snake_pos(s)))
            {
                res = tb->bonus[i];
            }
        }

        return res;
    }
    else
    {
        perror("Erreur no bonus");
    }

}


void bonus_eat_callback(Partie *partie, Snake *snake, Bonus *nourriture)
{
    nourriture->callback_eat(partie, snake, nourriture);
}

void bonus_end_callback(Partie *partie, Snake *snake, Bonus *nourriture)
{
    void (*bonus_end_cb)(Partie *partie, Snake *snake, Bonus *nourriture);
    bonus_end_cb = nourriture->be->callback_end;
    bonus_end_cb(partie, snake, nourriture);
}