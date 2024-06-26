/* ------------------------------------------------------------------------
-- paquetage machine_lexemes
--
-- analyse lexicale d'une expression arithmetique
--
-- P.Habraken - 18/01/2010
--
-- 2 aout 2006 - PH :
-- remplacement record a champs variants par record "plat"
------------------------------------------------------------------------ */

#ifndef __ANA_LEX__
#define __ANA_LEX__
#include "lecture_caracteres.h"

//Nature des caractères
typedef enum {
      ERREUR_CAR,   // caractere incorrect
      CHIFFRE,			// chiffre 
      LETTRE,       //lettre
      VIR,         //virgule
      PNT_VIR,    //point virgule
      TIRET,    //tiret (du 6 ou du 8)
      SYMBOLE,	// symbole,
      GUILLEMET, 
      C_FIN_SEQUENCE   // caractere de fin de sequence     
} Nature_Caractere ;

//Nature des lexemes
typedef enum { 
   CREATE,
   TABLE,
   TEXT,
   INTEGER, //entier

   CONSTRAINT,
   PRIMARY,
   KEY,      

   SELECT,
   FROM,
   WHERE,
   
   INSERT,
   INTO,
   VALUES,

   UPDATE,
   SET,

   VIRGULE, //,
   ETOILE,  //*

   VAR, //Variable
   AS,
   PARO,    //(
   PARF,    //)
   CHAINE,

   STR, //un string "nom"
   NOMBRE, //un nombre entier
   SUP,     //<
   INF,     //>
   INFEGAL, //>=
   SUPEGAL, //<=
   DIFF,    //!=
   EGAL,    //=
   AND,    //&&
   OR,    // ||
   PLUS,    //+
   MOINS,   //-

   SEPINST,
   FIN_SEQUENCE
   } Nature_Lexeme ;

// Structure représentant un lexème
typedef struct { 
   Nature_Lexeme nature;    // nature du lexeme
   unsigned int ligne;           // numero de ligne
   unsigned int colonne;         // numero de colonne
   char chaine[256];           // chaine de caracteres
   int val;                   //Valeur du lexeme
} Lexeme ; 


// Prototypes des fonctions
void afficher_lex(Lexeme l);

void demarrer(char *nom_fichier);
// e.i. : indifferent
// e.f. : la sequence de lexemes est lue dans le fichier designe par
//        nom_fichier
//        fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
//        (non fin_de_sequence) => lexeme courant est le premier
//        lexeme de la sequence
//        l'exception Erreur_Lexicale est levee en cas d'erreur

void avancer();
// pre-condition : la machine sequentielle est demarree
// e.i. : on pose i = rang du lexeme_courant :
//        (non fin_de_sequence)
//        et (non lexeme_courant.nature = FIN_SEQUENCE)
// e.f. : fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
//        (non fin_de_sequence) => lexeme_courant est le lexeme i+1
//        l'exception Erreur_Lexicale est levee en cas d'erreur

Lexeme lexeme_courant();
// pre-condition : la machine sequentielle est demarree
// lexeme_courant est :
// . le pseudo lexeme FIN_SEQUENCE si fin_de_sequence
// . le pseudo lexeme ERREUR si une erreur a ete detectee
// . le lexeme de rang i sinon

int fin_de_sequence();
// pre-condition : la machine sequentielle est demarree
// fin_de_sequence vaut :
// . vrai si tous les lexemes de la sequence ont ete reconnus
// . faux sinon

void arreter();
// e.i. : la machine sequentielle est demarree
// e.f. : la machine sequentielle est arreteeù

void ajouter_caractere (char *s, char c); //ajoute un caractere a la chaine s
Nature_Caractere nature_caractere (char c); //determine la nature du caractere
int est_separateur(char c ) ; //determine les separateur dans une chaine, comme un espace
int est_chiffre(char c ) ; //determine si un caractere est entre 0 et 9

int est_guillemet(char c); //determine si on a un guillemet
int est_symbole(char c ) ; //determine si on a un symbole operatoire
void reconnaitre_lexeme(); //reconnait le mmot pour l'automate
char *Nature_vers_Chaine (Nature_Lexeme nature) ; //conversion d'une nature vers une chaine 

#endif