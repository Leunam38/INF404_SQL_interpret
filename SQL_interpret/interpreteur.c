#include <stdio.h>
#include <stdlib.h>

#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"

int main (int argc, char *argv[]){
    if (argc != 2){
        printf("Nombre d'arguments incorrects!");
        exit(1);
    }
    table_aff_t* tab_aff= init_table_aff();
    analyser("tables_test.txt",tab_aff); //bdd par defaut
    arreter();
    analyser(argv[1],tab_aff); //bdd d'autres fichirs
    lib_table_aff(tab_aff);
    
    arreter();
    return 0;
}