//
// Created by thibaut on 14/03/16.
//

#include "struc.h"
#include "list.h"

#ifndef ALPHA_SNAKE_IA_H
#define ALPHA_SNAKE_IA_H

void snake_forward_ia1(struct snake *snake_ia, struct snake *snake, Coord bouf);
void snake_set_direction_ia(struct snake *snake_ia, struct snake *snake, Coord bouf,char * ia_name);
#endif //ALPHA_SNAKE_IA_H
