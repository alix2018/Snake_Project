//
// Created by stef on 14/05/16.
//

#ifndef ALPHA_SNAKE_APPLICATION_H
#define ALPHA_SNAKE_APPLICATION_H

#include <clutter/clutter.h>
#include "partie.h"

typedef struct _Application {
    ClutterScript*  ui;
    Partie *partie;
    ClutterActor *zone_snake;
    ClutterActor *menu_general;
    ClutterActor *menu_partie;
    ClutterActor *stage;
    Config *config;
}Application;

void init_application(Application *app, int width, int height);

#endif //ALPHA_SNAKE_APPLICATION_H
