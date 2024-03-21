#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"
#include "table_affectation.h"
#include "table_donnes.h"



void erreur(){
    printf("Erreur syntaxique, on n'est pas à la fin\n");
    exit(1);
}

void rec_suite_seq_attribut(cel_colonne_tete_t* col_tete){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case VIRGULE:
            avancer();
            rec_seq_attribut(col_tete);
            break;
        default:
            break;  //epsilon
    }
}

void rec_seq_attribut(cel_colonne_tete_t* col_tete){
    Lexeme LC=lexeme_courant();
    if (LC.nature != VAR){   //attribut    !! modifier pour SELECT *
        erreur();
    }
    cel_colonne_tete_t* col_tete_suite=init_colonne_tete(LC.chaine,0,RIEN);
    
    col_tete->suiv = col_tete_suite;    
    
    avancer();
    rec_suite_seq_attribut(col_tete_suite);
}

void rec_requete(table_aff_t* table_aff){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=SELECT){
      erreur();
   }
   avancer();

   cel_colonne_tete_t* col_tete= init_colonne_tete("col1", 0, RIEN);
   rec_seq_attribut(col_tete);
   cel_colonne_tete_t* temp=col_tete;
   col_tete=col_tete->suiv;
   free(temp->nom_col);
   free(temp);
   afficher_colonne_tete(col_tete);

   LC=lexeme_courant();
   if (LC.nature!=FROM){
      erreur();
   }
   avancer();
   LC=lexeme_courant();
   if (LC.nature!=VAR){  
      erreur();
   }
   table_t* relation = search_table_aff(table_aff, LC.chaine);
   afficher_table(relation);
   avancer();
   LC=lexeme_courant();
   
   switch (LC.nature) {
        case SEPINST:
            avancer();
            break;
        case WHERE:
            avancer();
            rec_condition();
            LC=lexeme_courant();
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

void rec_condition(){
    Lexeme LC=lexeme_courant();
    if (LC.nature != VAR){   //attribut
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
            break;
        case SUP:
            avancer();
            LC=lexeme_courant();
            switch (LC.nature){
                case EGAL:
                    avancer();
                    break;
                default:
                    break;
            }
            break;
        case INF:
            avancer();
            LC=lexeme_courant();
            switch (LC.nature){
                case EGAL:
                    avancer();
                    break;
                default:
                    break;
            }
            break;
        default:
            erreur();
            break;
    }
}

void rec_valeur(){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case GUILLEMETS:
            avancer();
            LC=lexeme_courant();
            if (LC.nature != VAR){
                erreur();
            }
            avancer();
            LC=lexeme_courant();
            if (LC.nature != GUILLEMETS){
                erreur();
            }
            avancer();
            break;
        case NOMBRE:
            avancer();
            break;
        default:
            erreur();
            break;
    }
}

void analyser(char *fichier) {
   /*ouverture et lecture du 1er caractere*/
   cel_colonne_tete_t* col_tete=init_colonne_tete("col1",0,ENTIER);
    
    col_tete->suiv=init_colonne_tete("col2",1,STRING);    
    col_tete->suiv->suiv=init_colonne_tete("col3",2,ENTIER);
    // lib_cel_colonne_tete(col_tete);
    list_ligne_t* list_lig=init_list_ligne();
    list_ligne_t* list_suiv=init_list_ligne();
    table_t* table=init_table("table",3,col_tete,list_lig);

    donnee_cel_t* cel_tete=init_donnee_cel(ENTIER);
    cel_tete->val=2;
    donnee_cel_t* cel_suiv=init_donnee_cel(STRING);
    cel_suiv->string=strdup("blablabal"); 
    cel_tete->suiv=cel_suiv;
    cel_suiv=init_donnee_cel(ENTIER);
    cel_tete->val=992;
    cel_tete->suiv->suiv=cel_suiv;
    list_lig->tete=cel_tete;

    afficher_donnee_cel(cel_tete);
    cel_tete=init_donnee_cel(ENTIER);
    cel_tete->val=12;
    cel_suiv=init_donnee_cel(STRING);
    cel_suiv->string=strdup("heuuu"); 
    cel_tete->suiv=cel_suiv;
    cel_suiv=init_donnee_cel(ENTIER);
    cel_tete->val=1;
    cel_tete->suiv->suiv=cel_suiv;
    list_suiv->tete=cel_tete;
    list_lig->suiv=list_suiv;

   table_aff_t* tab_aff= init_table_aff();
   tab_aff=ajout_table_aff(tab_aff,table);
   demarrer(fichier);
   while (! fin_de_sequence()){

   rec_requete(tab_aff);

   } 
}