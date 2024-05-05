#ifndef __TAB_AFF__
#define __TAB_AFF__

#include "type_table_donnes.h"

// Structure pour une cellule de la table d'affectation
struct cel_aff {
    char* nom_de_table;    // Nom de la table
    table_t* la_table_cor; // Table correspondante
    struct cel_aff *suiv;  // Cellule suivante dans la liste
};
typedef struct cel_aff cel_aff_t;

// Structure pour la table d'affectation
struct table_aff {
    cel_aff_t* tete; // Tête de la liste des cellules d'affectation
};
typedef struct table_aff table_aff_t;

// Initialise une cellule de la table d'affectation
cel_aff_t* init_cel_aff(void);

// Initialise une table d'affectation
table_aff_t* init_table_aff(void);

// Ajoute une table à la table d'affectation
void ajout_table_aff(table_aff_t* tab_aff, table_t* table);

// Recherche une table dans la table d'affectation par son nom
table_t* search_table_aff(table_aff_t* tab_aff, char* nom_tab_aff);

// Affiche les tables de la table d'affectation
void afficher_table_aff(table_aff_t* tab_aff);

#endif 
