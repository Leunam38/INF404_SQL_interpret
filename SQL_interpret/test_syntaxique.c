#include <stdio.h>
#include <stdlib.h>

#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"

int main (int argc, char *argv[]){
    if (argc != 2){
        printf("Nombre d'arguments incorrects!");
        exit(1);
    }
    // analyser("tables_test.txt"); //bdd par defaut
    analyser(argv[1]); //bdd d'autres fichirs
    arreter();
    return 0;
}