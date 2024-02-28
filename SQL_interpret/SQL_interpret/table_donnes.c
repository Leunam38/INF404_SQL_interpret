#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table_donnes.h"
#include "type_table_donnes.h"

//Initialisation d'une colonne d'entier
colonne_int_t* init_colonne_int(char* nom){
    colonne_int_t *colonne = (colonne_int_t*)malloc(sizeof(colonne_int_t));
    colonne->taille_col=0;
    colonne->nom_col=strdup(nom);//Récupère le nom et l'alloue en mémoire
    return colonne;
}

//Libère une colonne d'entier
void lib_colonne_int (colonne_int_t* col){
    free(col->nom_col); //On doit libérer son nom vu qu'il a été alloué
    donnee_int_t* tet_col=col->tete_col;

    //Boucle qui libére toutes les données de la colonne
    while(tet_col!=NULL){
        donnee_int_t* suiv=tet_col->suiv;
        free(tet_col);
        tet_col=suiv;
    }
    free(col);
}

//Ajoute une donnée dans une colonne d'entier
void ajout_donnee_int (colonne_int_t* col,int nb){
    //Si la colonne est vide
    if (col==NULL){
        printf("Erreur : colonne NULL\n");
        return;
    }

    col->taille_col++; //On ajoute 1 en taille
    donnee_int_t *nvldon = (donnee_int_t*)malloc(sizeof(donnee_int_t)); // On allou la mémoire pour cette nouvelle donée
    nvldon->val=nb; //On met stock la valeur donnée

    //Si il n'y a jamais eu d'autre valeur dans la colonne
    if (col->tete_col==NULL){        
        col->tete_col=nvldon;
        return;
    }
    //Sinon on la place devant les anciennes données
    else { 
        donnee_int_t* don=col->tete_col;
        nvldon->suiv=don;
        col->tete_col=nvldon;
    }    
}

//Initialisation d'une colonne de string
colonne_str_t* init_colonne_str(char* nom){
    colonne_str_t *colonne = (colonne_str_t*)malloc(sizeof(colonne_str_t));
    colonne->taille_col=0;
    colonne->nom_col=strdup(nom);//Récupère le nom et l'alloue en mémoire
    return colonne;
}


//Libère une colonne de string
void lib_colonne_str (colonne_str_t* col){
    free(col->nom_col);//On doit libérer son nom vu qu'il a été alloué
    donnee_str_t* tet_col=col->tete_col;

    //Boucle qui libére toutes les données de la colonne
    while(tet_col!=NULL){
        donnee_str_t* suiv=tet_col->suiv;
        free(tet_col->valeur); //On doit libérer la valeur qui est un string car il a été alloué
        free(tet_col);
        tet_col=suiv;
    }
    free(col);
}

//Ajoute une donnée dans une colonne de string
void ajout_donnee_str (colonne_str_t* col,char* string){
    //Si la colonne est vide
    if (col==NULL){
        printf("Erreur : colonne NULL\n");
        return;
    }

    col->taille_col++;//On ajoute 1 en taille
    donnee_str_t *nvldon = (donnee_str_t*)malloc(sizeof(donnee_str_t));// On allou la mémoire pour cette nouvelle donée
    nvldon->valeur=strdup(string);//On met stock la valeur donnée en allouant de la mémoire

    //Si il n'y a jamais eu d'autre valeur dans la colonne
    if (col->tete_col==NULL){        
        col->tete_col=nvldon;
        return;
    }
    //Sinon on la place devant les anciennes données
    else {
        donnee_str_t* don=col->tete_col;
        nvldon->suiv=don;
        col->tete_col=nvldon;
    }   
}

//Initalisation d'une liste de colonne
list_col_t* init_list_colonne(void){
    list_col_t *list_col = (list_col_t*)malloc(sizeof(list_col_t));
    list_col->type_don=RIEN; //Pas de type spécifique au départ
    return list_col;
}

//Libére une liste de colonne
void lib_list_colonne(list_col_t* list_col){
    //Boucle qui libére chaque colonne
    while (list_col!=NULL){
        list_col_t* suiv_list_col=list_col->suiv;

        //Suivant le type de donnée qu'il y avait
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

//Initialisation d'une table
table_t* init_table(char* nom){
    table_t *table = (table_t*)malloc(sizeof(table_t)); 
    list_col_t* list_colo=init_list_colonne(); //On crée ici la liste de colonne qui est lié à la table
    table->nb_arg=0; //Nombre d'argument de la table
    table->prm_col=list_colo; //On relie la table à la liste de colonne
    table->nom_table=strdup(nom); //On stock en mémoire le nom de la table
    return table;
}

//On libère la table
void lib_table(table_t* table){
    if (table==NULL){return;}

    lib_list_colonne(table->prm_col);//On libère d'abord la liste de colonne
    free(table->nom_table);
    free(table);
}

//On ajoute une colonne d'entier
void ajout_colonne_int (table_t* table,colonne_int_t* col){
    //Si la table est innexistante
    if (table==NULL){
        printf("Erreur : table NULL\n");
    }

    list_col_t* list_col=table->prm_col;//On récupère la liste de colonne de notre table
    table->nb_arg++; // On rajoute un nouvel argument à la table

    //Si il n'y a pas d'autre colonne dans la table
    if (list_col->type_don==RIEN){
        list_col->type_don=ENTIER; //On change le type de la colonne
        list_col->col_int=col; //On met le pointeur vers la colonne
        return;
    }
    //Si il y a déjà une autre colonne dans la table
    else{
        //On initialise une nouvelle colonne
        list_col_t* nvl_list_col=init_list_colonne();
        nvl_list_col->type_don=ENTIER;
        nvl_list_col->col_int=col;
        nvl_list_col->suiv=list_col;//On l'a met en tête de la table
        table->prm_col=nvl_list_col;
        return;
    }
}

//On ajoute une colonne de string
void ajout_colonne_str(table_t* table,colonne_str_t* col){
    //Si la table est innexistante
    if (table==NULL){
        printf("Erreur : table NULL\n");
    }

    list_col_t* list_col=table->prm_col;//On récupère la liste de colonne de notre table
    table->nb_arg++;// On rajoute un nouvel argument à la table

    //Si il n'y a pas d'autre colonne dans la table
    if (list_col->type_don==RIEN){
        list_col->type_don=STRING;//On change le type de la colonne
        list_col->col_str=col;//On met le pointeur vers la colonne
        return;
    }
    //Si il y a déjà une autre colonne dans la table
    else{
         //On initialise une nouvelle colonne
        list_col_t* nvl_list_col=init_list_colonne();
        nvl_list_col->type_don=STRING;
        nvl_list_col->col_str=col;
        nvl_list_col->suiv=list_col;//On l'a met en tête de la table
        table->prm_col=nvl_list_col;
        return;
    }
}

//Affiche brutement les données d'entier
void afficher_donnee_int(donnee_int_t* don){
    while (don!=NULL){
        printf("nb:%d\n",don->val);
        don=don->suiv;
    }
}

//Affiche brutement les coloonnes d'entier
void afficher_colonne_int(colonne_int_t* col){
    if (col==NULL){
        printf("Erreur : Colonne NULL\\n");
        return;
    }
    printf("Colone:%s, avec %d éléments\n",col->nom_col,col->taille_col);
    afficher_donnee_int(col->tete_col);
}

//Affiche brutement les données de string
void afficher_donnee_str(donnee_str_t* don){
    while (don!=NULL){
        printf("str:%s\n",don->valeur);
        don=don->suiv;
    }
}

//Affiche brutement les colonnes de string
void afficher_colonne_str(colonne_str_t* col){
    if (col==NULL){
        printf("Erreur : Colonne NULL\\n");
        return;
    }
    printf("Colone:%s, avec %d éléments\n",col->nom_col,col->taille_col);
    afficher_donnee_str(col->tete_col);
}

//Affiche brutement les tables (qui sont pas encore sous forme de table)
void afficher_table(table_t* table){
    if (table==NULL){
        printf("Erreur : table NULL\\n");
        return;
    }

    list_col_t* list_col=table->prm_col;
    printf("Nom de la table:%s avec %d arguments\n",table->nom_table,table->nb_arg);
    //Tant qu'on a pas parcouru toute les colonnes
    while(list_col!=NULL){
        switch (list_col->type_don){//Suivant leur type
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