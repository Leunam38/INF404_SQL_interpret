
#ifndef __TYPE_AST__
#define __TYPE_AST__ 

typedef enum {OPERATION, VALEUR, ATTRIBUT} TypeAst ;
typedef enum {N_PLUS, N_MUL, N_MOINS} TypeOperateur; 

typedef struct noeud { 
	TypeAst nature ;
	TypeOperateur operateur ;
	struct noeud *gauche, *droite ;
    int valeur ;
	int num_attribut;
} NoeudAst ;

typedef NoeudAst *Ast ;

#endif  
