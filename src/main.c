#include <clutter/clutter.h>
#include "partie.h"
#include "application.h"

int main(int argc, char **argv)
{
	
    ClutterScript *ui;
    Partie *partie;
    Application application;

    ClutterInitError err = clutter_init(&argc, &argv);

    init_application(&application);

    return 0;
	 
}
	
