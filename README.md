# Analyse de processus en cours d’exécution
Astier Candice - M1 CHPS - Projet AISE

## Description

Ce projet permet de creer une application pouvant analyser un autre programme. Il repose sur le principe de communication entre deux processus, où un processus lance le binaire donné et réalise une série d'analyses de l’extérieur.
Pour cela, le programme principale communique avec l'utilisateur via différentes commandes.

## Fonctionalités

* `help`: pour afficher la liste des commandes disponibles.
* `exit`: pour quitter l'application.
* `run` : pour lancer l'execution du programme.
* `signal`: pour analyser les signaux du programme. (Pour plus de pertinance, appeler cette commande après après avoir appeler la commande `run`.)
* `PID`: pour avoir le ProcessusID du programme analysé.
* `PPID`: pour avoir le ProcessusID de l'applications (programme qui analyse).
* `GID` : pour avoir le GroupID du programme analysé.
* `pwd` : pour avoir le chemin absolu dans la machine (si possible) du programme analysé.
* `file`: pour avoir le nom du fichier source du programme analysé.
* `meta`: pour avoir les propriétés générales du programme analysé (type, mode, propriétaire, tailles, dates)

## Utilisation

### Installation et compilation

Pour lancer la compilation et la création de l'executable, il suffit de lancer la commande `make` dans un terminal.

### Lancement

Pour pouvoir analyser un programme exterieur, lancer la commande:

`./bin/main <path-to-file>`

Si le programme a besoin d'arguments, lancer la commande:

`./bin/main --args <path-to-file> arg1 arg2 ...`

### Nettoyage

La commande `make clean` permettera de nettoyer le dossier `bin/` et de supprimer l'executable.

## Exemple
### good
La suite de commandes:

1. `make`
2. `make good`
3. `./bin/main bin/good`

permettent de tester le binaire créé d'après le programme `good.c`dans `src/`. Celui-ci est un programme simple ne générant pas d'erreur.

### bad
La suite de commandes:

1. `make`
2. `make bad`
3. `./bin/main bin/bad`

permettent de tester le binaire créé d'après le programme `bad.c`dans `src/`. Celui-ci est un programme simple générant une erreur de segmentation.

## Autre
