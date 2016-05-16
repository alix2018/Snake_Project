
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
 
Lorsque la version de Clutter installée est récente, des avertissements
indiquant que des fonctions de Clutter sont obsolètes peuvent apparaître.
L'utilisation de ces fonctions est nécessaire pour permettre la compatibilité
avec la version 1.16 de la bibliothèque.

Exécution
---------

    ./bin/alpha-snake [pseudo]
    
Pour les tests :

    ./bin/tests
