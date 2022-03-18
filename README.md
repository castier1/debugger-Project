# Analyse de processus en cours d’exécution
Astier Candice - M1 CHPS - Projet AISE

## Description

Ce projet permet de creer une application pouvant analyser un autre programme. Il repose sur le principe de communication entre deux processus, où un processus fils lance le binaire donné et un processessus père réalise une série d'analyses de l’extérieur.
Pour cela, le programme principale communique avec l'utilisateur via différentes commandes.

## Fonctionalités

### Commandes de base
* `help`: pour afficher la liste des commandes disponibles.
* `exit`: pour quitter l'application.
* `run` : pour lancer l'execution du programme.

### Environnement du programme
* `PID`: pour avoir le ProcessusID du programme analysé.
* `PPID`: pour avoir le ProcessusID de l'applications (programme qui analyse).
* `GID` : pour avoir le GroupID du programme analysé.
* `PGID`: pour avoir le GroupID de l'applications (programme qui analyse).
* `pwd` : pour avoir le chemin absolu dans la machine (si possible) du programme analysé.
* `file`: pour avoir le nom du fichier source du programme analysé.
* `meta`: pour avoir les propriétés générales du programme analysé (type, mode, propriétaire, tailles, dates).

### Deboggage
* `lib`: pour avoir la liste de toutes les bibliothèques dynamiques (`.so`) chargées.
* `fd`: pour avoir la liste de tous les noms de descripteurs de fichiers ouvert par le programme annalysé.
* `signal`: pour analyser les signaux du programme. (Pour plus de pertinance, appeler cette commande après après avoir appeler la commande `run`.).
* `func`: pour afficher l'ensemble des fonctions du programme analysé.
* `locate <function>`: pour afficher (si possible) la localisation (fichier source et ligne) d'une fonction utilisée dans le programme analysé (voir partie __Installations__).
* `dump [<function>]`: pour afficher l'ensemble ou une seulement fonction (si précisé) du binaire désassemblé (voir partie __Installations__).
* `syscall [all]`: pour afficher un seul (en cours) ou tous les appels systèmes du programme.
* `next`: pour pouvoir continuer l'execution jusqu'au prochain appel système.

## Utilisation

### Installations

Pour pouvoir utiliser correctement l'ensemble des fonctionnalités, il faut vérifier que les packages suivants sont bien installé sur la machine utilisée:
* [binutils](https://www.gnu.org/software/binutils/): pour pouvoir utiliser les commandes `dump` (qui appelle [objdump](https://linux.die.net/man/1/objdump "man objdump")) et `locate` (qui appelle [addr2line](https://linux.die.net/man/1/addr2line "man addr2line")).

### Lancement

Pour lancer la compilation et la création de l'executable, il suffit de lancer la commande `make` dans un terminal.

Puis, pour pouvoir analyser un programme exterieur, lancer la commande:

`./analyzer <path-to-file>`

Si le programme a besoin d'arguments, lancer la commande:

`./analyzer <path-to-file> arg1 arg2 ...`

### Commandes

La communication avec l'analyseur se fait via l'ensemble des commandes présentées dans la partie __Fonctionalités__.

Par exemple:
```
analyze >>> help
	help    to show this message
	extit   to quit this interface
	run     to run the program
	signal  to print the last signal received
	PID     to print the PID
	PPID    to print the Parent PID
	GID     to print the GID
	PGID    to print the Parent GID
	pwd     to print the absolute path of the program to analyse
	file    to print the name of the source code file
	meta    to print all the metadata of the file to analyse (type, mode, owner, size, time)
	file    to print the list of all the dynamic librairies loaded
	fd      to print all the file descriptor opened
	func    to print all the function of the program to analyse
	dump [<func>]    to dump all the program or just a given function (need objdump)
	syscall [all]    to check if there is a syscall at the time
                        (if 'all' option, then run and print all the syscall of the program)
	next    to jump to the next syscall instruction
	locate <func>    to print the location (file and line) of a given function (need addr2line)
analyse >>> PID
	 71776
analyse >>> PPID
	 71775
analyse >>> exit
	Process 71776 killed
```

### Nettoyage

La commande `make clean` permet de nettoyer le dossier `bin/` et de supprimer l'executable `analyzer`.

## Exemples
### good
La suite de commandes:

1. `make good`
2. `./analyzer bin/good`

permet de tester le binaire créé d'après le programme `good.c`dans `src/`. Celui-ci est un programme simple ne générant pas d'erreur.

### bad
La suite de commandes:

1. `make bad`
2. `./analyzer bin/bad`

permet de tester le binaire créé d'après le programme `bad.c`dans `src/`. Celui-ci est un programme simple générant une erreur de segmentation.

## Autre
