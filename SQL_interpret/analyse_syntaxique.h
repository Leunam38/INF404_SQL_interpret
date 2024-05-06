
#ifndef __ANA_SYN__
#define __ANA_SYN__

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "table_affectation.h"
#include "type_ast.h"
#include "ast_construction.h"
#include "table_donnes.h"
#include "ast_parcours.h"
#include "requete_SQL.h"

// --------------- ERREURS ---------------// 
void erreur();
void erreur_table_aff(char * nom_table);
void erreur_attribut_non_reconnu(char * nom_attribut);
void erreur_attribut_deja_existant();
void erreur_donnee_illegale();
void erreur_donnee_deja_existant(int num_ligne);
void erreur_colonne_inexistante();
void erreur_manque_donnee_insert();
void erreur_requete();


// --------------- CREATE ---------------//

// Fonction pour traiter la création de tables
void req_create(table_aff_t* table_aff);
// Fonction pour traiter une séquence d'attributs
void rec_seq_creation_attribut(table_t* table, cel_colonne_tete_t* colonnes, int* n);
// Fonction pour traiter la création d'un attribut
void rec_creation_attribut(table_t* table,cel_colonne_tete_t* colonnes,int* n);
// Fonction pour traiter le type d'un attribut
void rec_type(cel_colonne_tete_t* colonnes);
// Fonction pour traiter la suite de la création d'attributs
void rec_seq_suite_cree_attribut(table_t* table,cel_colonne_tete_t* colonnes,int* n);
// Fonction pour traiter une contrainte (primary key)
void rec_contrainte(table_t* table, cel_colonne_tete_t* colonnes);


// --------------- WHERE ---------------//

// Fonction pour traiter une condition dans un WHERE
void rec_condition(Ast *A,table_t* table);
// Fonction pour traiter une valeur d'une expression
void rec_valeur(Ast *A, table_t* table);
// Fonction pour traiter un eag
void rec_eag(Ast *A1,table_t* table);
// Fonction pour traiter une séquence de terme
void seq_terme(Ast *A2,table_t* table);
// Fonction pour traiter une séquence de termes dans un eag
void suite_seq_terme(Ast A1 , Ast *A2,table_t* table);
// Fonction pour traiter un terme 
void terme(Ast *A1,table_t* table);
// Fonction pour traiter un facteur
void seq_facteur(Ast *A2,table_t* table);
// Fonction pour traiter une suite de facteur 
void suite_seq_facteur(Ast A1 , Ast *A2,table_t* table);
// Fonction pour traiter un facteur 
void facteur(Ast *A1,table_t* table);
// Fonction pour traiter les opération de type 1
int op1(TypeOperateur *Op);
// Fonction pour traiter les opération de type 2
int op2(TypeOperateur *Op); 


// --------------- SELECT ---------------//

// Fonction pour traiter le début d'une requête SELECT
void req_select(table_aff_t* table_aff);
// Fonction pour traiter une séquence d'attributs dans une requête SELECT
void rec_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs, int* a_etoile);
// Fonction pour traiter la suite d'une séquence d'attributs dans une requête SELECT
void rec_suite_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs, int* a_etoile);


// --------------- INSERT ---------------//

// Fonction pour traiter les valeurs à insérer dans une ligne de la table
void rec_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col,int* position,table_t* table);
// Fonction récursive pour traiter les valeurs suivantes à insérer dans une ligne de la table
cel_colonne_tete_t* rec_seq_suite_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col,int* position,table_t* table);
// Fonction pour traiter la séquence de valeurs à insérer dans une ligne de la table
void Rec_seq_values(table_t* table);
// Fonction pour traiter le début de la requête INSERT 
void req_insert(table_aff_t* table_aff);


// --------------- UPDATE ---------------//

// Gère le traitement des affectations dans une requête UPDATE
void rec_affecation(table_t* table,affectation_cel_t* aff_cel);
// Fonction traitant la suite de l'affectation si la valeur est une chaîne ou un calcul
void rec_affecation_suite(table_t* table,affectation_cel_t* aff_cel);
// Fonction traitant la séquence des affectations supplémentaires s'il y en a
void rec_seq_suite_affecation(table_t* table,affectation_cel_t* aff_cel);
// Fonction pour traiter la requête UPDATE
void req_update(table_aff_t* table_aff);


// --------------- REQUETES GENERALES ---------------//

// Fonction reconnaissant et de traitant différents types de requêtes
void rec_requete(table_aff_t* table_aff);
// Fonction effectuant l'analyse syntaxique du fichier spécifié et appliquant les requêtes
void analyser(char *fichier,table_aff_t* tab_aff) ;

#endif 