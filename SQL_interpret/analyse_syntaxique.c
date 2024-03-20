#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"

void erreur(){
    printf("Erreur syntaxique, on n'est pas à la fin\n");
    exit(1);
}

void rec_requete(){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=SELECT){
      erreur();
   }
   avancer();
   req_seq_attribut();
   if (LC.nature!=FROM){
      erreur();
   }
   avancer();
   if (LC.nature!=CHAINE){  //table
      erreur();
   }
   avancer();
   switch (LC.nature) {
        case SEPINST:
            avancer();
            break;
        case WHERE:
            avancer();
            rec_condition();
            if (LC.nature!=SEPINST){
                erreur();
            }
            avancer();
            break;
        default:
            erreur();
            break;
   }
}

void rec_seq_attribut(){
    Lexeme LC=lexeme_courant();
    if (LC.nature != CHAINE){   //attribut
        erreur();
    }
    avancer();
    rec_suite_seq_attribut();
}

void rec_suite_seq_attribut(){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case VIRGULE:
            avancer();
            rec_suite_seq_attribut();
            break;
        default:
            break;  //epsilon
    }
}

void rec_condition(){
    Lexeme LC=lexeme_courant();
    if (LC.nature != CHAINE){   //attribut
        erreur();
    }
    avancer();
    rec_op();
    rec_valeur();
}

void rec_op(){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case EGAL:
            avancer();
        case SUP:
            avancer();
        case INF:
            avancer();
        default:
            erreur();
    }
}

void rec_valeur(){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case CHAINE:
            avancer();
        case ENTIER:
            avancer();
        default:
        erreur();
    }
}

void analyser(char *fichier) {
   /*ouverture et lecture du 1er caractere*/
   demarrer(fichier);
   Rec_exp();
}