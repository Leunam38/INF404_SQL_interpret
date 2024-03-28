#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table_donnes.h"
#include "type_table_donnes.h"

//Initialisation d'une colonne d'entier
cel_colonne_tete_t* init_colonne_tete(char* nom,int pos,Type_donnees type_don){
    cel_colonne_tete_t *cel_col = (cel_colonne_tete_t*)malloc(sizeof(cel_colonne_tete_t));
    cel_col->pos=pos;
    cel_col->type_don=type_don;
    cel_col->nom_col=strdup(nom);//Récupère le nom et l'alloue en mémoire
    return cel_col;
}

void lib_cel_colonne_tete (cel_colonne_tete_t* cel_col){

    //Boucle qui libére toutes les données de la colonne
    while(cel_col!=NULL){
        cel_colonne_tete_t* suiv=cel_col->suiv;
        free(cel_col->nom_col); //On doit libérer le nom de la colonne vu qu'il a été alloué
        free(cel_col);
        cel_col=suiv;
    }
}

donnee_cel_t* init_donnee_cel(Type_donnees type_don){
    donnee_cel_t *don_cel = (donnee_cel_t*)malloc(sizeof(donnee_cel_t));
    don_cel->type_don=type_don;
    return don_cel;
}

void lib_donnee_cel(donnee_cel_t* cel_don){

    //Boucle qui libére toutes les données de la colonne
    while(cel_don!=NULL){
        donnee_cel_t* suiv=cel_don->suiv;
        switch(cel_don->type_don){
            case STRING:
                free(cel_don->string);
                break;
            //Mettre date si il faut
            default:
                // printf("tu libère quelques choses non défini\n");
                break;
        }
        free(cel_don);
        cel_don=suiv;
    }
}

list_ligne_t* init_list_ligne(void){
    list_ligne_t *list_lig = (list_ligne_t*)malloc(sizeof(list_ligne_t));
    return list_lig;
}

void lib_list_ligne(list_ligne_t* list_lig){

    //Boucle qui libére toutes la liste de ligne
    while(list_lig!=NULL){
        list_ligne_t* suiv=list_lig->suiv;
        lib_donnee_cel(list_lig->tete);
        free(list_lig);
        list_lig=suiv;
    }
}

table_t* init_table(char* nom,int nb_arg,cel_colonne_tete_t *tete_col,list_ligne_t *tete_ligne ){
    table_t *table = (table_t*)malloc(sizeof(table_t));
    table->nb_arg=nb_arg;
    table->nom_table=strdup(nom);//Récupère le nom et l'alloue en mémoire
    table->tete_col=tete_col;
    table->tete_ligne=tete_ligne;
    return table;
}

void lib_table(table_t* table){
    lib_cel_colonne_tete(table->tete_col);
    lib_list_ligne(table->tete_ligne);
    free(table->nom_table);
    free(table);
}

char *type_don_vers_Chaine (Type_donnees type) {
	switch (type) {
        case ENTIER: return "ENTIER" ;
        case DATE: return "DATE" ;
        case STRING: return "STRING" ;            
        case RIEN: return "RIEN" ;    
      default: return "ERREUR" ;            
	} ;
   } 


//Affiche brutement les données d'entier
void afficher_colonne_tete(cel_colonne_tete_t* cel_col){
    while (cel_col!=NULL){
        printf("nom_col:%s,type:%s,pos:%d\n",cel_col->nom_col,type_don_vers_Chaine(cel_col->type_don),cel_col->pos);
        cel_col=cel_col->suiv;
        }
        //printf("nom_col:%d || ",don->val,type_don_vers_Chaine(don->type_don));
}

//Affiche brutement les données d'entier
void afficher_donnee_cel(donnee_cel_t* cel_don){
    while (cel_don!=NULL){
        //Suivant le type de donnée qu'il y avait
        switch(cel_don->type_don){
            case ENTIER:
                printf(" %d |",cel_don->val);
                break;
            case STRING:
                printf(" %s |",cel_don->string);
                break;
            default:
                printf("Sait po afficher\n");
                break;
        }
        cel_don=cel_don->suiv;
        }
        printf("\n");
        //printf("nom_col:%d || ",don->val,type_don_vers_Chaine(don->type_don));
}

//Affiche brutement les données d'entier
void afficher_table(table_t* table){
    printf("nom de la table:%s,nombre argu:%ds\n",table->nom_table,table->nb_arg);
    afficher_colonne_tete(table->tete_col);
    list_ligne_t *ligne=table->tete_ligne;
    while (ligne!=NULL){
        afficher_donnee_cel(ligne->tete);
        ligne=ligne->suiv;
        }
}

void ajout_entier_cellule(list_ligne_t* ll, int entier){
    donnee_cel_t* celtemp = ll->tete;
    if (celtemp == NULL){
        ll->tete = init_donnee_cel(STRING);
        ll->tete->val = entier;
        ll->queue=ll->tete;
        return;
    } 
    ll->queue->suiv = init_donnee_cel(ENTIER);
    celtemp->suiv->val = entier;
    ll->queue=ll->queue->suiv;
}

void ajout_string_cellule(list_ligne_t* ll, char* string){
    donnee_cel_t* celtemp = ll->tete;
    if (celtemp == NULL){
        ll->tete = init_donnee_cel(STRING);
        ll->tete->string = strdup(string);
        ll->queue=ll->tete;
        return;
    }
    ll->queue->suiv = init_donnee_cel(STRING);
    ll->queue->suiv->string = strdup(string);
    ll->queue=ll->queue->suiv;
}



