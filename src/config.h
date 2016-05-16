/**
 * @file      config.h
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Configuration de tout le jeu (snake, schlangà, bonus, taille du plateau...)
 * @details   ---
 */

#ifndef ALPHA_SNAKE_CONFIG_H
#define ALPHA_SNAKE_CONFIG_H

#define DEFAULT_NB_SNAKES 2
#define DEFAULT_NB_BONUS 1
#define DEFAULT_ADVANCED_BONUS 1
#define DEFAULT_TAILLE_SNAKE 5
#define DEFAULT_TAILLE_BOT 5
#define DEFAULT_HEIGHT 17
#define DEFAULT_WIDTH 25
#define DEFAULT_TYPE_PARTIE 1
#define DEFAULT_GRID_SIZE 25
#define DEFAULT_COLLISION 1
#define DEFAULT_INTERVAL_TIMEOUT 50
#define DEFAULT_INTERVAL_SCALE 6
#define DEFAULT_SCREEN_WIDTH 1366
#define DEFAULT_SCREEN_HEIGHT 600

typedef struct _config Config;

/**
 * @brief      Les configurations général et d'une partie
 */
/**
 * @brief      La structure d'une configuration générale
 *
 * @param[in]  nb_snake         Le nombre de snakes.
 * @param[in]  nb_bonus         Le nombre de bonus.
 * @param[in]  taille_snake     La taille du snake.
 * @param[in]  taille_bot       La taille des schlangà.
 * @param[in]  nb_snake         Le nombre de snakes.
 * @param[in]  nb_bonus         Le nombre de bonus.
 * @param[in]  height           La hauteur de la map.
 * @param[in]  weight           La largeur de la map.
 * @param[in]  type_partie      0 pour la partie par défaut, 1 pour classique,  2 pour la partie invasion.
 * @param[in]  grid_size        Taille de la grille.
 * @param[in]  collision        1 pour avoir des collisions, 0 sinon.
 * @param[in]  advanced_bonus   1 pour avoir des bonus avancés, 0 sinon.
 * @param[in]  interval_timeout Intervelle de temps.
 * @param[in]  interval_scale   Intervalle d'espace.
 * @param[in]  screen_width     La largeur de l'écran.
 * @param[in]  screen_height    La hauteur de l'écran.
 *
 */
struct _config
{
    int nb_snakes;
    int nb_bonus;
    int taille_snake;
    int taille_bot;
    int height;
    int width;
    int type_partie; // 0 pour partie par défaut 1 pour classique  2 pour invasion et 3 pour reseau
    int grid_size;
    int collision;
    int advanced_bonus;
    int interval_timeout;
    int interval_scale;
    int screen_width;
    int screen_height;

};

Config * init_default_config();
void free_config(Config *c);

Config * init_config();


#endif //ALPHA_SNAKE_CONFIG_H
