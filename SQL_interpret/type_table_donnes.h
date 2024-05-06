#ifndef __TYPE_TAB__
#define __TYPE_TAB__ 

#include "type_ast.h"

// Type énuméré pour savoir quel type de donnée on stocke dans notre colonne
typedef enum { ENTIER, STRING, ALL, RIEN } Type_donnees;

// Structure pour représenter une cellule de données
struct donnee_cel {
    Type_donnees type_don; // Le type de la cellule actuelle
    int val;               // Valeur entière (si le type est ENTIER)
    char *string;          // Chaîne de caractères (si le type est STRING)
    struct donnee_cel *suiv; // La donnée suivante de la ligne
};
typedef struct donnee_cel donnee_cel_t;

// Structure pour représenter une liste de lignes
struct list_ligne {
    donnee_cel_t *tete;   // Tête de la ligne actuelle
    donnee_cel_t *queue;  // Queue de la ligne actuelle
    struct list_ligne *suiv; // La ligne suivante
};
typedef struct list_ligne list_ligne_t;

// Structure pour représenter une cellule de colonne (tête de colonne)
struct cel_colonne_tete {
    Type_donnees type_don; // Le type de la cellule actuelle
    int pos;               // Ordre de la colonne par rapport à la table
    char *nom_col;         // Nom de la colonne
    struct cel_colonne_tete *suiv; // La cellule de colonne suivante
};
typedef struct cel_colonne_tete cel_colonne_tete_t;

// Structure pour représenter une table
struct table {
    char *nom_table;          // Nom de la table
    int nb_arg;               // Nombre d'arguments
    int primarykey;           // Clé primaire
    cel_colonne_tete_t *tete_col; // Tête de la liste des colonnes
    list_ligne_t *tete_ligne; // Tête de la liste de lignes qui constitue la table
    list_ligne_t *queue_ligne; // Queue de la liste de lignes qui constitue la table
};
typedef struct table table_t;

// Structure pour représenter une affectation de cellule
struct affectation_cel {
    Ast arbre;           // Arbre de calcul actuel
    char *chaine_aff;    // Chaîne d'affectation
    int num_att_aff;     // Numéro de l'attribut à affecter
    struct affectation_cel *suiv; // La donnée suivante de la ligne
};
typedef struct affectation_cel affectation_cel_t;

#endif