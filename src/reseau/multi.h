#ifndef _MULTI_
#define _MULTI_

#include <clutter/clutter.h>
#include "../struc.h"
#include "../bonus.h"
#include "../affichage.h"
#include "../partie.h"
#include "client.h"

typedef struct _Partie Partie;

void init_multi(int argc, char **argv, Partie *partie, ClutterScript *ui, int width, int height);

#endif