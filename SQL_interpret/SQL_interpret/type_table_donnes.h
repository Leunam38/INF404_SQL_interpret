#ifndef __TYPE_TAB__
#define __TYPE_TAB__ 

//Type énuméré pour savoir quel type de donnée on stock dans notre colonne
typedef enum {ENTIER,DATE,STRING,RIEN} Type_donnees ;

//Structure pour les données qui sont des entiers
struct donnee_int {
    int val; //La valeur de l'entier
    struct donnee_int *suiv; //La donnée suivante
};
typedef struct donnee_int donnee_int_t;

//Structure pour les colonnes qui stock des entiers
struct colonne_int {   
    char* nom_col; //Nom de la colonne
    int taille_col; //Taille de la colonne
    donnee_int_t *tete_col; //Tête de la colonne qui est une donnée d'un entier
};
typedef struct colonne_int colonne_int_t;

//Structure pour les données qui sont des string
struct donnee_str {
    char* valeur; //La valeur du string
    struct donnee_str *suiv; //La donnée suivante
};
typedef struct donnee_str donnee_str_t;

//Structure pour les colonnes qui stock des string
struct colonne_str {   
    char* nom_col; //Nom de la colonne
    int taille_col;//Taille de la colonne
    donnee_str_t *tete_col;  //Tête de la colonne qui est une donnée d'un string
};
typedef struct colonne_str colonne_str_t;

//Structure pour une liste de colonne
struct list_col{
    Type_donnees type_don; //Le type de la colonne actuelle
    colonne_int_t *col_int; //Pointeur vers une colonne d'entier, utiliser si type_don=ENTIER
    colonne_str_t *col_str; //Pointeur vers une colonne de string, utiliser si type_don=STRING
    struct list_col *suiv; //La colonne suivante
};
typedef struct list_col list_col_t;

//Structure pour avoir une table
struct table
{   
    char* nom_table; //Nom de la table
    int nb_arg; //Nombre d'argument
    list_col_t *prm_col; //Tête de la liste de colonne qui constitue la table
};
typedef struct table table_t;

#endif  
