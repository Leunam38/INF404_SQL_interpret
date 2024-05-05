
#ifndef __TYPE_AST__
#define __TYPE_AST__ 

typedef enum {OPERATION, VALEUR, ATTRIBUT} TypeAst ; //type enumere de l'ast
typedef enum {N_PLUS, N_MUL, N_MOINS} TypeOperateur;  //type de l'operateur

typedef struct noeud { 
	TypeAst nature ; //nature actuelle
	TypeOperateur operateur ; //operateur actuel
	struct noeud *gauche, *droite ; //fils
    int valeur ; // valeur actuelle
	int num_attribut; //num
} NoeudAst ;

typedef NoeudAst *Ast ;

#endif  
