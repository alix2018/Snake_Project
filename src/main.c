#include <clutter/clutter.h>
#include "partie.h"
#include "application.h"
#include "reseau/multi.h"

int main(int argc, char **argv)
{
	
    ClutterScript *ui;
    Partie *partie;
    Application application;

    ClutterInitError err = clutter_init(&argc, &argv);

    if(argc > 2)
    {
		init_multi(argc, argv, partie, ui, 30, 30);
    }
    else
    {
    	init_application(&application);
    }

    return 0;
	 
}