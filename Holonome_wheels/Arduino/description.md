## Contenu du présent dossier
Le dossier "Arduino" contient des exemples de fonctionnement du contrôle des ODrive via Arduino.
En particulier:
- 'arduino_moving' est un script de contrôle bas niveau du contrôleur ODrive. Il est calqué sur le fonctionnement de la librairie mais n'a pas fait ses preuves.
- 'robot_control' est capable (et a été testé) de contrôler 3 moteurs répartis sur 2 contrôleurs ODrive.

## Configuration des ODrive
Afin d'utiliser le script 'robot_control.ino', il est nécessaire de paramétrer les contrôleurs ODrive convenablement.
On va contrôler les moteurs en asservissement en vitesse avec rampe. On se place donc dans le mode de contrôle 2 (Velocity_control):
'odrvX.axisX.controller.config.control_mode=CTRL_MODE_VELOCITY_CONTROL'
Le programme Arduino se charge d'activer le controle en rampe:
'odrvX.axisX.controller.vel_ramp_enable=True'
Et d'en définir sa target:
'odrvX.axisX.controller.vel_ramp_target=XXXXXXX'

Il est à noter que pour sauvegarder une configuration ODrive, on peut utiliser la commande 'odrvX.save_configuration()'.
Attention néanmoins, cette commande ne sauvegarde que les .config !

## Fonctionnement du programme 'robot_control.ino'
Dans un premier temps, on initialise les ODrive sur les ports Serial2 et Serial 3 d'un Arduino MEGA (ou DUE)
On transmet 3 paramètres, vel_limit, current_lim et vel_ramp_enable aux noms explicites pour les deux cartes ODrive et sur chacun des axes des cartes.
On initialise les moteurs en effectuant une calibration, puis on passe en mode contrôle boucle fermée.
Ensuite, on vient définir les vitesses.

## Cas d'erreurs
Il a été observé à de très nombreuses reprises des erreurs 'OVERSPEED'. Il a été remarqué que ces erreurs disparaissaient lorsque la roue était montée sur le moteur. En correctif, on peu imaginer supprimer la détection de l'overspeed par le ODrive via la config:
'odrvX.axisX.controller.config.vel_limit_tolerance=0'
Il a aussi été observé des erreurs de type 'DC_BUS_UNDER_VOLTAGE' qui ont toujours été corrigée par un redémarrage des ODrive (Mise hors tension et sous tension)

## Voies d'amélioration
Il est nécessaire de mieux cerner les erreurs qui arrivent... Il faut aussi travailler sur la phase de calibration, comment peut-elle être supprimée (ou rendue non-nécessaire à chaque démarrage...) (Les encodeurs ODrive ont des index et n'ont donc normalement pas besoin de cette calibration à chaque démarrage)

## Documentation
La documentation du contrôleur ODrive est accessible à l'adresse suivante:
https://docs.odriverobotics.com
