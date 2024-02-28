#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table_donnes.h"
#include "type_table_donnes.h"

colonne_int_t* init_colonne_int(char* nom){
    colonne_int_t *colonne = (colonne_int_t*)malloc(sizeof(colonne_int_t));
    colonne->taille_col=0;
    colonne->nom_col=strdup(nom);
    return colonne;
}

void lib_colonne_int (colonne_int_t* col){
    free(col->nom_col);
    donnee_int_t* tet_col=col->tete_col;
    while(tet_col!=NULL){
        donnee_int_t* suiv=tet_col->suiv;
        free(tet_col);
        tet_col=suiv;
    }
    free(col);
}

void ajout_donnee_int (colonne_int_t* col,int nb){
    if (col==NULL){
        printf("Erreur : colonne NULL\n");
        return;
    }

    col->taille_col++;
    donnee_int_t *nvldon = (donnee_int_t*)malloc(sizeof(donnee_int_t));
    nvldon->val=nb;

    if (col->tete_col==NULL){        
        col->tete_col=nvldon;
        return;
    }
    else {
        donnee_int_t* don=col->tete_col;
        nvldon->suiv=don;
        col->tete_col=nvldon;
    }    
}


colonne_str_t* init_colonne_str(char* nom){
    colonne_str_t *colonne = (colonne_str_t*)malloc(sizeof(colonne_str_t));
    colonne->taille_col=0;
    colonne->nom_col=strdup(nom);
    return colonne;
}

void lib_colonne_str (colonne_str_t* col){
    free(col->nom_col);
    donnee_str_t* tet_col=col->tete_col;
    while(tet_col!=NULL){
        donnee_str_t* suiv=tet_col->suiv;
        free(tet_col->valeur);
        free(tet_col);
        tet_col=suiv;
    }
    free(col);
}
void ajout_donnee_str (colonne_str_t* col,char* string){
    if (col==NULL){
        printf("Erreur : colonne NULL\n");
        return;
    }

    col->taille_col++;
    donnee_str_t *nvldon = (donnee_str_t*)malloc(sizeof(donnee_str_t));
    nvldon->valeur=strdup(string);

    if (col->tete_col==NULL){        
        col->tete_col=nvldon;
        return;
    }
    else {
        donnee_str_t* don=col->tete_col;
        nvldon->suiv=don;
        col->tete_col=nvldon;
    }   
}


list_col_t* init_list_colonne(void){
    list_col_t *list_col = (list_col_t*)malloc(sizeof(list_col_t));
    list_col->type_don=RIEN;
    return list_col;
}

void lib_list_colonne(list_col_t* list_col){
    while (list_col!=NULL){
        list_col_t* suiv_list_col=list_col->suiv;
        switch(list_col->type_don){
            case ENTIER:
                lib_colonne_int(list_col->col_int);
                break;
            case STRING:
                lib_colonne_str(list_col->col_str);
                break;
            default:
                printf("tu libère quelques choses non défini\n");
                break;
        }
        free(list_col);
        list_col=suiv_list_col;
    }
}

table_t* init_table(char* nom){
    table_t *table = (table_t*)malloc(sizeof(table_t));
    list_col_t* list_colo=init_list_colonne();
    table->nb_arg=0;
    table->prm_col=list_colo;
    table->nom_table=strdup(nom);
    return table;
}

void lib_table(table_t* table){
    if (table==NULL){return;}

    lib_list_colonne(table->prm_col);
    free(table->nom_table);
    free(table);
}


void ajout_colonne_int (table_t* table,colonne_int_t* col){
    if (table==NULL){
        printf("Erreur : table NULL\n");
    }

    list_col_t* list_col=table->prm_col;
    table->nb_arg++;
    if (list_col->type_don==RIEN){
        list_col->type_don=ENTIER;
        list_col->col_int=col;
        return;
    }
    else{
        list_col_t* nvl_list_col=init_list_colonne();
        nvl_list_col->type_don=ENTIER;
        nvl_list_col->col_int=col;
        nvl_list_col->suiv=list_col;
        table->prm_col=nvl_list_col;
        return;
    }
}

void ajout_colonne_str(table_t* table,colonne_str_t* col){
    if (table==NULL){
        printf("Erreur : table NULL\n");
    }

    list_col_t* list_col=table->prm_col;
    table->nb_arg++;
    if (list_col->type_don==RIEN){
        list_col->type_don=STRING;
        list_col->col_str=col;
        return;
    }
    else{
        list_col_t* nvl_list_col=init_list_colonne();
        nvl_list_col->type_don=STRING;
        nvl_list_col->col_str=col;
        nvl_list_col->suiv=list_col;
        table->prm_col=nvl_list_col;
        return;
    }
}







void afficher_donnee_int(donnee_int_t* don){
    while (don!=NULL){
        printf("nb:%d\n",don->val);
        don=don->suiv;
    }
}

void afficher_colonne_int(colonne_int_t* col){
    if (col==NULL){
        printf("Erreur : Colonne NULL\\n");
        return;
    }
    printf("Colone:%s, avec %d éléments\n",col->nom_col,col->taille_col);
    afficher_donnee_int(col->tete_col);
}


void afficher_donnee_str(donnee_str_t* don){
    while (don!=NULL){
        printf("str:%s\n",don->valeur);
        don=don->suiv;
    }
}
void afficher_colonne_str(colonne_str_t* col){
    if (col==NULL){
        printf("Erreur : Colonne NULL\\n");
        return;
    }
    printf("Colone:%s, avec %d éléments\n",col->nom_col,col->taille_col);
    afficher_donnee_str(col->tete_col);
}


void afficher_table(table_t* table){
    if (table==NULL){
        printf("Erreur : table NULL\\n");
        return;
    }

    list_col_t* list_col=table->prm_col;
    printf("Nom de la table:%s avec %d arguments\n",table->nom_table,table->nb_arg);
    while(list_col!=NULL){
        switch (list_col->type_don){
            case ENTIER:
                afficher_colonne_int(list_col->col_int);
                break;
            case STRING:
                afficher_colonne_str(list_col->col_str);
                break;
            default:
                printf("CAS RIENpas d'affichage tu devrais pas voir ça\n");
                break;
        }
        list_col=list_col->suiv;
    }
}