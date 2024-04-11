#ifndef __TAB_AFF__
#define __TAB_AFF__

#include "type_table_donnes.h"

//Structure pour la table de table
struct cel_aff
{
    char* nom_de_table; //nom de la table
    table_t* la_table_cor; //Table correspondante
    struct cel_aff *suiv;
};
typedef struct cel_aff cel_aff_t;

//Structure pour la table de table
struct table_aff
{
    cel_aff_t* tete; //Table correspondante
};
typedef struct table_aff table_aff_t;


cel_aff_t* init_cel_aff(void);

table_aff_t* init_table_aff(void);

void ajout_table_aff(table_aff_t* tab_aff,table_t* table);

table_t* search_table_aff(table_aff_t* tab_aff, char* nom_tab_aff);

void afficher_table_aff(table_aff_t* tab_aff);


// void lib_table_aff(table_aff_t* tab_aff);

// void afficher_table_aff(table_aff_t* tab_aff);




#endif 