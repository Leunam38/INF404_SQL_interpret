/* ------------------------------------------------------------------------
-- paquetage analyse_lexicale
--
-- analyse lexicale d'une expression arithmetique
--
-- J-P. Peyrin, G. Serasset (version initiale) : janvier 1999
-- P.Habraken - 18/01/2010
--
-- 10 juin 2006 - PH :
-- remplacement record a champs variants par record "plat"
-- remplacement traitement iteratif par automate
--
-- Aout 2016 - LM : version C
------------------------------------------------------------------------ */

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "lecture_caracteres.h"
#include "analyse_lexicale.h"

   /* --------------------------------------------------------------------- */


/* Les variables et fonctions locales au module */

   Lexeme lexeme_en_cours ;	/* le lexeme courant */

   void ajouter_caractere (char *s, char c);
   Nature_Caractere nature_caractere (char c);
   int est_separateur(char c ) ;
   int est_chiffre(char c ) ;
   int est_point(char c) ;
   int est_vir(char c) ;
   int est_pnt_vir(char c);
   int est_tiret(char c);
   int est_symbole(char c ) ;
   int est_lettre(char c ) ;
   void reconnaitre_lexeme();

   /* --------------------------------------------------------------------- */

   void demarrer(char *nom_fichier) {
      demarrer_car(nom_fichier);
      avancer();
   }

   /* --------------------------------------------------------------------- */

   void avancer() {
      reconnaitre_lexeme();
   } 

   /* --------------------------------------------------------------------- */

   Lexeme lexeme_courant() {
      return lexeme_en_cours;
   } 

   /* --------------------------------------------------------------------- */

   int fin_de_sequence() {
      return lexeme_en_cours.nature == FIN_SEQUENCE;
   }

   /* --------------------------------------------------------------------- */

   void arreter() {
      arreter_car() ;
   }

   /* --------------------------------------------------------------------- */

   void tranfo_chaine_nature() {
      if (strcmp(lexeme_en_cours.chaine,"CREATE")==0 || strcmp(lexeme_en_cours.chaine,"create")==0 ){
         lexeme_en_cours.nature=CREATE;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"TABLE")==0){
         lexeme_en_cours.nature=TABLE;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"INTEGER")==0 || strcmp(lexeme_en_cours.chaine,"integer")==0){
         lexeme_en_cours.nature=INTEGER;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"TEXT")==0 || strcmp(lexeme_en_cours.chaine,"text")==0){
         lexeme_en_cours.nature=TEXT;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"CONSTRAINT")==0 || strcmp(lexeme_en_cours.chaine,"constraint")==0){
         lexeme_en_cours.nature=CONSTRAINT;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"PRIMARY")==0 || strcmp(lexeme_en_cours.chaine,"primary")==0){
         lexeme_en_cours.nature=PRIMARY;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"KEY")==0 || strcmp(lexeme_en_cours.chaine,"key")==0){
         lexeme_en_cours.nature=KEY;
         return;
      }
      
      if (strcmp(lexeme_en_cours.chaine,"SELECT")==0 || strcmp(lexeme_en_cours.chaine,"select")==0){
         lexeme_en_cours.nature=SELECT;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"FROM")==0 || strcmp(lexeme_en_cours.chaine,"from")==0){
         lexeme_en_cours.nature=FROM;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"WHERE")==0 || strcmp(lexeme_en_cours.chaine,"where")==0){
         lexeme_en_cours.nature=WHERE;
         return;
      }
      // if (strcmp(lexeme_en_cours.chaine,"IF")==0 || strcmp(lexeme_en_cours.chaine,"if")==0){
      //    lexeme_en_cours.nature=IF;
      //    return;
      // }
      // if (strcmp(lexeme_en_cours.chaine,"EXIST")==0 || strcmp(lexeme_en_cours.chaine,"exist")==0){
      //    lexeme_en_cours.nature=EXIST;
      //    return;
      // }
      if (strcmp(lexeme_en_cours.chaine,"DISTINCT")==0 || strcmp(lexeme_en_cours.chaine,"distinct")==0){
         lexeme_en_cours.nature=DISTINCT;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"AS")==0 || strcmp(lexeme_en_cours.chaine,"as")==0){
         lexeme_en_cours.nature=AS;
         return;
      }
      
      if (strcmp(lexeme_en_cours.chaine,"INSERT")==0 || strcmp(lexeme_en_cours.chaine,"insert")==0){
         lexeme_en_cours.nature=INSERT;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"INTO")==0 || strcmp(lexeme_en_cours.chaine,"into")==0){
         lexeme_en_cours.nature=INTO;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"VALUES")==0 || strcmp(lexeme_en_cours.chaine,"values")==0){
         lexeme_en_cours.nature=VALUES;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"UPDATE")==0 || strcmp(lexeme_en_cours.chaine,"update")==0){
         lexeme_en_cours.nature=UPDATE;
         return;
      }
      if (strcmp(lexeme_en_cours.chaine,"SET")==0 || strcmp(lexeme_en_cours.chaine,"set")==0){
         lexeme_en_cours.nature=SET;
         return;
      }
      
      
      lexeme_en_cours.nature=VAR;
      return;
   }

   /* --------------------------------------------------------------------- */

   // reconnaissance d'un nouveau lexeme
   // etat initial : le caractere courant est soit separateur 
   //                soit le 1er caractere d'un lexeme
   // etat final : 
   //       - un nouveau lexeme est reconnu dans lexeme_en_cours
   //       - le caractere courant est soit la fin de fichier, 
   //		soit un separateur,  soit le 1er caractere d'un lexeme

void reconnaitre_lexeme() {
   typedef enum {E_INIT, E_NOMBRE, E_CHAINE, E_SUP, E_INF,E_FIN} Etat_Automate ;
   Etat_Automate etat=E_INIT;

   // on commence par lire et ignorer les separateurs
   while (est_separateur(caractere_courant())) {
      avancer_car() ;
   } ;

   lexeme_en_cours.chaine[0] = '\0' ;

   // on utilise ensuite un automate pour reconnaitre et construire le prochain lexeme

   while (etat != E_FIN) {

	   switch (etat) {

		   case E_INIT: // etat initial

            switch(nature_caractere(caractere_courant())) {
               
               //Si fin de séquence
               case C_FIN_SEQUENCE:
                  lexeme_en_cours.nature = FIN_SEQUENCE;
                  etat = E_FIN;
                  break ;

               //Si point
               case PNT:
                  lexeme_en_cours.nature = POINT;
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  etat = E_FIN;
                  avancer_car() ;
                  break ;

               //Si virgule
               case VIR:
                  lexeme_en_cours.nature = VIRGULE;
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  etat = E_FIN;
                  avancer_car() ;
                  break ;
               
               //Si point_virgule
               case PNT_VIR:
                  lexeme_en_cours.nature = SEPINST;
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  etat = E_FIN;
                  avancer_car() ;
                  break;

               //Si c'est un chiffre
               case CHIFFRE: 
                  lexeme_en_cours.nature = NOMBRE;
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  lexeme_en_cours.val = caractere_courant() - '0';
                  etat = E_NOMBRE;
                  avancer_car() ;
                  break ;

               //Si c'est un symbole
               case SYMBOLE: 
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  switch (caractere_courant()) {
                     case '+':
                        lexeme_en_cours.nature = PLUS;
                        etat = E_FIN;
                        break;

                     case '-':
                        lexeme_en_cours.nature = MOINS;
                        etat = E_FIN;
                        break;

                     case '*':
                        lexeme_en_cours.nature = ETOILE;
                        etat = E_FIN;
                        break;

                     case '<':
                        lexeme_en_cours.nature = INF;
                        etat = E_INF;
                        break;
                        
                     case '>':
                        lexeme_en_cours.nature = SUP;
                        etat = E_SUP;
                        break;

                     case '=':
                        lexeme_en_cours.nature = EGAL;
                        etat = E_FIN;
                        break;
                        
                     case '!':
                        avancer_car() ;
                        if (caractere_courant()!='='){
                           printf("Erreur_Lexicale") ;
                           exit(0) ;
                        }
                        ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                        lexeme_en_cours.nature = DIFF;
                        etat = E_FIN;
                        break;

                     case '(':
                        lexeme_en_cours.nature = PARO;
                        etat = E_FIN;
                        break;

                     case ')':
                        lexeme_en_cours.nature = PARF;
                        etat = E_FIN;
                        break;

                     case '"':
                        lexeme_en_cours.nature = GUILLEMETS;
                        etat = E_FIN;
                        break;

                     default:
                        printf("Erreur_Lexicale") ;
                        exit(0) ;
                        break;
                  }

                  avancer_car() ;
                  break;

               //Si c'est une lettre
               case LETTRE:
                  lexeme_en_cours.nature = CHAINE;
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  etat = E_CHAINE;
                  avancer_car() ;
                  break ;
               
               default:
                  printf("Erreur_Lexicale\n") ;
                  exit(0);
                  break;
            } 
		      break;

         case E_INF:  // reconnaissance d'un nombre
            if (caractere_courant()=='='){
               lexeme_en_cours.nature = INFEGAL;
               ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
               avancer_car() ;
            }
            etat = E_FIN;
            break;
            
         case E_SUP:  // reconnaissance d'un nombre
            if (caractere_courant()=='='){
               lexeme_en_cours.nature = SUPEGAL;
               ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
               avancer_car() ;
            }
            etat = E_FIN;
            break;

         case E_NOMBRE:  // reconnaissance d'un nombre
            switch(nature_caractere(caractere_courant())) {
               case CHIFFRE:
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  lexeme_en_cours.val = lexeme_en_cours.val * 10 + caractere_courant() - '0';
                  etat = E_NOMBRE;
                  avancer_car ();
                  break ;

               default:
                  etat = E_FIN;
                  break;
            }
            break;


         case E_CHAINE:  // reconnaissance d'un nombre
            switch(nature_caractere(caractere_courant())) {
               case TIRET:
               case LETTRE:
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  avancer_car ();
                  break;
                  
               case CHIFFRE:
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  avancer_car ();
                  break;

               default:
                  etat = E_FIN;
                  break;
            }
            break;

         case E_FIN:  // etat final
         break ;
	    
	   } // fin du switch(etat)
	} // fin du while (etat != fin)

   if (lexeme_en_cours.nature==CHAINE){
      tranfo_chaine_nature();
      if (lexeme_en_cours.nature==CHAINE){
         printf("Erreur_Lexicale: Chaine non défini\n") ;
         exit(0);
      }
   }

}

/* --------------------------------------------------------------------- */

// cette fonction ajoute le caractere c a la fin de la chaine s 
// (la chaine s est donc modifiee)

void ajouter_caractere (char *s, char c) {
int l ;

l = strlen(s) ;
s[l] = c ;
s[l+1] = '\0' ;
} ;

/* --------------------------------------------------------------------- */

Nature_Caractere nature_caractere (char c) {
   if (fin_de_sequence_car(c)) return C_FIN_SEQUENCE;
   if (est_chiffre(c)) return CHIFFRE;
   if (est_point(c)) return PNT;
   if (est_vir(c)) return VIR;
   if (est_pnt_vir(c)) return PNT_VIR;
   if (est_tiret(c)) return TIRET;
   if (est_symbole(c)) return SYMBOLE;
   if (est_lettre(c)) return LETTRE;
   return ERREUR_CAR ;
}
/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere separateur
int est_separateur(char c) { 
   return c == ' ' || c == '\t' || c == '\n';
}

/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere chiffre
int est_chiffre(char c) {
   return c >= '0' && c <= '9' ;
}

int est_point(char c) {
   return c=='.';
}

int est_vir(char c) {
   return c==',';
}

int est_pnt_vir(char c) {
   return c==';';
}

int est_tiret(char c) {
   return c== '_';
}

/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere correspondant a un symbole arithmetique
int est_symbole(char c)  {
   switch (c) {
      case '+':  
      case '-':  
      case '*':
      case '=':
      case '<':
      case '>':
      case '!':  
      case '(':
      case ')':
      case '"':
         return 1;

      default:
         return 0;
   } 
}

/* --------------------------------------------------------------------- */

// vaut vrai ssi c désigne un caractere qui est une lettre
int est_lettre(char c ) {
   if (('a'<=c && c<='z') || ('A'<=c && c<='Z')){
      return 1;
   }
   return 0;
}

/* --------------------------------------------------------------------- */

// renvoie la chaine de caracteres correspondant a la nature du lexeme
char *Nature_vers_Chaine (Nature_Lexeme nature) {
switch (nature) {  
   case CREATE: return "CREATE" ;   
   case TABLE: return "TABLE" ;
   case TEXT: return "TEXT" ;
   case INTEGER: return "INTEGER" ;  
   case CONSTRAINT: return "CONSTRAINT" ;
   case PRIMARY: return "PRIMARY" ;
   case KEY: return "KEY" ;  

   case VAR: return "VAR" ;            
   case SEPINST: return "SEPINST" ; //Fin ; 
   case VIRGULE: return "VIRGULE" ; 
   case POINT: return "POINT" ;    
   case AS: return "AS" ;                  
   case SELECT: return "SELECT" ;            
   case FROM: return "FROM" ;                
   case DISTINCT: return "DISTINCT" ;        
   case WHERE: return "WHERE" ;        
   case ETOILE: return "ETOILE" ;              
   // case AS: return "AS" ;                
   // case IF: return "IF" ;            
   // case EXIST: return "EXIST" ;
   case NOMBRE: return "NOMBRE" ;
   case EGAL: return "EGAL" ;
   case DIFF: return "DIFF" ;
   case INF: return "INF" ;
   case SUP: return "SUP" ;
   case INFEGAL: return "INFEGAL" ;
   case SUPEGAL: return "SUPEGAL" ;
   case PLUS: return "PLUS" ;
   case MOINS: return "MOINS" ;  
   case PARO: return "PARO" ; 
   case PARF: return "PARF";  
   case GUILLEMETS: return "GUILLEMETS";
   case CHAINE: return "CHAINE";   
   case INSERT: return "INSERT";
   case INTO: return "INTO";   
   case VALUES: return "VALUES" ;   
   case UPDATE: return "UPDATE";   
   case SET: return "SET" ;   
   case FIN_SEQUENCE: return "FIN_SEQUENCE" ;     
   default: return "ERREUR" ;            
} ;
} 

/* --------------------------------------------------------------------- */

// affiche a l'ecran le lexeme l
/**/
void afficher_lex(Lexeme l) {

   switch (l.nature) {
      case FIN_SEQUENCE: 
         break;
      default:
         printf("(ligne %d, ", l.ligne);
         printf("colonne %d) : ",l.colonne);
         printf("[") ;
         printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
         printf(", chaine = %s, ", l.chaine) ;
         switch(l.nature) {
               case NOMBRE:
                     printf("valeur = %d", l.val);
                     break;
               default:
                     break;
         }
      printf("]") ;
   }
}