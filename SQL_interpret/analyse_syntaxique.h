#ifndef __ANA_SYN__
#define __ANA_SYN__

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "table_affectation.h"
#include "table_donnes.h"

table_t* requete_select(table_t* relation, cel_colonne_tete_t* attributs, int nb_attributs, Lexeme tab_where[3]);
void erreur();
void rec_requete(table_aff_t* tab_aff);
void rec_seq_attribut(cel_colonne_tete_t* col_tete, int* nb_attributs);
void rec_suite_seq_attribut(cel_colonne_tete_t* col_tete, int* nb_attributs);
void rec_condition();

void analyser(char *fichier);
// -- e.i : indifferent
// -- e.f : une Requete a ete lue dans fichier
// -- si elle contient une erreur, le programme 
// -- se termine avec un message

#endif 
