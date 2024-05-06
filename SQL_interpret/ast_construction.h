#ifndef __AST_CONSTRUCTION__
#define __AST_CONSTRUCTION__

#include "type_ast.h"

// Définition des fonctions pour la construction de l'arbre syntaxique abstrait (AST)

// Crée un noeud représentant une opération binaire avec l'opérateur opr
// et les opérandes opde_gauche et opde_droit comme enfants.
Ast creer_operation(TypeOperateur opr, Ast opde_gauche, Ast opde_droit);

// Crée une feuille de l'arbre abstrait avec la valeur val.
Ast creer_valeur(int val);

// Fonction pour créer une feuille de l'arbre abstrait avec une chaine donnée
Ast creer_chaine(char* string);

// Crée un noeud représentant un attribut avec la valeur att.
Ast creer_attribut(int att);

// Fonction pour créer un noeud représentant une comparaison entre 2 arbres
Ast creer_comparateur(TypeComparateur comp, Ast opde_gauche, Ast opde_droit);

// Fonction pour créer un noeud représentant une opération booléen avec un opérateur donné
Ast creer_operationBooL(TypeOperateurBool ope, Ast opde_gauche, Ast opde_droit);

// Libère l'espace mémoire alloué pour l'arbre abstrait.
void lib_ast(Ast arbre);

#endif