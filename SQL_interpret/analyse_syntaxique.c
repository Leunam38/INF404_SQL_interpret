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
    list_ligne_t* debut_ligne_affiche = init_list_ligne();
    // ajout_entier_cellule(debut_ligne_affiche, 69691111);
    table_t* affichage = init_table("affichage", nb_attributs, attributs, debut_ligne_affiche);
    // afficher_table_final(affichage);
    int positions[nb_attributs];
    cel_colonne_tete_t* tempo = relation->tete_col;
    for (int i = 0; i < nb_attributs; i++){ // test affichage tableau positions
        positions[i] = tempo->pos;
        tempo = tempo->suiv;
    }
    for (int x = 0; x < nb_attributs; x++){ // test affichage tableau positions
        printf("position: %d\n",positions[x]);
    }

    list_ligne_t* relation_liste_ligne = relation->tete_ligne;
    afficher_ligne_final(relation_liste_ligne);
    // while(relation_liste_ligne->suiv != NULL){
        donnee_cel_t* relation_tete_ligne = relation_liste_ligne->tete;
        
        for (int i = 0; i < nb_attributs-1; i++){
            if (positions[i] > 0){
                for (int j = 0; j < positions[i]; j++){
                    relation_tete_ligne = relation_tete_ligne->suiv;
                }
            }
            if (relation_tete_ligne->type_don==ENTIER){
                printf("\nVal: %d\n", relation_tete_ligne->val);
                ajout_entier_cellule(debut_ligne_affiche, 69691111);
             } 
             else{
                printf("\nVal: %s\n", relation_tete_ligne->string);
                ajout_string_cellule(debut_ligne_affiche, relation_tete_ligne->string);
            }
        }
    //     list_ligne_t* nouvelle_ligne = init_list_ligne();
    //     relation_liste_ligne->suiv = nouvelle_ligne;
    //     relation_liste_ligne = relation_liste_ligne->suiv;
    // }  
    printf("coucoucououououoiu\n");
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
   printf("les attributs de la nouvelle table_dans requete:\n");
   afficher_colonne_tete_final(attributs);
   printf("\nNombre d'attributs_dans requete: %d \n", nb_attributs);

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
   printf("Table trouvé dans requete: ");
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

    list_ligne_t* list_lig=init_list_ligne();
    list_ligne_t* list_suiv=init_list_ligne();
    table_t* table=init_table("table",3,col_tete,list_lig);
    list_lig->suiv=list_suiv;
    ajout_entier_cellule(list_lig,69691111);
    ajout_string_cellule(list_lig,"dld,zpd");
    ajout_entier_cellule(list_lig,1);

    
    ajout_entier_cellule(list_suiv,111);
    ajout_string_cellule(list_suiv,"ARABE");
    ajout_entier_cellule(list_suiv,111);

   table_aff_t* tab_aff= init_table_aff();
   tab_aff=ajout_table_aff(tab_aff,table);
   demarrer(fichier);
   while (! fin_de_sequence()){

   rec_requete(tab_aff);

   } 
}