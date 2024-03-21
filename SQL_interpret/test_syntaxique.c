#include <stdio.h>
#include <stdlib.h>

#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"

int main (int argc, char *argv[]){
    if (argc != 2){
        printf("Nombre d'arguments incorrects!");
        exit(1);
    }
    analyser(argv[1]);
    arreter();
    return 0;
}