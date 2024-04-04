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
    table_t* affichage = init_table("affichage", nb_attributs, attributs);
    afficher_colonne_tete_final(attributs);
    //Convertir les attributs du select en tableau de  
    int positions[nb_attributs];
    cel_colonne_tete_t* attributs_temp = attributs;
    for (int i = 0; i < nb_attributs; i++){
        cel_colonne_tete_t* tempo = relation->tete_col;
        while (strcmp(tempo->nom_col,attributs_temp->nom_col)!=0){
            tempo = tempo->suiv;
        }
        positions[i] = tempo->pos;
        attributs_temp = attributs_temp->suiv;
    }

    // //TEST AFFICHAGE TABLEAU POSITIONS
    // for (int x = 0; x < nb_attributs; x++){ 
    //     printf("position: %d\n",positions[x]);
    // }
    

    list_ligne_t* relation_liste_ligne = relation->tete_ligne;
    while(relation_liste_ligne != NULL){
        
        list_ligne_t* ligne_affichage = init_list_ligne();
        
        for (int i = 0; i < nb_attributs; i++){
            donnee_cel_t* relation_tete_ligne = relation_liste_ligne->tete;
            if (positions[i] > 0){
                for (int j = 0; j < positions[i]; j++){
                    relation_tete_ligne = relation_tete_ligne->suiv;
                }
            }
            if (relation_tete_ligne->type_don==ENTIER){
                ajout_entier_cellule(ligne_affichage, relation_tete_ligne->val);
                // printf("\nVal: %d", relation_tete_ligne->val);
             } 
             else{
                ajout_string_cellule(ligne_affichage, relation_tete_ligne->string);
                // printf("\nVal: %s", relation_tete_ligne->string);
            }
        }
        ajouter_liste_ligne(affichage, ligne_affichage);
        relation_liste_ligne = relation_liste_ligne->suiv;
    }  

    //Ajout des type_don a colonne_tete attributs
    cel_colonne_tete_t* attributs_temp2 = attributs;
    donnee_cel_t* cel_temp2 = affichage->tete_ligne->tete;
    while(attributs_temp2 != NULL){
        attributs_temp2->type_don = cel_temp2->type_don;
        attributs_temp2 = attributs_temp2->suiv;
        cel_temp2 = cel_temp2->suiv;
    }

    afficher_table_final(affichage);
    return affichage;
}

void rec_requete(table_aff_t* table_aff){

    Lexeme LC=lexeme_courant();
    if (LC.nature!=SELECT){
      erreur();
   }
   avancer();
   int nb_attributs = 0;
   cel_colonne_tete_t* attributs= init_colonne_tete("col1_tempo", 0, RIEN);
   rec_seq_attribut(attributs, &nb_attributs);
   cel_colonne_tete_t* temp=attributs;
   attributs=attributs->suiv;
   free(temp->nom_col);
   free(temp);
//    printf("Les attributs de la nouvelle table dans requete:\n");
//    afficher_colonne_tete_final(attributs);
//    printf("\nNombre d'attributs_dans requete: %d \n", nb_attributs);

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
   printf("Table trouvé dans requete: \n");
   afficher_table_final(relation);
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
   cel_colonne_tete_t* col_tete=init_colonne_tete("col1",0,ENTIER);
    
    col_tete->suiv=init_colonne_tete("col2",1,STRING);    
    col_tete->suiv->suiv=init_colonne_tete("col3",2,ENTIER);

    
    
    table_t* table=init_table("table",3,col_tete);
    list_ligne_t* list_lig=init_list_ligne();
    list_ligne_t* list_suiv=init_list_ligne();
    ajout_entier_cellule(list_lig,69691111);
    ajout_string_cellule(list_lig,"dld,zpd");
    ajout_entier_cellule(list_lig,1);
    ajout_entier_cellule(list_suiv,111);
    ajout_string_cellule(list_suiv,"ARABE");
    ajout_entier_cellule(list_suiv,112);
    ajouter_liste_ligne(table, list_lig);
    ajouter_liste_ligne(table, list_suiv);

   table_aff_t* tab_aff= init_table_aff();
   tab_aff=ajout_table_aff(tab_aff,table);
   demarrer(fichier);
   while (! fin_de_sequence()){

   rec_requete(tab_aff);

   } 
}