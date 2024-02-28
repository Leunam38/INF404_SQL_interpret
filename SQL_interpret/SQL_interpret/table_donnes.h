#ifndef __TAB_DONNEE__
#define __TAB_DONNEE__ 

#include "type_table_donnes.h"



colonne_int_t* init_colonne_int(char* nom);
void lib_colonne_int (colonne_int_t* col);
void ajout_donnee_int (colonne_int_t* col,int nb);

colonne_str_t* init_colonne_str(char* nom);
void lib_colonne_str (colonne_str_t* col);
void ajout_donnee_str (colonne_str_t* col,char* string);


list_col_t* init_list_colonne(void);
void lib_list_colonne (list_col_t* list_col);

table_t* init_table(char * nom);
void lib_table(table_t* table);

void ajout_colonne_int(table_t* table,colonne_int_t* col);
void ajout_colonne_str(table_t* table,colonne_str_t* col);

void afficher_donnee_int(donnee_int_t* don);
void afficher_colonne_int(colonne_int_t* col);

void afficher_donnee_str(donnee_str_t* don);
void afficher_colonne_str(colonne_str_t* col);

void afficher_table(table_t* list_col);
#endif  