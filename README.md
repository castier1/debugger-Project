# Analyse de processus en cours d’exécution
Astier Candice - M1 CHPS - Projet AISE

## Description

Ce projet permet de creer une application pouvant analyser un autre programme. Il repose sur le principe de communication entre deux processus, où un processus fils lance le binaire donné et un processessus père réalise une série d'analyses de l’extérieur.
Pour cela, le programme principale communique avec l'utilisateur via différentes commandes.

## Fonctionalités

* `help`: pour afficher la liste des commandes disponibles.
* `exit`: pour quitter l'application.
* `run` : pour lancer l'execution du programme.
* `signal`: pour analyser les signaux du programme. (Pour plus de pertinance, appeler cette commande après après avoir appeler la commande `run`.)
* `PID`: pour avoir le ProcessusID du programme analysé.
* `PPID`: pour avoir le ProcessusID de l'applications (programme qui analyse).
* `GID` : pour avoir le GroupID du programme analysé.
* `PGID`: pour avoir le GroupID de l'applications (programme qui analyse).
* `pwd` : pour avoir le chemin absolu dans la machine (si possible) du programme analysé.
* `file`: pour avoir le nom du fichier source du programme analysé.
* `meta`: pour avoir les propriétés générales du programme analysé (type, mode, propriétaire, tailles, dates)
* `lib`: pour avoir la liste de toutes les bibliothèques dynamiques (`.so`) chargées
* `fd`: pour avoir la liste de tous les noms de descripteurs de fichiers ouvert par le programme annalysé

## Utilisation

### Installation et compilation

Pour lancer la compilation et la création de l'executable, il suffit de lancer la commande `make` dans un terminal.

### Lancement

Pour pouvoir analyser un programme exterieur, lancer la commande:

`./analyzer <path-to-file>`

Si le programme a besoin d'arguments, lancer la commande:

`./analyzer <path-to-file> arg1 arg2 ...`

### Commandes

La communication avec l'analyseur se fait via l'ensemble des commandes présentées dans la partie Fonctionalités.

Par exemple:
```
analyze >>> help
	help	to show this message
	exit	to quit this interface
	run	 	to run the program
	signal	to print the last signal received
	PID	 	to print the PID
	PPID	to print the Parent PID
	GID	 	to print the GID
	PGID	to print the Parent GID
	pwd	 	to print the absolute path of the program to analyse
	file	to print the name of the source code file
	meta	to print all the metadata of the file to analyse
		     (file type, mode, owner, file size, times)
	file	to print the list of all the dynamic librairies loaded
	fd	 	to print all the file descriptor opened
analyse >>> PID
	 71776
analyse >>> PPID
	 71775
analyse >>> exit
	Process 71776 killed
```

### Nettoyage

La commande `make clean` permettera de nettoyer le dossier `bin/` et de supprimer l'executable `analyzer`.

## Exemple
### good
La suite de commandes:

1. `make`
2. `make good`
3. `./analyzer bin/good`

permettent de tester le binaire créé d'après le programme `good.c`dans `src/`. Celui-ci est un programme simple ne générant pas d'erreur.

### bad
La suite de commandes:

1. `make`
2. `make bad`
3. `./analyzer bin/bad`

permettent de tester le binaire créé d'après le programme `bad.c`dans `src/`. Celui-ci est un programme simple générant une erreur de segmentation.

## Autre
