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

void rec_deb_select(table_aff_t* table_aff){
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


void rec_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col){
    Lexeme LC=lexeme_courant();
    
    if (LC.nature==GUILLEMETS){
        avancer();
        if (cel_col->type_don!=STRING){
            printf("Ajout de donnée illégale\n");
            erreur();
        }
        LC=lexeme_courant();
        if (LC.nature!=VAR){
            erreur();
        }
        ajout_string_cellule(ligne,LC.chaine);
        avancer();
        LC=lexeme_courant();
        if (LC.nature!=GUILLEMETS){
            erreur();
        }
        avancer();
    }
    else if (LC.nature==NOMBRE){   
        if (cel_col->type_don!=ENTIER){
            printf("Ajout de donnée illégale\n");
            erreur();
        }
        ajout_entier_cellule(ligne,LC.val);
        avancer();
    }
    else {
        erreur();
    }
}

cel_colonne_tete_t* rec_seq_suite_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col){
    Lexeme LC=lexeme_courant();
    
    if (LC.nature==VIRGULE){
        avancer();
        rec_values(ligne,cel_col);
        if (cel_col->suiv!=NULL){
            return rec_seq_suite_values(ligne,cel_col->suiv);
        }
        return cel_col->suiv;
    }
    return cel_col;
}

void Rec_seq_values(table_t* table){
    list_ligne_t* ligne=init_list_ligne();
    cel_colonne_tete_t* cel_col;
    rec_values(ligne,table->tete_col);
    if (table->tete_col->suiv!=NULL){
        cel_col=rec_seq_suite_values(ligne,table->tete_col->suiv);
    }
    if (cel_col!=NULL){
        printf("Il manque des données à rajouter dans l'insert\n");
        erreur();
    }
    ajouter_liste_ligne(table,ligne);
}


void debut_insert(table_aff_t* table_aff){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=INSERT){
      erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=INTO){
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=VAR){
        erreur();
    }
    avancer();
    table_t* table = search_table_aff(table_aff, LC.chaine);
    LC=lexeme_courant();
    if (LC.nature!=VALUES){
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=PARO){
        erreur();
    }
    avancer();
    Rec_seq_values(table);
    
    LC=lexeme_courant();
    if (LC.nature!=PARF){
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=SEPINST){
        erreur();
    }
    avancer();
    afficher_table_final(table);
}



void rec_create(table_aff_t* table_aff){
    char nomtable[100];
    int n=0;
    cel_colonne_tete_t* colonnes=(cel_colonne_tete_t*)malloc(sizeof(cel_colonne_tete_t));
    Lexeme LC=lexeme_courant();
    if (LC.nature!=CREATE){
      erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=TABLE){
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=VAR){
        erreur();
    }
    strcpy(nomtable,LC.chaine);
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=PARO){
        erreur();
    }
    avancer();

    rec_seq_creation_attribut(colonnes,&n);
    
    LC=lexeme_courant();
    if (LC.nature!=PARF){
        erreur();
    }
    avancer();

    LC=lexeme_courant();
    if (LC.nature!=SEPINST){
        erreur();
    }
    avancer();
    printf("arbre %d\n",n);
    table_t* table = init_table(nomtable,n,colonnes);
    ajout_table_aff(table_aff,table);
    afficher_table_aff(table_aff);

}

void rec_seq_creation_attribut(cel_colonne_tete_t* colonnes, int* n){
    rec_creation_attribut(colonnes,n);
    rec_seq_suite_cree_attribut(colonnes,n);
}

void rec_creation_attribut(cel_colonne_tete_t* colonnes,int* n){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=VAR){
        erreur();
    }
    colonnes->pos=*n;
    colonnes->nom_col=strdup(LC.chaine);
    *n=*n+1;
    avancer();
    rec_type(colonnes);
}

void rec_type(cel_colonne_tete_t* colonnes){
    Lexeme LC=lexeme_courant();
    if ((LC.nature!=TEXT) && (LC.nature!=INTEGER)){
        erreur();
    }
    if (LC.nature==TEXT){
        colonnes->type_don=STRING;
    }
    else if (LC.nature==INTEGER) {
        colonnes->type_don=ENTIER;
    }
    avancer();
}

void rec_seq_suite_cree_attribut(cel_colonne_tete_t* colonnes,int* n){
    Lexeme LC=lexeme_courant();
    if (LC.nature==VIRGULE){    
        cel_colonne_tete_t* cel_suiv=(cel_colonne_tete_t*)malloc(sizeof(cel_colonne_tete_t));
        colonnes->suiv=cel_suiv;
        avancer();
        rec_creation_attribut(cel_suiv,n);
        rec_seq_suite_cree_attribut(cel_suiv,n);
    }
}



void rec_requete(table_aff_t* table_aff){
    Lexeme LC=lexeme_courant();
    switch(LC.nature){
        case SELECT :
            rec_deb_select(table_aff);
            break;
        case CREATE :
            rec_create(table_aff);
            break;
        case INSERT :
            debut_insert(table_aff);
            break;
        default:
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
   ajout_table_aff(tab_aff,table);
   demarrer(fichier);
   while (! fin_de_sequence()){

   rec_requete(tab_aff);

   } 
}