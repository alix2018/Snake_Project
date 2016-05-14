//
// Created by thibaut on 14/03/16.
//



#ifndef ALPHA_SNAKE_IA_H
#define ALPHA_SNAKE_IA_H

#include "struc.h"
#include "list.h"
#include "partie.h"


void snake_forward_ia1(Snake *snake_ia,Partie *p);
void snake_forward_ia2(Snake *snake_ia,Partie *p);
void snake_set_direction_ia(Snake * snake_ia,Partie *p,char * ia_name);
#endif //ALPHA_SNAKE_IA_H
