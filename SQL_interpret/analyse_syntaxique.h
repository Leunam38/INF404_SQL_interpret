
#ifndef __ANA_SYN__
#define __ANA_SYN__

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "table_affectation.h"
#include "table_donnes.h"
#include "type_ast.h"
#include "ast_construction.h"
#include "ast_parcours.h"

void erreur();
void rec_requete(table_aff_t* tab_aff);

table_t* requete_select(table_t* relation, cel_colonne_tete_t* attributs, int nb_attributs, Lexeme tab_where[3]);
void erreur();
void rec_requete(table_aff_t* tab_aff);
void rec_seq_attribut(cel_colonne_tete_t* col_tete, int* nb_attributs);
void rec_suite_seq_attribut(cel_colonne_tete_t* col_tete, int* nb_attributs);
void rec_condition();


void rec_create(table_aff_t* table_aff);
void rec_seq_creation_attribut(table_t* table, cel_colonne_tete_t* colonnes, int* n);
void rec_creation_attribut(cel_colonne_tete_t* colonnes,int* n);
void rec_type(cel_colonne_tete_t* colonnes);
void rec_seq_suite_cree_attribut(table_t* table,cel_colonne_tete_t* colonnes,int* n);
void rec_contrainte(table_t* table, cel_colonne_tete_t* colonnes);

void rec_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col);

cel_colonne_tete_t* rec_seq_suite_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col);

void Rec_seq_values(table_t* table);

void debut_insert(table_aff_t* table_aff);



void rec_update(table_aff_t* table_aff);
void rec_seq_suite_affecation(table_t* table,affectation_cel_t* aff_cel);
void rec_affecation_suite(table_t* table,affectation_cel_t* aff_cel);
void rec_affecation(table_t* table,affectation_cel_t* aff_cel);


int search_attribut(table_t* table,char* attribut);

void requete_update(table_t* relation, affectation_cel_t* aff_cel, Lexeme tab_where[3]);
int evaluation_update(Ast expr,list_ligne_t* lst_lig);

void rec_eag(Ast *A1,table_t* table);
void seq_terme(Ast *A2,table_t* table);
void suite_seq_terme(Ast A1 , Ast *A2,table_t* table);
void terme(Ast *A1,table_t* table);
void seq_facteur(Ast *A2,table_t* table);
void suite_seq_facteur(Ast A1 , Ast *A2,table_t* table);
void facteur(Ast *A1,table_t* table);
int op1(TypeOperateur *Op);
int op2(TypeOperateur *Op);
TypeOperateur Operateur(Nature_Lexeme nature);



void analyser(char *fichier);
// -- e.i : indifferent
// -- e.f : une Requete a ete lue dans fichier
// -- si elle contient une erreur, le programme 
// -- se termine avec un message

#endif 