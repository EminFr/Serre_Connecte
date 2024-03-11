Cette partie sert à faire fonctionner les actionneurs de notre Serre Connecté. Elle contient un fichier ino pour l'application Arduino.

Face avant de la serre

![Serre Face](img/Montage_Serre_face.jpg)

Le côté de la serre

![Serre Face](img/Montage_Serre_cote.jpg)


Explication du fonctionnement de la maquette.
Après le traitement de l'IA sur node Red la sortie est renvoyé vers notre deuxième carte LoRa UCA qui transmet le signal vers la carte Arduino Mega 2560 qui exécute les actions notamment :  

Ventialation : 

--> Deux servomoteurs situés sur chaque côtés de la maquette qui lève la partie surpérieure de la serre.

--> Le ventilateur situé à l'arrière de la serre qui démarre son activité. Le sens de courant est vers l'extérieur de la maquette.

--> La durée est déterminé par la variable temps_ventilation.

Arrosage :

--> La pompe a eau situé dans un réservoir d'eau qui s'active.

--> La durée est détermine par la variable temps_pompe.

Démonstration :

Commande 1 (Activation de la ventilation) :


https://github.com/EminFr/Serre_Connecte/assets/129940365/8643616c-d8a8-46ed-9ba6-5dc2da1d22af


Commande 2 (Activation de l'arrosage) :


https://github.com/EminFr/Serre_Connecte/assets/129940365/a6b09c74-9cc5-493a-a7e1-65f721374cc7


Commande 3 (Activation de la ventilation + arrosage) :



https://github.com/EminFr/Serre_Connecte/assets/129940365/59e14907-da94-4384-a736-7117bcce7611

