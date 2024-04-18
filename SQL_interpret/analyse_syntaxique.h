#ifndef __ANA_SYN__
#define __ANA_SYN__

#include "table_affectation.h"
#include "table_donnes.h"

void erreur();
void rec_requete(table_aff_t* tab_aff);

table_t* requete_select(table_t* relation, cel_colonne_tete_t* attributs, int nb_attributs);
void rec_deb_select(table_aff_t* table_aff);
void rec_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs);
void rec_suite_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs);
void rec_condition();
void rec_op();
void rec_valeur();

void rec_seq_suite_cree_attribut(cel_colonne_tete_t* colonnes,int* n);
void rec_type(cel_colonne_tete_t* colonnes);
void rec_creation_attribut(cel_colonne_tete_t* colonnes,int* n);
void rec_seq_creation_attribut(cel_colonne_tete_t* colonnes, int* n);
void rec_create(table_aff_t* table_aff);

void rec_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col);

cel_colonne_tete_t* rec_seq_suite_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col);

void Rec_seq_values(table_t* table);

void debut_insert(table_aff_t* table_aff);

void analyser(char *fichier);
// -- e.i : indifferent
// -- e.f : une Requete a ete lue dans fichier
// -- si elle contient une erreur, le programme 
// -- se termine avec un message

#endif 
