#ifndef __TYPE_TAB__
#define __TYPE_TAB__ 

typedef enum {ENTIER,DATE,STRING,RIEN} Type_donnees ;

struct donnee_int {
    int val;
    struct donnee_int *suiv;
};
typedef struct donnee_int donnee_int_t;

struct colonne_int {   
    char* nom_col;
    donnee_int_t *tete_col;
    int taille_col;
};
typedef struct colonne_int colonne_int_t;

struct donnee_str {
    char* valeur;
    struct donnee_str *suiv;
};
typedef struct donnee_str donnee_str_t;

struct colonne_str {   
    char* nom_col;
    donnee_str_t *tete_col;
    int taille_col;
};
typedef struct colonne_str colonne_str_t;

struct list_col
{
    Type_donnees type_don;
    colonne_int_t *col_int;
    colonne_str_t *col_str;
    struct list_col *suiv;
};

typedef struct list_col list_col_t;

struct table
{   
    char* nom_table;
    int nb_arg;
    list_col_t *prm_col;
};

typedef struct table table_t;

// typedef struct noeud { 
//     char * nom_table
// 	TypeOperateur operateur ;
// 	struct noeud *droite ;
//     int nb_col; //nombre de colonne
// } NoeudAst ;

// typedef NoeudAst *Ast ;

#endif  
