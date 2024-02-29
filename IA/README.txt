Répertoire dédié à l'IA via Node Red pour la serre connectée depuis une machine virtuelle (réalisé avec Microsoft Azure ici).

Le but de cette partie est d'implémenter du Machine Learning dans notre Serre connectée pour gérer l'arrosage et la ventilation.
Nos paramètres sont l'humidité ambiante, l'humidité du sol et la luminosité (traitée au préalable, on récupère un 0 ou 1 afin d'indiquer si nous sommes en environnement éclairé ou non).
L'objectif est de récupérer une décision sous forme de classe allant de 0 à 3:

    -0: Aucune action effectuée
    -1: Ventilation activée
    -2: Arrosage activé uniquement
    -3: Ventilation et Arrosage activés conjointement

La classe 0 est envoyée si les conditions d'humidité sont respectées.
La classe 1 est envoyée si l'humidité ambiante est trop élevée et l'humidité du sol suffisament élevée.
La classe 2 est envoyée si l'humidité du sol est trop élevée, si l'humidité ambiante est suffisament basse et si on est dans un environnement éclairé (0 sinon).
La classe 3 est activée si l'humidité ambiante est trop élevée, si l'humidité du sol trop basse et si on est dans un environnement éclairé (1 sinon).


Le flow Node Red est disponible sous le nom flow_ia.json (la version est susceptible d'avoir changée depuis son importation). Il est important de vérifier les path de chaque noeud pour qu'ils correspondent aux chemins locaux de l'utilisateur (ou de la machine virtuelle).


//************************************************************************************************************\\
Prérequis: -palette node-red-contrib-machine-learning
           -node js version 14.15.1 ou 18.19.1. 
            Si version 18.19.1, il faut modifier les fichiers utils.js, en remplaçant la ligne:
                      node.proc = spawn(pcmd, [node.file],['pipe', 'pipe','pipe'])
            Par:
                      node.proc = spawn(pcmd, [node.file],{stdio:['pipe', 'pipe','pipe']})

Afin de lier le GitHub et la machine virtuelle il est nécessaire de générer une clef SSH depuis la machine virtuelle avec cette commande:
            ssh-keygen -t rsa -b 4096 -C "adressemail@exemple.com"

Puis afficher la clef avec:
            cat ~/.ssh/id_rsa.pub

Une fois cette clef récupérée, il faut la lier sur le compte GitHub afin de récupérer les informations sur la machine virtuelle avec la commande:

            git clone git@github.com:EminFr/Serre_Connecte.git

Maintenant dans le répertoire toutes les commandes git sont disponibles pour ce repository.

Cette étape est importante car il nous faut pouvoir récupérer la base de données (ici actionneurtest.csv) afin de réaliser l'apprentissage du modèle Random Forest Classifier.

Le dossier dataset (créé automatiquement) est créé par le noeud create dataset, il contient deux variables: train.csv et test.csv, il s'agit d'une découpe de la base de données actionneurtest.csv. Cette base de données est découpée en 80/20: 80% pour le train.csv et 20% pour test.csv. Ceci est réalisé pour l'apprentissage du modèle, afin de l'entraîner et de le tester pour évaluer son efficacité avant son utilisation dans la chaîne de traitement de l'information.

Le dossier model (créé automatiquement) contient un fichier rfc.b qui correspond au modèle entraîné (ici le Random Forest Classifier). Ce modèle sera appelé dans la chaîne de traitement de l'information afin de prendre la décision vis-à-vis de la donnée traitée. Dans notre cas ce modèle après apprentissage sort une précision de 94% (parfois cela varie entre 84% et 94%, il suffit de réentraîner le modèle plusieurs fois jusqu'à obtenir la précision souhaitée).

Le dossier Donnees est créé/écrasé automatiquement lorsque le flow est déployé sur Node Red, il permet de contenir la variable nommée test.csv (différente de la précédente, il n'y a pas de rapport) qui stocke les données de la trame reçue par TTN et est écrasé à chaque réception de nouvelles données.

Le dossier historique.csv permet de garder une trace de toutes les données qui ont été traitées dans le réseau.
\\************************************************************************************************************//


Fonctionnement:

I)Après avoir déployé le flow, il est nécessaire de lancer la ligne de create dataset afin de réaliser l'importation et la découpe de la base de données (cf présentation partie 7- Machine Learning –Choix et apprentissage du modèle – Part 1).

II)Ensuite, il faut entraîner le modèle avec la ligne du dessous contenant load dataset et random forrest classifier (cf présentation partie 7- Machine Learning –Choix et apprentissage du modèle – Part 3)).

III) Une fois le modèle entraîné, on réaliser l'évaluation de la précision du modèle avec la partie en dessous du traininf (ligne contenant le noeud assesment). Dans le débogage on peut alors trouver la précision de notre modèle entraîné. (cf présentation partie 7- Machine Learning –Validation part I et II).


Le modèle est à présent entraîné et prêt à être utilisé.

IV) Sur la longue chaîne du dessous, on reçoit les données capteurs via un MQTT in lié à notre serveur TTN (qui lui-même reçoit les valeurs des capteurs depuis les devices). (cf présentation partie 7- Machine Learning – Récupération des données de TTN).

V) On met ensuite en forme la trame afin de pouvoir traiter la donnée, car nous recevons une donnée sous forme d'objet. On extrait alors de la trame les informations qui nous intéressent, à savoir les données mesurées par les capteurs et nous les affectons à une variable qui sera traitée dans la suite de la chaîne.
Cette variable est stockée et mise en forme dans un fichier csv pour qu'elle soit exploitable par le modèle en respectant la configuration des paramètres : les colonnes 0,1 et 2 correspondent aux paramètres d'entrées et la colonne 3 correspond à la sortie donc à la classe de décision (par défaut, pour respecter le format on l'initialise à 0 car elle n'a pas d'influence pour cette partie, elle sera modifiée après le passage de la donnée dans le RFC).
(cf présentation 7) Machine Learning – Mise en forme des données Part 1 et 2).

VI)La variable une fois mise en forme va être passée dans le RFC afin de savoir quelle classe (et donc quelle décision) va être choisie en fonction du jeu de données reçue. (cf présentation 7- Machine Learning – Prise de décision)).

VII)Enfin, pour transmettre la décision au serveur TTN, on doit remettre en forme la variable afin qu'elle soit sous le bon format pour la transmission.  (cf présentation 7- Machine Learning – Envoie des données de Node-Red vers TTN)


Ce petit modèle fonctionne parfaitement et est résumé dans le powerpoint de la présentation partie 7, avec des captures d'écran de la démarche.

Finalement, nous recevons bien la donnée de la décision sur TTN et d'après notre étude et nos expériences, nous avons toujours reçu la bonne décision.






