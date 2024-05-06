#include <string.h>
#include <stdlib.h>
#include <stdio.h> 

#include "ast_construction.h"
#include "type_ast.h"

// Fonction pour créer un noeud représentant une opération binaire avec un opérateur donné
Ast creer_operation(TypeOperateur opr, Ast opde_gauche, Ast opde_droit){
    Ast expr;
    // Allocation de mémoire pour le nouveau noeud
    expr = (Ast) malloc (sizeof(NoeudAst));
    // Attribution des valeurs aux champs de la structure du noeud
    expr->nature = OPERATION;
    expr->operateur = opr;
    // Vérification si les opérandes sont valides
    if (opde_gauche == NULL || opde_droit == NULL) {
        printf("ERREUR_EXPRESSION\n");
        exit(1);
    } else {
        // Attribution des opérandes
        expr->gauche = opde_gauche;
        expr->droite = opde_droit;
    }
    return expr;
}

// Fonction pour créer une feuille de l'arbre abstrait avec une valeur donnée
Ast creer_valeur(int val){ 
    Ast expr;
    // Allocation de mémoire pour le nouveau noeud
    expr = (Ast) malloc (sizeof(NoeudAst));
    // Attribution des valeurs aux champs de la structure du noeud
    expr->nature = VALEUR;
    expr->valeur = val;
    return expr;
}

// Fonction pour créer une feuille de l'arbre abstrait avec une chaine donnée
Ast creer_chaine(char* string){ 
    Ast expr;
    // Allocation de mémoire pour le nouveau noeud
    expr = (Ast) malloc (sizeof(NoeudAst));
    // Attribution des valeurs aux champs de la structure du noeud
    expr->nature = CHAINESTR;
    expr->chainestr = strdup(string);
    return expr;
}

// Fonction pour créer un noeud représentant un attribut avec une valeur donnée
Ast creer_attribut(int att){ 
    Ast expr;
    // Allocation de mémoire pour le nouveau noeud
    expr = (Ast) malloc (sizeof(NoeudAst));
    // Attribution des valeurs aux champs de la structure du noeud
    expr->nature = ATTRIBUT;
    expr->num_attribut = att;
    return expr;
}

// Fonction pour créer un noeud représentant une comparaison entre 2 arbres
Ast creer_comparateur(TypeComparateur comp, Ast opde_gauche, Ast opde_droit){
    Ast expr;
    // Allocation de mémoire pour le nouveau noeud
    expr = (Ast) malloc (sizeof(NoeudAst));
    // Attribution des valeurs aux champs de la structure du noeud
    expr->nature = COMPARATEUR;
    expr->comparateur = comp;
    // Vérification si les opérandes sont valides
    if (opde_gauche == NULL || opde_droit == NULL) {
        printf("ERREUR_EXPRESSION\n");
        exit(1);
    } 
    else if (opde_gauche->nature!=OPBOOL && opde_gauche->nature!=OPBOOL && opde_droit->nature!=OPBOOL && opde_droit->nature!= COMPARATEUR) {

        // Attribution des opérandes
        expr->gauche = opde_gauche;
        expr->droite = opde_droit;
    }
    else {
        printf("Erreur: Mauvaise expression à côté de votre comparateur\n");
        exit(1);
    }
    return expr;
}

// Fonction pour créer un noeud représentant une opération booléen avec un opérateur donné
Ast creer_operationBooL(TypeOperateurBool ope, Ast opde_gauche, Ast opde_droit){
    Ast expr;
    // Allocation de mémoire pour le nouveau noeud
    expr = (Ast) malloc (sizeof(NoeudAst));
    // Attribution des valeurs aux champs de la structure du noeud
    expr->nature = OPBOOL;
    expr->ope_bool = ope;

    // Vérification si les opérandes sont valides
    if (opde_gauche == NULL || opde_droit == NULL) {
        printf("ERREUR_EXPRESSION\n");
        exit(1);
    } else {
        // Attribution des opérandes
        expr->gauche = opde_gauche;
        expr->droite = opde_droit;
    }

    //Vérification de si les noeuds enfants sont du bons type
    if (opde_gauche->nature!=OPBOOL && opde_gauche->nature!=COMPARATEUR){
        printf("Erreur: Mauvaises expression à gauche de votre opérateur booléen\n");
        exit(1);
    }
    if (opde_droit->nature!=OPBOOL && opde_droit->nature!=COMPARATEUR){
        printf("Erreur: Mauvaises expression à droite de votre opérateur booléen\n");
        exit(1);
    }
    return expr;
}


// Fonction pour libérer l'espace mémoire alloué à l'arbre abstrait
void lib_ast(Ast arbre){
    // Vérification si l'arbre est vide
    if (arbre == NULL) {
        return;
    }
    // Libération récursive des sous-arbres
    lib_ast(arbre->gauche);
    lib_ast(arbre->droite);
    // Libération du noeud courant
    
    if (arbre->nature==CHAINESTR){
        free(arbre->chainestr);
    }
    free(arbre);
}
