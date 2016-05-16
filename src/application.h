/**
 * @file      application.h
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Toutes les fonctions permettant d'afficher la fenpetre et le menu puis lancer le jeu
 * @details   ---
 */

#ifndef ALPHA_SNAKE_APPLICATION_H
#define ALPHA_SNAKE_APPLICATION_H

#include <clutter/clutter.h>
#include "partie.h"

/**
 * @brief      La structure de l'acteur application
 *
 * @use Application
 * @param[in]  ui                  Le fichier ui contenant la déclaration de la fenêtre du Snake.
 * @param[in]  partie              Partie à jouer.
 * @param[in]  zone_snake          Le ClutterActor qui contient la zone de jeu.
 * @param[in]  menu_general        Le ClutterActor qui contient le menu général.
 * @param[in]  menu_partie         Le ClutterActor qui contient le sous menu concernant le choix des parties.
 * @param[in]  image_alpha_snake   Le ClutterActor qui contient le logo sur le menu général
 * @param[in]  image_alpha_snake2  Le ClutterActor qui contient le logo sur le menu partie
 * @param[in]  image_alpha_snake3  Le ClutterActor qui contient le logo sur le menu partie avancée
 * @param[in]  menu_avance         Le ClutterActor qui contient le menu avancé.
 * @param[in]  stage               Le ClutterActor qui contient la fenêtre principale.
 * @param[in]  config              Le ClutterActor qui contient les configurations de jeu.
 */
typedef struct _Application {
    ClutterScript*  ui;
    Partie *partie;
    ClutterActor *zone_snake;
    ClutterActor *menu_general;
    ClutterActor *menu_partie;
    ClutterActor *image_alpha_snake;
    ClutterActor *image_alpha_snake2;
    ClutterActor *image_alpha_snake3;
    ClutterActor *menu_avance;
    ClutterActor *stage;
    Config *config;
}Application;

void init_application(Application *app);

#endif //ALPHA_SNAKE_APPLICATION_H
