#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table_donnes.h"
#include "table_affectation.h"
#include "ast_construction.h"

// Définition de la taille maximale pour les chaînes de caractères et les entiers
int TAILLE_MAX_STRING = 30;
int TAILLE_MAX_ENTIER = 8;

// Initialise une cellule de colonne (tête de colonne)
cel_colonne_tete_t* init_colonne_tete() {
    // Alloue de la mémoire pour une cellule de colonne
    cel_colonne_tete_t *cel_col = (cel_colonne_tete_t*)malloc(sizeof(cel_colonne_tete_t));
    cel_col->pos = -1;
    cel_col->type_don = RIEN;
    cel_col->nom_col = NULL; // Initialise le nom de colonne à NULL
    cel_col->suiv = NULL;
    return cel_col;
}

// Remplit une cellule de colonne avec les informations spécifiées
void remplissage_colonne_tete(cel_colonne_tete_t* cel_col, char* nom, int pos, Type_donnees type_don) {
    cel_col->pos = pos;
    cel_col->type_don = type_don;
    cel_col->nom_col = strdup(nom); // Alloue de la mémoire pour le nom de la colonne
}

// Libère la mémoire allouée pour une cellule de colonne
void lib_cel_colonne_tete(cel_colonne_tete_t* cel_col) {
    while(cel_col != NULL) {
        cel_colonne_tete_t* suiv = cel_col->suiv;
        free(cel_col->nom_col); // Libère le nom de colonne
        free(cel_col);
        cel_col = suiv;
    }
}

// Initialise une cellule de donnée avec le type spécifié
donnee_cel_t* init_donnee_cel(Type_donnees type_don) {
    donnee_cel_t *don_cel = (donnee_cel_t*)malloc(sizeof(donnee_cel_t));
    don_cel->type_don = type_don;
    don_cel->string = NULL;
    don_cel->suiv = NULL;
    return don_cel;
}

// Libère la mémoire allouée pour une cellule de donnée
void lib_donnee_cel(donnee_cel_t* cel_don) {
    while(cel_don != NULL) {
        donnee_cel_t* suiv = cel_don->suiv;
        switch(cel_don->type_don) {
            case STRING:
                free(cel_don->string);
                break;
            default:
                break;
        }
        free(cel_don);
        cel_don = suiv;
    }
}

// Initialise une liste de lignes
list_ligne_t* init_list_ligne(void) {
    list_ligne_t *list_lig = (list_ligne_t*)malloc(sizeof(list_ligne_t));
    list_lig->tete = NULL;
    list_lig->queue = NULL;
    list_lig->suiv = NULL;
    return list_lig;
}

// Libère la mémoire allouée pour une liste de lignes
void lib_list_ligne(list_ligne_t* list_lig) {
    while(list_lig != NULL) {
        list_ligne_t* suiv = list_lig->suiv;
        lib_donnee_cel(list_lig->tete);
        free(list_lig);
        list_lig = suiv;
    }
}

// Initialise une affectation de cellule
affectation_cel_t* init_affectation_cel() {
    affectation_cel_t *aff_cel = (affectation_cel_t*)malloc(sizeof(affectation_cel_t));
    aff_cel->arbre = NULL;
    aff_cel->num_att_aff = -1;
    aff_cel->chaine_aff = NULL;
    aff_cel->suiv = NULL;
    return aff_cel;
}
// Libère la mémoire allouée pour une cellule d'affectation
void lib_affectation_cel (affectation_cel_t* aff_cel) {
    while(aff_cel != NULL) {
        affectation_cel_t* suiv = aff_cel->suiv;
        lib_ast(aff_cel->arbre);
        free(aff_cel->chaine_aff);
        free(aff_cel);
        aff_cel = suiv;
    }
}

// Initialise une table
table_t* init_table() {
    table_t *table = (table_t*)malloc(sizeof(table_t));
    table->nb_arg = 0;
    table->nom_table = NULL;
    table->tete_col = NULL;
    table->tete_ligne = NULL;
    table->primarykey = -1;
    return table;
}

// Remplit une table avec les informations spécifiées
void remplissage_table(table_t* table, char* nom, int nb_arg, cel_colonne_tete_t* tete_col) {
    table->nb_arg = nb_arg;
    table->nom_table = strdup(nom);
    table->tete_col = tete_col;
}

// Libère la mémoire allouée pour une table
void lib_table(table_t* table) {
    lib_cel_colonne_tete(table->tete_col);
    lib_list_ligne(table->tete_ligne);
    free(table->nom_table);
    free(table);
}

// Convertit un type de données en chaîne de caractères
char *type_don_vers_Chaine(Type_donnees type) {
    switch (type) {
        case ENTIER: return "ENTIER";
        case STRING: return "STRING";
        case ALL: return "ALL";
        case RIEN: return "RIEN";
        default: return "ERREUR";
    }
}

// Affiche les informations d'une cellule de colonne
void afficher_colonne_tete_debug(cel_colonne_tete_t* cel_col) {
    while (cel_col != NULL) {
        printf("nom_col:%s,type:%s,pos:%d\n", cel_col->nom_col, type_don_vers_Chaine(cel_col->type_don), cel_col->pos);
        cel_col = cel_col->suiv;
    }
}

// Affiche les informations d'une cellule de donnée
void afficher_donnee_cel_debug(donnee_cel_t* cel_don) {
    while (cel_don != NULL) {
        switch(cel_don->type_don) {
            case ENTIER:
                printf(" %d |", cel_don->val);
                break;
            case STRING:
                printf(" %s |", cel_don->string);
                break;
            default:
                printf("Sait po afficher\n");
                break;
        }
        cel_don = cel_don->suiv;
    }
    printf("\n");
}

// Affiche les informations d'une table
void afficher_table(table_t* table) {
    printf("nom de la table:%s,nombre argu: %d\n:%d:\n", table->nom_table, table->nb_arg, table->primarykey);
    afficher_colonne_tete_debug(table->tete_col);
    list_ligne_t *ligne = table->tete_ligne;
    while (ligne != NULL) {
        afficher_donnee_cel_debug(ligne->tete);
        ligne = ligne->suiv;
    }
}
// Ajoute un entier dans une cellule de la liste de lignes
void ajout_entier_cellule(list_ligne_t* ll, int entier) {
    donnee_cel_t* celtemp = ll->tete;
    if (celtemp == NULL) {
        ll->tete = init_donnee_cel(ENTIER);
        ll->tete->val = entier;
        ll->queue = ll->tete;
        return;
    } 
    ll->queue->suiv = init_donnee_cel(ENTIER);
    ll->queue->suiv->val = entier;
    ll->queue = ll->queue->suiv;
}

// Ajoute une chaîne de caractères dans une cellule de la liste de lignes
void ajout_string_cellule(list_ligne_t* ll, char* string) {
    donnee_cel_t* celtemp = ll->tete;
    if (celtemp == NULL) {
        ll->tete = init_donnee_cel(STRING);
        ll->tete->string = strdup(string);
        ll->queue = ll->tete;
        return;
    }
    ll->queue->suiv = init_donnee_cel(STRING);
    ll->queue->suiv->string = strdup(string);
    ll->queue = ll->queue->suiv;
}

// Ajoute une liste de lignes à une table
void ajouter_liste_ligne(table_t* table, list_ligne_t* ligne) {
    list_ligne_t* ligne_temp = table->tete_ligne;
    if (ligne_temp == NULL) {
        table->tete_ligne = ligne;
        table->queue_ligne = table->tete_ligne;
        return;
    } 
    table->queue_ligne->suiv = ligne;
    table->queue_ligne = table->queue_ligne->suiv;
}

// Affiche les noms de colonnes de la table avec un format final
void afficher_colonne_tete(cel_colonne_tete_t* cel_col) {
    while (cel_col != NULL) {
        int taille_nom = strlen(cel_col->nom_col);
        printf("%s", cel_col->nom_col);
        switch(cel_col->type_don) {
            case STRING:
                while (taille_nom < TAILLE_MAX_STRING) {
                    printf(" ");
                    taille_nom++;
                }
                break;
            case ENTIER:
                while (taille_nom < TAILLE_MAX_ENTIER) {
                    printf(" ");
                    taille_nom++;
                }
                break;
            default:
                break;
        }
        printf(" ");
        cel_col = cel_col->suiv;
    }
}

// Affiche des tirets de séparation pour les noms de colonnes avec un format final
void afficher_tiret_tete(cel_colonne_tete_t* cel_col) {
    while (cel_col != NULL) {
        int taille_nom = strlen(cel_col->nom_col);
        switch(cel_col->type_don) {
            case STRING:
                if (taille_nom < TAILLE_MAX_STRING) {
                    taille_nom = TAILLE_MAX_STRING;
                }
                for (int i = 0; i < taille_nom; i++) {
                    printf("-");
                }
                break;
            case ENTIER:
                if (taille_nom < TAILLE_MAX_ENTIER) {
                    taille_nom = TAILLE_MAX_ENTIER;
                }
                for (int i = 0; i < taille_nom; i++) {
                    printf("-");
                }
                break;
            default:
                break;
        }
        printf(" ");
        cel_col = cel_col->suiv;
    }
}

// Affiche les lignes de la table avec un format final
void afficher_ligne(list_ligne_t* ligne, cel_colonne_tete_t* cel_col) {
    cel_colonne_tete_t* cel_tete_col = cel_col;
    char nb_string[50];
    while (ligne != NULL) {
        donnee_cel_t* cel = ligne->tete;
        int taille_str;
        while (cel != NULL && cel_col != NULL) {
            switch(cel->type_don) {
                case STRING:
                    printf("%s", cel->string);
                    taille_str = strlen(cel->string);
                    while (taille_str < TAILLE_MAX_STRING) {
                        printf(" ");
                        taille_str++;
                    }
                    break;
                case ENTIER:
                    printf("%d", cel->val);
                    snprintf(nb_string, 50, "%d", cel->val);
                    taille_str = strlen(nb_string);
                    int taille_max = strlen(cel_col->nom_col);
                    if (TAILLE_MAX_ENTIER > taille_max) {
                        taille_max = TAILLE_MAX_ENTIER;
                    }
                    while (taille_str < taille_max) {
                        printf(" ");
                        taille_str++;
                    }
                    break;
                default:
                    break;
            }
            printf(" ");
            cel = cel->suiv;
            cel_col = cel_col->suiv;
        }
        printf("\n");
        ligne = ligne->suiv;
        cel_col = cel_tete_col;
    }
}

// Affiche une table avec un format final
void afficher_table_final(table_t* table) {
    printf("Nom de la table: %s, Nombre arguments: %d\n", table->nom_table, table->nb_arg);
    afficher_colonne_tete(table->tete_col);
    printf("\n");
    afficher_tiret_tete(table->tete_col);
    printf("\n");
    afficher_ligne(table->tete_ligne, table->tete_col);
    printf("\n");
}

// Libère la mémoire allouée pour une table d'affectation
void lib_table_aff(table_aff_t* tab_aff){
    cel_aff_t* cel_aff=tab_aff->tete;
    while (cel_aff!=NULL){
        cel_aff_t* suiv = cel_aff->suiv;
        // free(cel_aff->nom_de_table);
        lib_table(cel_aff->la_table_cor);
        free(cel_aff);
        cel_aff=suiv;
    }
    free(tab_aff);
}

