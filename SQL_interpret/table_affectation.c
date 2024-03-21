#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "type_table_donnes.h"
#include "table_affectation.h"

table_aff_t* init_table_aff(void){
    table_aff_t *table_aff = (table_aff_t*)malloc(sizeof(table_aff_t));
    return table_aff;
}

table_aff_t* ajout_table_aff(table_aff_t* tab_aff,table_t* table){
    while(tab_aff!=NULL){
        tab_aff=tab_aff->suiv;
    }
    tab_aff=init_table_aff();
    tab_aff->nom_de_table=table->nom_table;
        printf("%s\n\n",tab_aff->nom_de_table);
    tab_aff->la_table_cor=table;
    return tab_aff;
}

table_t* search_table_aff(table_aff_t* tab_aff, char* nom_tab_aff){
    table_aff_t* tab_aff2=tab_aff;
    while(tab_aff2!=NULL){
        printf("%s\n\n",tab_aff2->nom_de_table);
        if (strcmp(tab_aff->nom_de_table,nom_tab_aff)==0){
            return tab_aff->la_table_cor;
        }
        tab_aff2=tab_aff2->suiv;
    }
    printf("Table non trouvé");
    return NULL;
}

void lib_table_aff(table_aff_t* tab_aff){

    //Boucle qui libére toutes les données la table d'affectation
    while(tab_aff!=NULL){
        table_aff_t* suiv=tab_aff->suiv;
        free(tab_aff);
        tab_aff=suiv;
    }
}

