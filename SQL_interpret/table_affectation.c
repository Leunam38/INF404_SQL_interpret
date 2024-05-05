#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "type_table_donnes.h"
#include "table_affectation.h"


cel_aff_t* init_cel_aff(void){
    cel_aff_t *cel_aff = (cel_aff_t*)malloc(sizeof(cel_aff_t));
    return cel_aff;
}

table_aff_t* init_table_aff(void){
    table_aff_t *table_aff = (table_aff_t*)malloc(sizeof(table_aff_t));
    table_aff->tete=NULL;
    return table_aff;
}



void ajout_table_aff(table_aff_t* tab_aff,table_t* table){
    cel_aff_t* cel_aff;
    cel_aff=init_cel_aff();
    cel_aff->nom_de_table=table->nom_table;
    cel_aff->la_table_cor=table;

    if (tab_aff->tete==NULL){
        tab_aff->tete=cel_aff;
    }
    else {
        cel_aff->suiv=tab_aff->tete;
        tab_aff->tete=cel_aff;
    }

}

table_t* search_table_aff(table_aff_t* tab_aff, char* nom_tab_aff){
    cel_aff_t* cel_aff=tab_aff->tete;
    while(cel_aff!=NULL){
        if (strcmp(cel_aff->nom_de_table,nom_tab_aff)==0){
            return cel_aff->la_table_cor;
        }
        cel_aff=cel_aff->suiv;
    }
    return NULL;
}


void afficher_table_aff(table_aff_t* tab_aff){
    cel_aff_t* cel_aff=tab_aff->tete;
    printf("Table d'affectation:\n");
    while(cel_aff!=NULL){
        printf("%s\n",cel_aff->nom_de_table);
        cel_aff=cel_aff->suiv;
    }
}

// void lib_table_aff(table_aff_t* tab_aff){

//     //Boucle qui libére toutes les données la table d'affectation
//     while(tab_aff!=NULL){
//         table_aff_t* suiv=tab_aff->suiv;
//         free(tab_aff);
//         tab_aff=suiv;
//     }
// }

// void afficher_table_aff(table_aff_t* tab_aff){
//     while (tab_aff!=NULL){
//         printf("%s\n",tab_aff->nom_de_table);
//         tab_aff=tab_aff->suiv;
//     }
// }

