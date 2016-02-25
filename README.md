
ALPHA SNAKE
===========
Récuparation des sources
------------------------
Ouvrir un terminal et taper:
    
    git clone ssh://git@git.ares-ensiie.eu:2222/buchert2018/alpha-snake.git
    cd alpha-sanke
    git checkout develop
    git branch #pour verifier s'il y a une étoile devant develop
    git pull #obselete



Compilation
-----------

Ouvrir un terminal et taper :

    autoreconf
    ./configure
    make
    ./bin/alpha-snake
    
Autres cibles définies dans le Makefile :

 - `make clean` : supprime les fichiers objet générés par le compilateur.
 - `make maintainer-clean` : le grand ménage.


Require
-------
clutter en version 1.0 dev
    
    sudo apt-get install libclutter-1.0-dev
