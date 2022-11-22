# 4AESE_Voilier
Projet de commande de voilier dans le cadre du cours de Périphériques STM32 de 4AESE à l'INSA Toulouse.

Auteurs :
- Arthur Gautheron
- Olivier Lagrost
- Tran Kien Trunh
- Kenza Bouzergan

Objectif : répondre à plusieurs besoins sur une maquette de voilier.
F1 : bordage des voiles
En fonction de la direction du vent, détectée par un codeur incrémental sur la girouette, l'angle des voiles est modifié pour prendre le vent de manière optimale.

F2 : orientation du voilier
La télécommande connectée au module XBEE de la maquette permet de communiquer avec le voilier, et d'imposer une vitesse de rotation au plateau.

F3 : détection du roulis
Lorsque le roulis est trop important, le voilier replie automatiquement ses voiles pour arrêter de prendre le vent et revenir droit.

F4 : communication d'informations à la télécommande
Le voilier envoie périodiquement des informations à la télécommande, telles que l'état de la batterie, l'angle des voiles...
