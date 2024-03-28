#ifndef __TAB_AFF__
#define __TAB_AFF__

#include "type_table_donnes.h"

//Structure pour la table de table
struct table_aff
{
    char* nom_de_table; //nom de la table
    table_t* la_table_cor; //Table correspondante
    struct table_aff *suiv;
};
typedef struct table_aff table_aff_t;

table_aff_t* init_table_aff(void);

table_aff_t* ajout_table_aff(table_aff_t* tab_aff,table_t* table);

table_t* search_table_aff(table_aff_t* tab_aff, char* nom_tab_aff);

void lib_table_aff(table_aff_t* tab_aff);




#endif 