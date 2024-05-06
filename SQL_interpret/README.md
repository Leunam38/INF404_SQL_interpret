# Système de Gestion de Base de Données (SGBD)
Auteurs : 
- Loïc GIRAUD
- Kévin DHENIN
- Emmanuel MAYOUX LI 

Ce projet consiste en un système de gestion de base de données (SGBD) simplifié, développé en langage C. Le SGBD permet de créer, manipuler et interroger des bases de données relationnelles en utilisant un ensemble de commandes spécifiques.

## Fonctionnalités
Le SGBD offre les fonctionnalités suivantes :

Création de tables : Permet de créer des tables avec des attributs spécifiés.
Insertion de données : Permet d'insérer des données dans les tables existantes.
Interrogation des données : Permet d'interroger les données à l'aide de requêtes SELECT.
Mise à jour de données : Permet de mettre à jour les données existantes dans les tables.

## Structure du Projet
Le projet est structuré de la manière suivante :

Répertoire principal: Contient les fichiers source du projet.
- analyse_lexicale.c : Module pour l'analyse lexicale du langage.
- analyse_syntaxique.c : Module pour l'analyse syntaxique du langage.
- table_donnes.c : Module pour la gestion des données.
- requete_SQL.c : Module pour appliqué certaine requetes SQL
- table_affectation.c : Module pour l'affectation de plusieurs table entre elle
- ast_construction.c/ast_parcours.c : Module pour construire et utiliser des arbre abstrait
- interpreteur.c : Point d'entrée du programme.

Tests/ : Contient les fichiers d'en-tête.
Makefile : Fichier pour automatiser la compilation du projet.
run.sh : Fichier permettant d'executer tous les tests

## Compilation: 
Utilisez 'make' pour compiler le programme.
Si vous voulez nettoyer le répertoire des fichiers, utilisez 'make clean'.


## Utilisation:
Pour exécuter le programme, utilisez './interpreteur fichier.txt'.
Spécifiez le nom du fichier contenant les commandes à exécuter en remplaçant 'fichier.txt' par le chemin du test.
Pour lancer tous les fichiers tests d'un coup faite : './run.sh' /!\ il faut que vous vous mettiez les droit d'execution sur ce fichier


## Fonctionnement:
Le programme commence par exécuter les commandes dans le fichier 'tables_exemples.txt'.
Ces commandes créent des tables remplies de valeurs.

Ensuite, le programme exécuter les commandes dans le fichier fourni (fichier.txt) pour ajouter de nouvelles tables, mettre à jour des valeurs existantes ou sélectionner des valeurs.

