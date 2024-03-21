
#ifndef __ANA_SYN__
#define __ANA_SYN__

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "table_affectation.h"

void erreur();
void rec_requete();
void rec_seq_attribut();
void rec_suite_seq_attribut();
void rec_condition();
void rec_op();
void rec_valeur();

void analyser(char *fichier);
// -- e.i : indifferent
// -- e.f : une Requete a ete lue dans fichier
// -- si elle contient une erreur, le programme 
// -- se termine avec un message

#endif 