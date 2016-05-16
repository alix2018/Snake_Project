/**
 * @file      ia.h
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Toutes les fonctions permettant de créer de nouvelles ia.
 * @details   ---
 */



#ifndef ALPHA_SNAKE_IA_H
#define ALPHA_SNAKE_IA_H

#include "struc.h"
#include "list.h"
#include "partie.h"


void snake_forward_ia1(Snake *snake_ia,Partie *p);
void snake_forward_ia2(Snake *snake_ia,Partie *p);
void snake_set_direction_ia(Snake * snake_ia,Partie *p,char * ia_name);
#endif //ALPHA_SNAKE_IA_H
