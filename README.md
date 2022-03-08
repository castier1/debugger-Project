# Analyse de processus en cours d’exécution

## Description

Ce projet permet de creer une application pouvant analyser un autre programme. Il repose sur le principe de communication entre deux processus, où un processus lance le binaire donné et réalise une série d'analyses de l’extérieur.
Pour cela, le programme principale communique avec l'utilisateur via différentes commandes.

## Fonctionalités

* `help`
* `exit`
* `run`
* `PID`
* `PPID`
* `GID`
* `signal`
* `pwd`
* `meta`

## Utilisation

### Installation et compilation

`make`

### Lancement

`./bin/main <path-to-file>`

### Nettoyage

`make clean`

## Exemple

`make`

`./bin/main bin/bad`

or

`make`

`./bin/main bin/good`

## Autre