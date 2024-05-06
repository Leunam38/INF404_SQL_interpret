#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "analyse_lexicale.h"
#include "table_affectation.h"
#include "table_donnes.h"
#include "type_ast.h"

// Fonction pour gérer une erreur de comparaison entre deux types différents
void erreur_comparaison_entre_2_type_diff(){
    printf("Erreur: Comparaison entre 2 éléments de type différents\n");
    exit(1);
}

// Fonction qui retourne vrai si le lexème est un comparateur
int est_comparateur(Lexeme l)  {
    switch (l.nature) {
        case SUP:
        case INF:
        case INFEGAL:
        case SUPEGAL: 
        case DIFF:
        case EGAL:
            return 1;
        default:
            return 0;
    } 
}

// Fonction qui transforme une nature de lexème en type de comparateur
TypeComparateur tranfo_nature_TypeComp(Nature_Lexeme nat)  {
    switch (nat) {
        case SUP: return N_SUP;
        case INF: return N_INF;
        case INFEGAL: return N_INFEGAL;
        case SUPEGAL: return N_SUPEGAL;
        case DIFF: return N_DIFF;
        case EGAL: return N_EGAL;
        default:
            printf("Erreur: nature non valide pour comparateur\n");
            exit(1);
    } 
} 

// Fonction qui transforme une nature de lexème en type de comparateur
TypeOperateur tranfo_nature_TypeOperateur(Nature_Lexeme nature){
	switch(nature)	{
		case PLUS : return N_PLUS;
		case MOINS : return N_MOINS;
		case ETOILE: return N_MUL;
		default: printf("Erreur: nature non valide pour operateur"); exit(1) ; 
	}
}

// Vérifie si le type de donnée d'un attribut est une chaîne de caractères et la renvoie
char* est_type_str(list_ligne_t* lst_lig, Ast A){
    if (A->nature==CHAINESTR){ return A->chainestr;}
    if (A->nature==ATTRIBUT){
        int i=0;
        donnee_cel_t* cel_donn = lst_lig->tete;
        while (cel_donn!=NULL && i<A->num_attribut){
            cel_donn = cel_donn->suiv;
            i++;
        }
        if (cel_donn==NULL ){
            printf("Erreur: Attribut avec le mauvais numero: %d\n",A->num_attribut);
            exit(1);
        }
        if (cel_donn->type_don==STRING){
            return cel_donn->string;
        }
        return NULL;
    }
    return NULL;
}

// Évaluation d'une expression mathématique suivant la ligne donnée
int evaluation(Ast expr,list_ligne_t* lst_lig) {
	switch(expr->nature)	{
		//C'est une operation
        case OPERATION:
            switch (expr->operateur){
                case N_MUL:
                    return evaluation(expr->gauche,lst_lig)*evaluation(expr->droite,lst_lig);
        
                case N_MOINS:
                    return evaluation(expr->gauche,lst_lig)-evaluation(expr->droite,lst_lig);

                case N_PLUS:
                    return evaluation(expr->gauche,lst_lig)+evaluation(expr->droite,lst_lig);

                default:
                    exit(1);  // erreur operateur
            }

		//C'est un nombre
        case VALEUR:
            return expr->valeur; 

		//C'est un attribut
        case ATTRIBUT:
			printf(""); //Pour éviter le bug des cases
			//Parcours pour aller chercher la bonne valeur
            int i=0;
            donnee_cel_t* cel_donn = lst_lig->tete;
            while (cel_donn!=NULL && i<expr->num_attribut){ 
                cel_donn = cel_donn->suiv;
                i++;
            }
            if (cel_donn==NULL || cel_donn->type_don!=ENTIER){
                printf("Erreur: Attribut avec le mauvais numero: %d\n",expr->num_attribut);
                exit(1);
            }
            return cel_donn->val; 
            break;
            
        default:
		    exit(2); // erreur Ast 
	}

}

// Évalue une expression logique 
int evaluation_bool(Ast expr,list_ligne_t* lst_lig) {
    int type_str=0;
	char* chaine1=NULL;
	char* chaine2=NULL;
    if (expr==NULL){return 1;}//Si il y a pas de condition

	switch(expr->nature)	{
		//C'est un opérateur booléen
        case OPBOOL:
            switch (expr->ope_bool){
                case N_AND:
                    return evaluation_bool(expr->gauche,lst_lig) && evaluation_bool(expr->droite,lst_lig);
                case N_OR:
                    return evaluation_bool(expr->gauche,lst_lig) || evaluation_bool(expr->droite,lst_lig);
            }
		//C'est une opération de comparaison
        case COMPARATEUR:
            switch(expr->comparateur){
                case N_SUP:
                    return evaluation(expr->gauche,lst_lig)>evaluation(expr->droite,lst_lig);
                case N_INF:
                    return evaluation(expr->gauche,lst_lig)<evaluation(expr->droite,lst_lig);
                case N_INFEGAL:
                    return evaluation(expr->gauche,lst_lig)<=evaluation(expr->droite,lst_lig);
                case N_SUPEGAL:
                    return evaluation(expr->gauche,lst_lig)>=evaluation(expr->droite,lst_lig);
				//Si c'est on a un comparateur qui est différent ou égal, alors on doit aussi vérifier pour les string
                case N_DIFF:
                    chaine1 =est_type_str(lst_lig, expr->gauche);
                    chaine2 =est_type_str(lst_lig, expr->droite);
                    if (chaine1==NULL && chaine2==NULL){ //à droite et à gauche il n'y a pas de string
                        return evaluation(expr->gauche,lst_lig)!=evaluation(expr->droite,lst_lig);
                    }
                    else if (chaine1!=NULL && chaine2!=NULL){ //à droite et à gauche il y a des string
                        return strcmp(chaine1,chaine2)!=0;
                    }
                    erreur_comparaison_entre_2_type_diff();//Un côté a un string l'autre non
                    break;
                case N_EGAL:
                    chaine1 =est_type_str(lst_lig, expr->gauche);
                    chaine2 =est_type_str(lst_lig, expr->droite);
                    if (chaine1==NULL && chaine2==NULL){ //à droite et à gauche il n'y a pas de string
                        return evaluation(expr->gauche,lst_lig)==evaluation(expr->droite,lst_lig);
                    }
                    else if (chaine1!=NULL && chaine2!=NULL){ //à droite et à gauche il y a des string
                        return strcmp(chaine1,chaine2)==0;
                    }
                    erreur_comparaison_entre_2_type_diff();//Un côté a un string l'autre non
                    break;
            }
            break;
	    default:
		    exit(2); // erreur Ast 
	}
    return 0;

}

// Afficher un opérateur donné
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

// Afficher un opérateur donné
void aff_operateurBool(TypeOperateurBool op){
	switch (op) {
		case N_AND:
			printf(" AND ") ;
			break;
		case N_OR:
			printf(" OR ") ;
			break;
	} 
}

// Afficher un opérateur donné 
void aff_comparateur(TypeComparateur op){
	switch (op) {
		case N_SUP:
			printf(">") ;
			break;
		case N_INF:
			printf("<") ;
			break;
		case N_INFEGAL:
			printf("<=") ;
			break;
		case N_SUPEGAL:
			printf(">=") ;
			break;
		case N_DIFF:
			printf("!=") ;
			break;
		case N_EGAL:
			printf("=") ;
			break;
	}
} 

// Afficher l'arbre abstrait
void afficherA(Ast expr) {
	switch (expr->nature) {
		// Si le noeud est une operation, afficher le calcul parenthéser
		case OPERATION:
			printf("(");
			afficherA(expr->gauche);
			aff_operateur(expr->operateur) ;
			afficherA(expr->droite);
			printf(")");
			break ;

		// Si le noeud est un comparateur, afficher l'exepression logique parenthéser
		case COMPARATEUR:
			printf("(");
			afficherA(expr->gauche);
			aff_comparateur(expr->comparateur) ;
			afficherA(expr->droite);
			printf(")");
			break ;
			
		// Si le noeud est une operation booléen, afficher l'exepression logique parenthéser
		case OPBOOL:
			printf("(");
			afficherA(expr->gauche);
			aff_operateurBool(expr->ope_bool) ;
			afficherA(expr->droite);
			printf(")");
			break ;

		// Si le noeud est une valeur, afficher simplement sa valeur
		case VALEUR:
			printf("%d", expr->valeur);
			break ;

		// Si le noeud est une valeur, afficher simplement sa valeur
		case CHAINESTR:
			printf("'%s'", expr->chainestr);
			break ;
        
		// Si le noeud est un attribut, afficher son numéro d'attribut
		case ATTRIBUT:
            printf("Att:%d", expr->num_attribut);
			break ;
   }
}
