#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table_donnes.h"

int main(int argc, char ** argv){
    colonne_int_t* colonne1=init_colonne_int("cuisine");
    for (int i=10;i>0;i--){
        ajout_donnee_int(colonne1,i);
    }
    // afficher_colonne_int(colonne1);

    colonne_int_t* colonne2=init_colonne_int("salon");
    for (int i=100;i<=111;i++){
        ajout_donnee_int(colonne2,i);
    }
    // afficher_colonne_int(colonne2);
    table_t* table_test=init_table("table test");
    ajout_colonne_int(table_test,colonne1);
    // afficher_table(table_test);
    ajout_colonne_int(table_test,colonne2);
    // afficher_table(table_test);
    // lib_table(table_test);

    colonne_str_t* colonne3=init_colonne_str("toilette");
    char var[50];
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