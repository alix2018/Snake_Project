
ALPHA SNAKE
===========

Dépendances
-----------

La bibliothèque Clutter (≥ 1.16) est nécessaire pour compiler et exécuter le
jeu.

Compilation
-----------

Ouvrir un terminal et taper :

    ./configure
    make
    
Autres cibles définies dans le Makefile :

 - `make clean` : supprime les fichiers objet générés par le compilateur.
 - `make maintainer-clean` : supprime tous les fichiers générés par make et 
                             autoconf.
 - `make tests` : compile les tests unitaires.
 - `make doc` : Génère la documentation.

Exécution
---------

    ./bin/alpha-snake [pseudo]
    
Pour les tests :

    ./bin/tests
