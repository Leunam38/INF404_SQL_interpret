
#ifndef __REQ_SQL__
#define __REQ_SQL__

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "table_affectation.h"
#include "table_donnes.h"
#include "type_ast.h"
#include "ast_construction.h"
#include "ast_parcours.h"

// Recherche un attribut dans une table et renvoyer sa position
int search_attribut(table_t* table,char* attribut);

// Fonction pour rechercher une clé primaire dans une table
int search_unique_key(table_t* table, char* nom_string,int nb_ent);

// Fonction pour développer le symbole '*' dans SELECT
void developpe_etoile(table_t* table, cel_colonne_tete_t* attributs, int* nb_attributs);

// Fonction pour exécuter une requête SELECT
table_t* requete_select(table_t* relation, cel_colonne_tete_t* attributs, int nb_attributs, Ast A_condi);

// Fonction pour exécuter une requête UPDATE
void requete_update(table_t* relation, affectation_cel_t* aff_cel, Ast A_condi );

#endif 