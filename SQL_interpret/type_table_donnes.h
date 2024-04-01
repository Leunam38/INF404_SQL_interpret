#ifndef __TYPE_TAB__
#define __TYPE_TAB__ 

//Type énuméré pour savoir quel type de donnée on stock dans notre colonne
typedef enum {ENTIER,DATE,STRING,RIEN} Type_donnees ;

struct donnee_cel {
    Type_donnees type_don; //Le type de la cellule actuelle
    int val;
    char* string;
    //Rajouter ici date?
    struct donnee_cel *suiv; //La donnée suivante de la ligne
};
typedef struct donnee_cel donnee_cel_t;

//Structure pour une liste de ligne
struct list_ligne{
    donnee_cel_t * tete; //Tete de la ligne actuelle
    donnee_cel_t * queue; //Queue de la ligne actuelle
    struct list_ligne *suiv; //La ligne suivante
};
typedef struct list_ligne list_ligne_t;

struct cel_colonne_tete{
    Type_donnees type_don; //Le type de la cellule actuelle
    int pos; // ordre pour de où ils sont par rapport à la table
    char* nom_col;
    //Rajouter ici date?
    struct cel_colonne_tete *suiv; //La donnée suivante de la ligne
};
typedef struct cel_colonne_tete cel_colonne_tete_t;

//Structure pour avoir une table
struct table
{   
    char* nom_table; //Nom de la table
    int nb_arg; //Nombre d'argument
    cel_colonne_tete_t * tete_col; //Vers la liste des colonnes
    list_ligne_t *tete_ligne; //Tête de la liste de lignes qui constitue la table
};
typedef struct table table_t;


#endif