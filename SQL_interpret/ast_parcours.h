#ifndef __AST_PARCOURS__
#define __AST_PARCOURS__

#include "analyse_lexicale.h"
#include "table_affectation.h"
#include "type_table_donnes.h"
#include "type_ast.h"
#include "table_donnes.h"


void erreur_comparaison_entre_2_type_diff();

// Fonction qui retourne vrai si l est un comparateur
int est_comparateur(Lexeme l) ;

// Fonction qui transforme une nature en typeComparaison
TypeComparateur tranfo_nature_TypeComp(Nature_Lexeme nat) ;

// Fonction qui transforme une nature de lexème en type de comparateur
TypeOperateur tranfo_nature_TypeOperateur(Nature_Lexeme nature);

//Renvoie vrai ssi le type de donnée de l'attribut est un string
char* est_type_str(list_ligne_t* lst_lig, Ast A);

//Renvoie l'entier evaluer grace à un calcul
int evaluation(Ast expr,list_ligne_t* lst_lig) ;

//Retourne un booléen 0= pas valide 1=valide
int evaluation_bool(Ast expr,list_ligne_t* lst_lig) ;


//Affiche l'operateur 
void aff_operateur(TypeOperateur op);

// Afficher un opérateur donné 
void aff_comparateur(TypeComparateur op);

// Afficher un opérateur donné
void aff_operateurBool(TypeOperateurBool op);

// Afficher l'arbre abstrait
void afficherA(Ast expr) ;


#endif

