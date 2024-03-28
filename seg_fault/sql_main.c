#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table_donnes.h"
#include "table_affectation.h"

int main(int argc, char ** argv){

    cel_colonne_tete_t* col_tete=init_colonne_tete("col1",0,ENTIER);
    
    col_tete->suiv=init_colonne_tete("col2",1,STRING);    
    col_tete->suiv->suiv=init_colonne_tete("col3",2,ENTIER);
    // lib_cel_colonne_tete(col_tete);

    list_ligne_t* list_lig=init_list_ligne();
    ajout_entier_cellule(list_lig,6969);
    list_ligne_t* list_suiv=init_list_ligne();
    table_t* table=init_table("table princi",3,col_tete,list_lig);
    list_lig->suiv=list_suiv;
    ajout_entier_cellule(list_lig,6969);
    ajout_string_cellule(list_lig,"ARABE");
    ajout_string_cellule(list_lig,"dld,zpd");

    
    ajout_string_cellule(list_suiv,"ARABE");
    ajout_string_cellule(list_suiv,"ARABE");
    ajout_string_cellule(list_suiv,"ARABE");
    ajout_string_cellule(list_suiv,"ARABE");



    table_aff_t* tab_aff= init_table_aff();
    tab_aff=ajout_table_aff(tab_aff,table);
    
     printf("%s\n\n",tab_aff->nom_de_table);
    table_t* testtttttt=search_table_aff(tab_aff,"table princi");

    printf("PPP:%s:\n",testtttttt->nom_table);
    
    afficher_table(table);
    
    // lib_donnee_cel(cel_tete);


    // //Initialisation d'une première colonne
    // colonne_int_t* colonne1=init_colonne_int("cuisine");
    // for (int i=10;i>0;i--){
    //     ajout_donnee_int(colonne1,i);
    // }

    // //Initialisation d'une seconde colonne
    // colonne_int_t* colonne2=init_colonne_int("salon");
    // for (int i=100;i<=111;i++){
    //     ajout_donnee_int(colonne2,i);
    // }
    // // afficher_colonne_int(colonne2);

    // //Initialisation de la table
    // table_t* table_test=init_table("table test");
    // ajout_colonne_int(table_test,colonne1);
    // ajout_colonne_int(table_test,colonne2);

    // //Initialisation d'une troisième colonne avec des string
    // colonne_str_t* colonne3=init_colonne_str("toilette");
    // ajout_donnee_str(colonne3,"coucou1");
    // ajout_donnee_str(colonne3,"coucou12");
    // ajout_donnee_str(colonne3,"1coucou1");
    // ajout_donnee_str(colonne3,"c2oucou1");
    // ajout_donnee_str(colonne3,"co3ucou1");
    // ajout_donnee_str(colonne3,"cou4cou1");
    // ajout_donnee_str(colonne3,"couc5ou1");
    // ajout_donnee_str(colonne3,"c2ou6cou1");
    // ajout_donnee_str(colonne3,"co3uc6ou1");
    // ajout_donnee_str(colonne3,"cou4co6u1");
    // ajout_donnee_str(colonne3,"couc5ou61");
    // // afficher_colonne_str(colonne3);

    // ajout_colonne_str(table_test,colonne3);
    // afficher_table(table_test);

    // lib_table(table_test);
}
