# Analyse de processus en cours d’exécution
Astier Candice - M1 CHPS - Projet AISE

## Description

Ce projet permet de créer une application pouvant analyser un autre programme. Il repose sur le principe de communication entre deux processus, où un processus fils lance un binaire donné et un processus père réalise une série d'analyses dessus.
Pour cela, le programme principal communique avec l'utilisateur via différentes commandes.

L'objectif de ce projet est de simuler l'action de [gdb](https://www.sourceware.org/gdb/).

## Fonctionalités

### Commandes de base
* `help`: pour afficher la liste des commandes disponibles.
* `exit`: pour quitter l'application.
* `run` : pour lancer l'exécution du programme.

### Environnement du programme
* `pid`: pour avoir le ProcessusID du programme analysé.
* `ppid`: pour avoir le ProcessusID de l'application (programme qui analyse).
* `gid` : pour avoir le GroupID du programme analysé.
* `pgid`: pour avoir le GroupID de l'application (programme qui analyse).
* `pwd` : pour avoir le chemin absolu dans la machine (si possible) du programme analysé.
* `file`: pour avoir le nom du fichier source du programme analysé.
* `meta`: pour avoir les propriétés générales du programme analysé (type, mode, propriétaire, tailles, dates).


Pour la suite des commandes, les arguments entre crochets sont optionnels et ceux entre chevrons désignent des noms de variables à remplacer.

### Deboggage
* `lib`: pour avoir la liste de toutes les bibliothèques dynamiques (`.so`) chargées.
* `fd`: pour avoir la liste de tous les noms de descripteurs de fichiers ouverts par le programme annalysé.
* `signal`: pour analyser les signaux du programme. (Pour plus de pertinence, appeler cette commande après après avoir appelé la commande `run`.).
* `gvar`: pour avoir la liste de toutes les variables globales utilisées (nom et taille).
* `func`: pour afficher l'ensemble des fonctions du programme analysé.
* `locate <function>`: pour afficher (si possible) la localisation (fichier source et ligne) d'une fonction utilisée dans le programme analysé (cf. __Installations__).
* `dump [<function>]`: pour afficher l'ensemble ou seulement une fonction (si précisé) du binaire désassemblé (cf. __Installations__).
* `syscall [all]`: pour afficher un seul (en cours) ou tous les appels systèmes du programme.
* `next`: pour pouvoir continuer l'exécution jusqu'au prochain appel système.
* `stack`: pour avoir l'état de la pile d'appel d'un processus. Affiche le contenu du fichier `/proc/[pid]/stack`. (Demande les droits sudo.)
* `bp list`: pour afficher la liste des breakpoints créés.
* `bp add <function>`: pour créer un breakpoint à une fonction donnée (liste des fonctions accessibles via `func`). (cf. __Problèmes rencontrés et ouverture__)
* `bp rm <function>/all`: pour supprimer le breakpoint d'une fonction donnée (liste des breakpoints possibles via `bp list`), ou pour supprimer tout les breakpoints de la liste. (cf. __Problèmes rencontrés et ouverture__)

## Utilisation

### Installations

Pour pouvoir utiliser correctement l'ensemble des fonctionnalités, il faut vérifier que les packages suivants sont bien installés sur la machine utilisée:
* [binutils](https://www.gnu.org/software/binutils/): pour pouvoir utiliser les commandes `dump` (qui appellent [objdump](https://linux.die.net/man/1/objdump "man objdump")) et `locate` (qui appellent [addr2line](https://linux.die.net/man/1/addr2line "man addr2line")).

### Lancement

Pour lancer la compilation et la création de l'exécutable, il suffit de lancer la commande `make` dans un terminal.

Puis, pour pouvoir analyser un programme exterieur, lancer la commande:

`./analyzer <path-to-file>`

Si le programme a besoin d'arguments, lancer la commande:

`./analyzer <path-to-file> arg1 arg2 ...`

### Commandes

La communication avec l'analyseur se fait via l'ensemble des commandes présentées dans la partie __Fonctionalités__.

Par exemple:
```
analyze >>> help
	help     to show this message
	extit    to quit this interface
	run      to run the program
	signal   to print the last signal received
	pid      to print the PID
	ppid     to print the Parent PID
	gid      to print the GID
	pgid     to print the Parent GID
	pwd      to print the absolute path of the program to analyse
	file     to print the name of the source code file
	meta     to print all the metadata of the file to analyse (type, mode, owner, size, time)
	file     to print the list of all the dynamic librairies loaded
	fd       to print all the file descriptor opened
	func     to print all the function of the program to analyse
	dump [<func>]    to dump all the program or just a given function (need objdump)
	syscall [all]    to check if there is a syscall at the time
                        (if 'all' option, then run and print all the syscall of the program)
	next     to jump to the next syscall instruction
	locate <func>    to print the location (file and line) of a given function (need addr2line)
	stack    to print the state of the program-to-analyse's stack (need sudo rights)
	gvar     to print all the global data in the program
	bp list	 to print all the breakpoints created
	bp add <func>    to create a breakpoint at a function [Not functional yet]
	bp rm <func>/all     to remove a breakpoint at a function
	                     or all the breakpoints created[Not functional yet]
analyse >>> pid
	 71776
analyse >>> ppid
	 71775
analyse >>> exit
	Process 71776 killed
```

### Nettoyage

La commande `make clean` permet de nettoyer le dossier `bin/` et de supprimer l'exécutable `analyzer`.

## Exemples
### good
La suite de commandes:

1. `make good`
2. `./analyzer bin/good`

permet de tester le binaire créé d'après le programme `good.c`dans `src/`. Celui-ci est un programme simple ne générant pas d'erreurs.

### bad
La suite de commandes:

1. `make bad`
2. `./analyzer bin/bad`

permet de tester le binaire créé d'après le programme `bad.c`dans `src/`. Celui-ci est un programme simple générant une erreur de segmentation.

## Autre
### Problèmes rencontrés et ouverture
L'un des problèmes se situe au niveau de la gestion des breakpoints, avec la résolution des adresses à transmettre à `ptrace()`.

Dans la suite, on pourrait ajouter plus de commandes, telles que:
* `lvar` (liste l'ensemble des variables locales) s'appuyant sur la structure DWARF et la bibiothèque libdwarf,
* `bt` (backtrace) s'appuyant sur la bibiothèque libdunwind.

## References
* man pages
* cours AISE
* elf : https://linux.die.net/man/5/elf, https://www.pwnthebox.net/write/your/own/xxxx/2019/11/15/writing-an-elf-parsing-library-part1-what-is-elf.html
* https://stackoverflow.com/
* ptrace : https://www.linuxjournal.com/article/6100
* https://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1
* syscall table : https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md
* signal : https://www.mkssoftware.com/docs/man5/siginfo_t.5.asp
* wait : https://www.gnu.org/software/libc/manual/html_node/Process-Completion-Status.html