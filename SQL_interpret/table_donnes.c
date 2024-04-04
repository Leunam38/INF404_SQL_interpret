#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table_donnes.h"
#include "type_table_donnes.h"

int TAILLE_MAX_STRING=40;
int TAILLE_MAX_ENTIER=10;

//Initialisation d'une colonne d'entier
cel_colonne_tete_t* init_colonne_tete(char* nom,int pos,Type_donnees type_don){
    cel_colonne_tete_t *cel_col = (cel_colonne_tete_t*)malloc(sizeof(cel_colonne_tete_t));
    cel_col->pos=pos;
    cel_col->type_don=type_don;
    cel_col->nom_col=strdup(nom);//Récupère le nom et l'alloue en mémoire
    cel_col->suiv=NULL;
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
    don_cel->string=NULL;
    don_cel->suiv=NULL;
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
    list_lig->tete=NULL;
    list_lig->queue=NULL;
    list_lig->suiv=NULL;
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

table_t* init_table(char* nom,int nb_arg,cel_colonne_tete_t *tete_col){
    table_t *table = (table_t*)malloc(sizeof(table_t));
    table->nb_arg=nb_arg;
    table->nom_table=strdup(nom);//Récupère le nom et l'alloue en mémoire
    table->tete_col=tete_col;
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
}

//Affiche brutement les données d'entier
void afficher_table(table_t* table){
    printf("nom de la table:%s,nombre argu:%d\n",table->nom_table,table->nb_arg);
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
        ll->tete = init_donnee_cel(ENTIER);
        ll->tete->val = entier;
        ll->queue=ll->tete;
        return;
    } 
    ll->queue->suiv = init_donnee_cel(ENTIER);
    ll->queue->suiv->val = entier;
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


void afficher_colonne_tete_final(cel_colonne_tete_t* cel_col){
    while (cel_col!=NULL){
        int taille_nom=strlen(cel_col->nom_col);
        printf("%s",cel_col->nom_col);
        switch(cel_col->type_don){
            case STRING:
                while (taille_nom<TAILLE_MAX_STRING){
                    printf(" ");
                    taille_nom++;
                }
                break;
            case ENTIER:
                while (taille_nom<TAILLE_MAX_ENTIER){
                    printf(" ");
                    taille_nom++;
                }
                break;
            default:
                break;
        }
        printf(" ");
        cel_col=cel_col->suiv;
    }
}

void afficher_tiret_tete_final(cel_colonne_tete_t* cel_col){
    while (cel_col!=NULL){
        int taille_nom=strlen(cel_col->nom_col);
        switch(cel_col->type_don){
            case STRING:
                if (taille_nom<TAILLE_MAX_STRING){
                    taille_nom=TAILLE_MAX_STRING;
                }
                for (int i=0;i<taille_nom;i++){
                    printf("-");
                }
                break;
            case ENTIER:
                if (taille_nom<TAILLE_MAX_ENTIER){
                    taille_nom=TAILLE_MAX_ENTIER;
                }
                for (int i=0;i<taille_nom;i++){
                    printf("-");
                }
                break;
            default:
                break;
        }
        printf(" ");
        cel_col=cel_col->suiv;
    }
}
void afficher_ligne_final(list_ligne_t* ligne){
    
    char nb_string[50];
    while(ligne!=NULL){
        donnee_cel_t* cel = ligne->tete;
        
        int taille_str;
        while (cel!=NULL){
            switch(cel->type_don){
                case STRING:
                    printf("%s",cel->string);
                    taille_str=strlen(cel->string);
                    while (taille_str<TAILLE_MAX_STRING){
                        printf(" ");
                        taille_str++;
                    }
                    break;
                case ENTIER:
                    printf("%d",cel->val);
                    snprintf(nb_string,50,"%d",cel->val);
                    taille_str=strlen(nb_string);
                    while (taille_str<TAILLE_MAX_ENTIER){
                        printf(" ");
                        taille_str++;
                    }
                    break;
                default:
                    break;
            }
            printf(" ");
            cel=cel->suiv;
        }
        printf("\n");
        ligne=ligne->suiv;
    }
}

void ajouter_liste_ligne(table_t* table, list_ligne_t* ligne){
    list_ligne_t* ligne_temp = table->tete_ligne;
    if (ligne_temp == NULL){
        table->tete_ligne = ligne;
        table->queue_ligne = table->tete_ligne;
        return;
    } 
    table->queue_ligne->suiv = ligne;
    table->queue_ligne = table->queue_ligne->suiv;
}


void afficher_table_final(table_t* table){    
    printf("nom de la table:%s,nombre argu:%d\n",table->nom_table,table->nb_arg);
    afficher_colonne_tete_final(table->tete_col);
    printf("\n");
    afficher_tiret_tete_final(table->tete_col);
    printf("\n");
    afficher_ligne_final(table->tete_ligne);
    printf("\n");
}




