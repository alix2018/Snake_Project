//Contient le programme semblable à celui de affichage.c pour le multi
#include "game_multi.h"

#define GRID_SIZE 23

#ifdef _CLIENT_
#define JOUEUR 2
#endif

#ifdef _SERVEUR_
#define JOUEUR 1
#endif

gboolean timeout_tick_cb_res(gpointer data){

    printf("\n\n  ---- TIMEOUT ---- \n\n");
	UpletActor *ua = data;
    
    SnakeActor *j1 = ua->sa;
    SnakeActor *j2 = ua->sa_ia;
    BoufActor  *bouf = ua->bouf;

    float w, h;
    clutter_actor_get_size(j1->parent, &w, &h);
    int l_w = (int) w/GRID_SIZE;
    int l_h = (int) h/GRID_SIZE;

    j1->snake = serv_get_j1();
    j2->snake = serv_get_j2();

    if(!snake_border_map(j1) && !snake_border_snake(j1) && !snake_border_other(j1, j2))
    {
        snake_forward(j1->snake);
        if(snake_eat(j1->snake, bouf->bouf))
        {
            snake_increase(j1->snake);
            bouf_update(bouf->bouf, l_w, l_h);
            serv_set_bouf(bouf->bouf->coord);
            printf("Snake eat!\n");
        }
    }

    if(!snake_border_map(j2) && !snake_border_snake(j2) && !snake_border_other(j2, j1))
    {
        snake_forward(j2->snake);
        if(snake_eat(j2->snake, bouf->bouf))
        {
            snake_increase(j2->snake);
            bouf_update(bouf->bouf, l_w, l_h);
            serv_set_bouf(bouf->bouf->coord);
            printf("Snake eat!\n");
        }
    }

    serv_set_j1(j1->snake);
    serv_set_j2(j2->snake);

    Coord *bouf_coord = serv_get_bouf();

    bouf->bouf->coord.x = bouf_coord->x;
    bouf->bouf->coord.y = bouf_coord->y;

    snake_actor_update(j1);
    snake_actor_update(j2);
    bouf_actor_update(bouf);

    printf("\n\n  -- FIN TIMEOUT -- \n\n");

    return G_SOURCE_CONTINUE;
}

void init_game_m(int width, int height)
{

    ClutterInitError err = clutter_init(NULL, NULL);
    if(err == CLUTTER_INIT_SUCCESS){
        printf("Init GAME!\n");
    }
    else{
        printf("Erreur init....\n");
    }

    ClutterScript *ui;

    ui = clutter_script_new();
    clutter_script_load_from_file(ui, "gui/stage.json", NULL);
    clutter_script_connect_signals(ui, ui);

    ClutterActor *zone_snake;
    ClutterActor *stage;
    Snake *snk_j1, *snk_j2;
    SnakeActor *j1, *j2;
    Bouf *bouf;
    BoufActor *ba;

    stage = CLUTTER_ACTOR(clutter_script_get_object(ui, "stage"));
    clutter_actor_set_size(stage, width * GRID_SIZE, height * GRID_SIZE);
    printf("Stage ok\n");

    printf("\n --- INIT --- \n");

    printf("Reception j1 \n");
    snk_j1 = serv_get_j1();

    printf("Reception j2 \n");
    snk_j2 = serv_get_j2();

    
    printf("Envoie j1 \n");
    serv_set_j1(snk_j1);

    printf("Envoie j2 \n");
    serv_set_j2(snk_j2);

    printf(" --- INIT ok ---\n\n");

    Coord *bouf_coord = serv_get_bouf();
    printf("Bouf coord : (%d, %d)\n", bouf_coord->x, bouf_coord->y);
    print_coord(*bouf_coord);
    bouf = bouf_new(bouf_coord->x, bouf_coord->y);

    printf("Bouf ok\n");

    zone_snake = CLUTTER_ACTOR(clutter_script_get_object(ui, "zone_snake"));
    clutter_stage_set_key_focus(CLUTTER_STAGE(stage), zone_snake);
    
    SnakeImage *simages = snake_generate_image();

    j1 = create_snake_actor(zone_snake, snk_j1, clutter_color_new(0, 0, 255, 255), simages);
    j2 = create_snake_actor(zone_snake, snk_j2, clutter_color_new(255, 0, 0, 255), simages);
    ba = create_bouf_actor(zone_snake, bouf, clutter_color_new(0, 255, 0, 255));

    snake_actor_update(j1);
    snake_actor_update(j2);
    bouf_actor_update(ba);


    if(JOUEUR == 1)
    {
        printf("JOUEUR 1\n");
        g_signal_connect(zone_snake, "key-press-event", G_CALLBACK(zone_snake_key_press_cb), snk_j1);
        g_signal_connect(stage, "destroy", G_CALLBACK(stage_destroy_cb), j1);
    }
    else
    {
        printf("JOUEUR 2\n");
        g_signal_connect(zone_snake, "key-press-event", G_CALLBACK(zone_snake_key_press_cb), snk_j2);
        g_signal_connect(stage, "destroy", G_CALLBACK(stage_destroy_cb), j1);
    }

    UpletActor ua = uplet_actor_new(j1, j2, ba);
    g_timeout_add(200, timeout_tick_cb_res, &ua);

    ClutterContent *image = generate_image("../data/fond.jpg");
    clutter_actor_set_content (zone_snake,image);

    clutter_actor_show(stage);

    clutter_main();

    free_snake(snk_j1);
    free_snake(snk_j2);

}
