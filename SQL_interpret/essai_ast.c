#include <stdio.h> 

#include "ast_construction.h"
#include "ast_parcours.h"

int main() {

 Ast ast1, ast2, ast3, ast4, ast5, ast6, ast ;
 Ast t1,t2,t3,t4,t5,t;
 Ast g1,g2,g;
 Ast h1,h2,h;
 Ast i1,i;

 ast1  = creer_valeur(50) ;
 ast2  = creer_valeur(14) ;
 ast3 = creer_operation(N_MOINS, ast1, ast2) ;
 ast4  = creer_valeur(3) ;
 ast5 = creer_operation(N_PLUS, ast3, ast4) ;
 ast6  = creer_valeur(6) ;
 ast = creer_operation(N_MUL, ast6, ast5) ;

 t1 = creer_valeur(2);
 t2 = creer_valeur(3);
 t3 = creer_valeur(5);
 t4 = creer_operation(N_MUL,t2,t3);
 t5 = creer_operation(N_PLUS,t1,t4);
 t = creer_operation(N_MOINS,t5,t1);

 g1 = creer_operation(N_PLUS,t1,t2);
 g2 = creer_operation(N_MOINS,t3,t1);
 g = creer_operation(N_MUL,g1,g2);

 h1 = creer_operation(N_MOINS,t3,t1);
 h2 = creer_operation(N_PLUS,t2,h1);
 h = creer_operation(N_PLUS,t1,h2);

 i1 = creer_operation(N_MOINS, creer_operation(N_MOINS,t3,t1),t2);
 i = creer_operation(N_MOINS, t2, i1);


 printf("Arbre abstrait de l'expression\n") ;
 afficher(ast) ; 
 printf("\n\nValeur de l'expression : %f\n", evaluation(ast)) ;
 afficher(t) ; 
 printf("\n\nValeur de l'expression : %f\n", evaluation(t)) ;
 afficher(g) ; 
 printf("\n\nValeur de l'expression : %f\n", evaluation(g)) ;
 afficher(h) ; 
 printf("\n\nValeur de l'expression : %f\n", evaluation(h)) ;
 afficher(i) ; 
 printf("\n\nValeur de l'expression : %f\n", evaluation(i)) ;
 return 0 ;

}
