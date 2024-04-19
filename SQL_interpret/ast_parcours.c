#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include "type_ast.h"


void aff_operateur(TypeOperateur op){
	switch (op) {
		case N_PLUS:
			printf(" + ") ;
			break;
		case N_MOINS:
			printf(" - ") ;
			break;
		case N_MUL:
			printf(" * ") ;
			break;
	} 
}

void afficherA(Ast expr) {
	switch (expr->nature) {
      case OPERATION:
         printf("(");
         afficherA(expr->gauche);
         aff_operateur(expr->operateur) ;
         afficherA(expr->droite);
         printf(")");
      break ;
      case VALEUR:
         printf("%d", expr->valeur);
      break ;
	  case ATTRIBUT:
         printf("Att:%d", expr->num_attribut);
      break ;
   }
}

int evaluation(Ast expr) {
    int denominateur ;
	switch(expr->nature)
	{
	  case OPERATION:
		switch (expr->operateur)
		{

			case N_MUL:
			  return evaluation(expr->gauche)*evaluation(expr->droite);
	
			case N_MOINS:
			  return evaluation(expr->gauche)-evaluation(expr->droite);

			case N_PLUS:
			  return evaluation(expr->gauche)+evaluation(expr->droite);			
			default:
			  exit(1);  // erreur operateur
		}

	   case VALEUR:
		return expr->valeur; 

		case ATTRIBUT:
		printf("à changer !!!!!!!!!!");
		return expr->valeur; 
		
	   default:
		exit(2); // erreur Ast 
	}

}