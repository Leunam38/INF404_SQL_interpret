#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table_donnes.h"

int main(int argc, char ** argv){

    //Initialisation d'une première colonne
    colonne_int_t* colonne1=init_colonne_int("cuisine");
    for (int i=10;i>0;i--){
        ajout_donnee_int(colonne1,i);
    }

    //Initialisation d'une seconde colonne
    colonne_int_t* colonne2=init_colonne_int("salon");
    for (int i=100;i<=111;i++){
        ajout_donnee_int(colonne2,i);
    }
    // afficher_colonne_int(colonne2);

    //Initialisation de la table
    table_t* table_test=init_table("table test");
    ajout_colonne_int(table_test,colonne1);
    ajout_colonne_int(table_test,colonne2);

    //Initialisation d'une troisième colonne avec des string
    colonne_str_t* colonne3=init_colonne_str("toilette");
    ajout_donnee_str(colonne3,"coucou1");
    ajout_donnee_str(colonne3,"coucou12");
    ajout_donnee_str(colonne3,"1coucou1");
    ajout_donnee_str(colonne3,"c2oucou1");
    ajout_donnee_str(colonne3,"co3ucou1");
    ajout_donnee_str(colonne3,"cou4cou1");
    ajout_donnee_str(colonne3,"couc5ou1");
    ajout_donnee_str(colonne3,"c2ou6cou1");
    ajout_donnee_str(colonne3,"co3uc6ou1");
    ajout_donnee_str(colonne3,"cou4co6u1");
    ajout_donnee_str(colonne3,"couc5ou61");
    // afficher_colonne_str(colonne3);

    ajout_colonne_str(table_test,colonne3);
    afficher_table(table_test);

    lib_table(table_test);
}