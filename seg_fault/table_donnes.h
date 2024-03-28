#ifndef __TAB_DONNEE__
#define __TAB_DONNEE__ 

#include "type_table_donnes.h"
#include "table_affectation.h"

cel_colonne_tete_t* init_colonne_tete(char* nom,int pos,Type_donnees type_don);

void lib_cel_colonne_tete (cel_colonne_tete_t* cel_col);

donnee_cel_t* init_donnee_cel(Type_donnees type_don);

void lib_donnee_cel(donnee_cel_t* cel_don);

list_ligne_t* init_list_ligne(void);

void lib_list_ligne(list_ligne_t* list_lig);
table_t* init_table(char* nom,int nb_arg,cel_colonne_tete_t *tete_col,list_ligne_t *tete_ligne );

void lib_table(table_t* table);


void ajout_string_cellule(list_ligne_t* ll, char* string);
void ajout_entier_cellule(list_ligne_t* ll, int entier);



char *type_don_vers_Chaine (Type_donnees type);

void afficher_donnee_cel(donnee_cel_t* cel_don);

void afficher_colonne_tete(cel_colonne_tete_t* cel_col);

void afficher_table(table_t* table);

void affichage_table_aff(table_aff_t* tab_aff);


// colonne_int_t* init_colonne_int(char* nom);
// void lib_colonne_int (colonne_int_t* col);
// void ajout_donnee_int (colonne_int_t* col,int nb);

// colonne_str_t* init_colonne_str(char* nom);
// void lib_colonne_str (colonne_str_t* col);
// void ajout_donnee_str (colonne_str_t* col,char* string);


// list_col_t* init_list_colonne(void);
// void lib_list_colonne (list_col_t* list_col);

// table_t* init_table(char * nom);
// void lib_table(table_t* table);

// void ajout_colonne_int(table_t* table,colonne_int_t* col);
// void ajout_colonne_str(table_t* table,colonne_str_t* col);

// void afficher_donnee_int(donnee_int_t* don);
// void afficher_colonne_int(colonne_int_t* col);

// void afficher_donnee_str(donnee_str_t* don);
// void afficher_colonne_str(colonne_str_t* col);

// void afficher_table(table_t* list_col);
#endif  