//Contient le programme semblable à celui de affichage.c pour le multi
#include <clutter/clutter.h>
#include <gdk/gdk.h>
#include <time.h>

#include "affichage.h"
#include "struc.h"
#include "list.h"

#ifdef _CLIENT_
#include "multi_c.h"
#endif

#ifdef _SERVEUR_
#include "multi_h.h"
#endif

#ifndef _GAME_M_
#define _GAME_M_

void init_game_m(int width, int height);

#endif