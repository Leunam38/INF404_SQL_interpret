
#ifndef __TYPE_AST__
#define __TYPE_AST__ 

// Type énuméré pour les différentes natures d'un noeud dans l'Ast.
typedef enum {OPERATION, VALEUR, CHAINESTR ,ATTRIBUT,COMPARATEUR, OPBOOL} TypeAst;

// Type énuméré pour les différents opérateurs dans l'Ast.
typedef enum {N_PLUS, N_MUL, N_MOINS} TypeOperateur;

// Type énuméré pour les différents comparateur dans l'Ast.
typedef enum {N_SUP,N_INF,N_INFEGAL,N_SUPEGAL,N_DIFF,N_EGAL} TypeComparateur;

// Type énuméré pour les différents comparateur dans l'Ast.
typedef enum {N_AND,N_OR} TypeOperateurBool;

// Structure d'un noeud de l'Ast.
typedef struct noeud {
	TypeAst nature;             // Nature du noeud (opération, valeur ou attribut).
	TypeOperateur operateur;   // Si le noeud est une opération, quel est l'opérateur.
	TypeComparateur comparateur;
	TypeOperateurBool ope_bool;
	struct noeud *gauche, *droite;  // Pointeurs vers les noeuds enfants (gauche et droit).
	int valeur;                // La valeur du noeud s'il est de type VALEUR.
	char* chainestr;                // La chaine string
	int num_attribut;          // Le numéro de l'attribut s'il est de type ATTRIBUT.
} NoeudAst;

typedef NoeudAst *Ast ;

#endif  
