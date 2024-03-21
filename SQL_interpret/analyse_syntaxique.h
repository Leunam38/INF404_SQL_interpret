
#ifndef __ANA_SYN__
#define __ANA_SYN__

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "table_affectation.h"
#include "table_donnes.h"

void erreur();
void rec_requete(table_aff_t* tab_aff);
void rec_seq_attribut(cel_colonne_tete_t* col_tete);
void rec_suite_seq_attribut(cel_colonne_tete_t* col_tete);
void rec_condition();
void rec_op();
void rec_valeur();

void analyser(char *fichier);
// -- e.i : indifferent
// -- e.f : une Requete a ete lue dans fichier
// -- si elle contient une erreur, le programme 
// -- se termine avec un message

#endif 