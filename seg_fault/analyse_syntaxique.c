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

void rec_suite_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case VIRGULE:
            avancer();
            rec_seq_attribut(attributs, nb_attributs);
            break;
        default:
            break;  //epsilon
    }
}

void rec_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs){
    Lexeme LC=lexeme_courant();
    if (LC.nature != VAR){   //attribut    !! modifier pour SELECT *
        erreur();
    }
    cel_colonne_tete_t* attributs_suite=init_colonne_tete(LC.chaine,0,RIEN);
    
    attributs->suiv = attributs_suite;    
    *nb_attributs += 1;
    avancer();
    rec_suite_seq_attribut(attributs_suite, nb_attributs);
}


table_t* requete_select(table_t* relation, cel_colonne_tete_t* attributs, int nb_attributs){
    list_ligne_t* debut_ligne = init_list_ligne();
    table_t* affichage = init_table("affichage", nb_attributs, attributs, debut_ligne);
    int positions[nb_attributs];
    int i = 0;
    cel_colonne_tete_t* temp = relation->tete_col;
    while(i < nb_attributs){
        positions[i] = temp->pos;
        temp = temp->suiv;
        i += 1;
    }
    for (int x = 0; x < nb_attributs; x++){ // test affichage tableau positions
        printf("position: %d\n",positions[x]);
    }

    list_ligne_t* relation_liste_ligne = relation->tete_ligne;
    while(relation_liste_ligne->suiv != NULL){
        donnee_cel_t* tete_ligne = relation_liste_ligne->tete;

        for (int i = 0; i < nb_attributs; i++){
            donnee_cel_t* temp = tete_ligne;
            if (positions[i] > 0){
                for (int j = 0; j < positions[i]; j++){
                temp = temp->suiv;
                }
            }
            if (temp->type_don ==ENTIER){
                printf("\nVal: %d\n", temp->val);
                ajout_entier_cellule(affichage->tete_ligne, temp->val);
            }else{
                printf("\nVal: %s\n", temp->string);
                ajout_string_cellule(affichage->tete_ligne, temp->string);
            }
        }
        list_ligne_t* nouvelle_ligne = init_list_ligne();
        relation_liste_ligne->suiv = nouvelle_ligne;
        relation_liste_ligne = relation_liste_ligne->suiv;
    }  
    afficher_table(affichage);
    return affichage;
}

void rec_requete(table_aff_t* table_aff){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=SELECT){
      erreur();
   }
   avancer();
   int nb_attributs = 0;
   cel_colonne_tete_t* attributs= init_colonne_tete("col1", 0, RIEN);
   rec_seq_attribut(attributs, &nb_attributs);
   cel_colonne_tete_t* temp=attributs;
   attributs=attributs->suiv;
   free(temp->nom_col);
   free(temp);
   afficher_colonne_tete(attributs);
   printf("\nNombre d'attributs: %d \n\n", nb_attributs);

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
   requete_select(relation, attributs, nb_attributs);
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
   cel_colonne_tete_t* attributs=init_colonne_tete("col1",0,ENTIER);
    
    attributs->suiv=init_colonne_tete("col2",1,STRING);    
    attributs->suiv->suiv=init_colonne_tete("col3",2,ENTIER);
    // lib_cel_colonne_tete(attributs);
    list_ligne_t* list_lig=init_list_ligne();
    list_ligne_t* list_suiv=init_list_ligne();
    table_t* table=init_table("table",3,attributs,list_lig);

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