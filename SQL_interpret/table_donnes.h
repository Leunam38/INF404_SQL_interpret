#ifndef __TAB_DONNEE__
#define __TAB_DONNEE__

#include "type_table_donnes.h"
#include "table_affectation.h"
#include "type_ast.h"
#include "ast_construction.h"
#include "ast_parcours.h"

// Initialise une cellule de colonne (tête de colonne)
cel_colonne_tete_t* init_colonne_tete();

// Remplit une cellule de colonne avec les informations spécifiées
void remplissage_colonne_tete(cel_colonne_tete_t* cel_col, char* nom, int pos, Type_donnees type_don);

// Libère la mémoire allouée pour une cellule de colonne
void lib_cel_colonne_tete(cel_colonne_tete_t* cel_col);

// Initialise une cellule de donnée avec le type spécifié
donnee_cel_t* init_donnee_cel(Type_donnees type_don);

// Libère la mémoire allouée pour une cellule de donnée
void lib_donnee_cel(donnee_cel_t* cel_don);

// Initialise une liste de lignes
list_ligne_t* init_list_ligne(void);

// Libère la mémoire allouée pour une liste de lignes
void lib_list_ligne(list_ligne_t* list_lig);

// Initialise une affectation de cellule
affectation_cel_t* init_affectation_cel();

// Initialise une table
table_t* init_table();

// Remplit une table avec les informations spécifiées
void remplissage_table(table_t* table, char* nom, int nb_arg, cel_colonne_tete_t* tete_col);

// Libère la mémoire allouée pour une table
void lib_table(table_t* table);

// Ajoute une chaîne à une cellule de la liste de lignes spécifiée
void ajout_string_cellule(list_ligne_t* ll, char* string);

// Ajoute un entier à une cellule de la liste de lignes spécifiée
void ajout_entier_cellule(list_ligne_t* ll, int entier);

// Convertit un type de donnée en chaîne de caractères
char *type_don_vers_Chaine(Type_donnees type);

// Affiche les informations d'une cellule de donnée
void afficher_donnee_cel(donnee_cel_t* cel_don);

// Affiche les informations d'une cellule de colonne
void afficher_colonne_tete(cel_colonne_tete_t* cel_col);

// Affiche les informations d'une table
void afficher_table(table_t* table);

// Affiche les informations d'une table avec affectation
void affichage_table_aff(table_aff_t* tab_aff);

// Affiche le contenu d'une cellule de colonne pour une représentation finale de la table
void afficher_colonne_tete_final(cel_colonne_tete_t* cel_col);

// Affiche les tirets pour une représentation finale de la table
void afficher_tiret_tete_final(cel_colonne_tete_t* cel_col);

// Affiche une ligne pour une représentation finale de la table
void afficher_ligne_final(list_ligne_t* ligne, cel_colonne_tete_t* cel_col);

// Ajoute une liste de lignes à une table
void ajouter_liste_ligne(table_t* table, list_ligne_t* ligne);

// Affiche le contenu final de la table
void afficher_table_final(table_t* table);

#endif
