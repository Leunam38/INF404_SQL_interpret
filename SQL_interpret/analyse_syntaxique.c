#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"
#include "table_affectation.h"
#include "type_ast.h"
#include "ast_construction.h"
#include "table_donnes.h"
#include "ast_parcours.h"
#include "requete_SQL.h"

/************************************************************\
|                           ERREURS                          |
\************************************************************/
// Fonctions pour gérer les erreurs syntaxiques et autres
void erreur(){
    printf("Erreur syntaxique\n");
    exit(1);
}

void erreur_table_aff(char * nom_table){
    printf("Erreur : la table %s n'a pas été trouvé dans la base de donnée\n",nom_table);
    exit(1);
}

void erreur_attribut_non_reconnu(char * nom_attribut){
    printf("Erreur: Attribut '%s' non recconu\n",nom_attribut);
    exit(1);
}

void erreur_attribut_deja_existant(){
    printf("Erreur: attribut déjà existant dans la table\n");
    exit(1);
}

void erreur_donnee_illegale(){
    printf("Erreur: Ajout de donnée illégale\n");
    exit(1);
}

void erreur_donnee_deja_existant(int num_ligne){
    printf("Erreur: La valeur que vous voulez rajoutez existe déjà, ligne :%d\n",num_ligne);
    exit(1);
}

void erreur_colonne_inexistante(){
    printf("Erreur : Pas de colonne à quoi rajouter des données\n");
    exit(1);
}

void erreur_manque_donnee_insert(){
    printf("Erreur: Il manque des données à rajouter dans l'insert\n");
    exit(1);
}

void erreur_requete(){
    printf("Erreur syntaxique: On ne reconnait pas la requête\n");
    exit(1);
}

/************************************************************\
|                           CREATE                           |
\************************************************************/
// Déclaration de variables et initialisation de structures de données

// Fonction pour traiter la création de tables
void req_create(table_aff_t* table_aff){
    char nomtable[100]; //Le nom de la table stocker temporairement
    int nb_attribut=0;
    cel_colonne_tete_t* colonnes=(cel_colonne_tete_t*)malloc(sizeof(cel_colonne_tete_t));
    
    // Vérification des éléments syntaxiques attendus
    if (lexeme_courant().nature!=CREATE){erreur();} //mot create      
    avancer();
    if (lexeme_courant().nature!=TABLE){erreur();} //mot table        
    avancer();
    
    Lexeme LC=lexeme_courant(); 
    if (LC.nature!=VAR){erreur(); }//Nom de la table

    //On verifie que ce nom n'est pas déjà utilisé pour une table
    if (search_table_aff(table_aff,LC.chaine)!=NULL){
        printf("Erreur: La table %s existe déjà\n",LC.chaine);
        exit(1);
    }
    strcpy(nomtable,LC.chaine); //on le stocke le nom de la table temporairement
    avancer();

    if (lexeme_courant().nature!=PARO){erreur();}//Parenthèse ouvrante
    avancer();

    table_t* table = init_table(); //initialisation de la table dans la memoire 
    rec_seq_creation_attribut(table,colonnes,&nb_attribut); //recherche des attributs/contrainte
    
    if (lexeme_courant().nature!=PARF){erreur();}//Parenthèse fermante
    avancer();
    if (lexeme_courant().nature!=SEPINST){erreur();}//Point virgule        
    avancer();

    remplissage_table(table,nomtable,nb_attribut,colonnes); //on remplit la table
    // afficher_table(table); //Affichage de la table pour debug
    ajout_table_aff(table_aff,table); //on ajoute a la liste de tables la table table
}

// Fonction pour traiter une séquence d'attributs
void rec_seq_creation_attribut(table_t* table, cel_colonne_tete_t* colonnes, int* n){
    rec_creation_attribut(table,colonnes,n);
    table->tete_col=colonnes;
    rec_seq_suite_cree_attribut(table,colonnes,n);
}

// Fonction pour traiter la création d'un attribut
void rec_creation_attribut(table_t* table,cel_colonne_tete_t* colonnes,int* n){
    Lexeme LC=lexeme_courant(); /*nom de l'attribut*/
    if (LC.nature!=VAR){erreur();}

    // Vérification que l'attribut n'existe pas
    if (search_attribut(table,LC.chaine)!=-1){
        erreur_attribut_deja_existant();
    }

    colonnes->pos=*n; //affectation position
    colonnes->nom_col=strdup(LC.chaine); //on stocke le nom de l'attribut en memoire
    *n=*n+1; //on incremente n pour savoir le nb total d'attribut/suivi des positions

    avancer();
    rec_type(colonnes);//definit le type de l'attribut
}

// Fonction pour traiter le type d'un attribut
void rec_type(cel_colonne_tete_t* colonnes){
    /*type d'un attribut*/
    switch (lexeme_courant().nature){
        case TEXT:
            colonnes->type_don=STRING;
            break;
        case INTEGER:
            colonnes->type_don=ENTIER;
            break;
        default:
            erreur();
            break;
    }
    avancer();
}

// Fonction pour traiter la suite de la création d'attributs
void rec_seq_suite_cree_attribut(table_t* table,cel_colonne_tete_t* colonnes,int* n){
    Lexeme LC;
    if (lexeme_courant().nature==VIRGULE){ //si il y a une virgule
        avancer();

        LC=lexeme_courant(); //si il y a un autre attribut à enregistrer
        if (LC.nature==VAR){
            cel_colonne_tete_t* cel_suiv=init_colonne_tete(); //on passe a l'endroit suivant en memoire            
            rec_creation_attribut(table,cel_suiv,n); //on creer l'attribut
            colonnes->suiv=cel_suiv;
            rec_seq_suite_cree_attribut(table,cel_suiv,n); //on re-regarde si il y a encore quelque chose apres
            }
        else if (LC.nature==CONSTRAINT){ //si on a une contrainte
            /*ajouter ici pour les check & fk si on le fait*/
            rec_contrainte(table, colonnes); //reagrde la contrainte primary key
        }
        else { // si aucun des cas erreur
            erreur();
        }
    }
    //Sinon epsilon on ne fait rien
}

// Fonction pour traiter une contrainte (primary key)
void rec_contrainte(table_t* table, cel_colonne_tete_t* colonnes){
    if (lexeme_courant().nature!=CONSTRAINT){erreur();}//mot contrainte
    avancer();
    if (lexeme_courant().nature!=VAR){erreur();} //Nom de la primary key        
    avancer();
    if (lexeme_courant().nature!=PRIMARY){erreur();}//mot primary
    avancer();
    if (lexeme_courant().nature!=KEY){erreur();}//mot key
    avancer();
    if (lexeme_courant().nature!=PARO){erreur();}//Parenthèse ouvrante
    avancer();

    Lexeme LC=lexeme_courant();
    if (LC.nature!=VAR){erreur();}// Nom de l'attribut
        
    //On vérifie que l'attribut existe bien pas dans la table
    int res_recherche_att=search_attribut(table,LC.chaine);
    if (res_recherche_att==-1){erreur_attribut_non_reconnu(LC.chaine); }

    table->primarykey = res_recherche_att;
    avancer();

    if (lexeme_courant().nature!=PARF){erreur();}//Parenthese fermante
    avancer();

    if (lexeme_courant().nature==VIRGULE){//virgule (au-cas ou on rajoute des fk et check)
        avancer();
        rec_contrainte(table, colonnes);
    }
    //Sinon epsilon
}

/************************************************************\
|                           WHERE                            |
\************************************************************/
// Conditions et filtres

// Fonction pour traiter une condition dans un WHERE
void rec_condition(Ast *A,table_t* table){
    Lexeme LC;
    TypeComparateur comp;
    TypeOperateurBool ope_bool;
    Ast A1; // EAG 1
    Ast A2; // EAG 2
    Ast A3; // Comparateur
    Ast A4; // Autre arbre à comparateur
    rec_valeur(&A1,table); //On récupère la valeur à gauche
    //On récupère le comparateur entre les 2
    LC=lexeme_courant();
    if (est_comparateur(LC)){
        comp = tranfo_nature_TypeComp(LC.nature);
        avancer();
    }
    rec_valeur(&A2,table); //On récupère la valeur à droite
    
    A3=creer_comparateur(comp,A1,A2); //On créer l'arbre avec le comparateur
    //Si il y a une operation booleen apres
    switch (lexeme_courant().nature) {
        case AND:
            ope_bool=N_AND;
            avancer();
            break;
        case OR:
            ope_bool=N_OR;
            avancer();
            break;
        default:
            *A=A3;//On renvoi l'arbre si pas d'operateur
            return;
    }
    rec_condition(&A4,table);
    *A=creer_operationBooL(ope_bool,A3,A4);//On créer l'arbre avec l'operteur booléen
}

// Fonction pour traiter une valeur d'une expression
void rec_valeur(Ast *A, table_t* table){
    if (lexeme_courant().nature==STR){ //Si c'est un string
        *A=creer_chaine(lexeme_courant().chaine);
        avancer();
    }
    else { //Si c'est une valeur qui donnera un entier
        rec_eag(A,table);
    }
}

// Fonction pour traiter un eag
void rec_eag(Ast *A1,table_t* table){
    seq_terme(A1,table);
}
// Fonction pour traiter une séquence de terme
void seq_terme(Ast *A2,table_t* table){
    Ast A1;
    terme(&A1,table);
    suite_seq_terme(A1,A2,table);
}
// Fonction pour traiter une séquence de termes dans un eag
void suite_seq_terme(Ast A1 , Ast *A2,table_t* table){
    Ast A3,A4;
    TypeOperateur op;
        
    if(op1(&op)){ //Si il y a un + ou un -
        terme(&A3,table);
        A4 = creer_operation(op,A1,A3);
        suite_seq_terme(A4,A2,table);		
    }	
    else{
        *A2=A1;		
    }
}
// Fonction pour traiter un terme 
void terme(Ast *A1,table_t* table){
    seq_facteur(A1,table);
}
// Fonction pour traiter un facteur
void seq_facteur(Ast *A2,table_t* table){
    Ast A1;
    facteur(&A1,table);
    suite_seq_facteur(A1,A2,table);
}

// Fonction pour traiter une suite de facteur 
void suite_seq_facteur(Ast A1 , Ast *A2,table_t* table){
    Ast A3,A4;
    TypeOperateur op;
    int r = op2(&op);
    if(r!=0){//Si il y a une multiplication
        facteur(&A3,table);
        A4 = creer_operation(op,A1,A3);
        suite_seq_facteur(A4,A2,table);
    }
    else{
        *A2=A1;		
    }
}

// Fonction pour traiter un facteur 
void facteur(Ast *A1,table_t* table){
    switch(lexeme_courant().nature){
        case NOMBRE://On est sur un nombre
            *A1 = creer_valeur(lexeme_courant().val);
            avancer();
            break;
        
        case VAR: //On est sur un attribut
            //On transforme notre attribut en sa position pour améliorer la fluidité du programme
            printf("") ; //à cause du case qui ne marche pas bien      
            int res_recherche_att=search_attribut(table,lexeme_courant().chaine); 
            if (res_recherche_att==-1){erreur_attribut_non_reconnu(lexeme_courant().chaine); }
            *A1 = creer_attribut(res_recherche_att);
            avancer();
            break;

        case PARO: //On est sur une parenthèse
            avancer();
            rec_eag(A1,table);
            if(lexeme_courant().nature != PARF){erreur();}
            avancer();            
            break;

        default: //Autres cas       
            printf("ERREUR : entier ou parenthese ouvrante attendu (ligne %u, colonne %u)\n",
                                            lexeme_courant().ligne, lexeme_courant().colonne);
            exit(0);
    }
    
}

// Fonction pour traiter les opération de type 1
int op1(TypeOperateur *Op){
    switch(lexeme_courant().nature){
        case PLUS:
        case MOINS:
            *Op = tranfo_nature_TypeOperateur(lexeme_courant().nature);
            avancer(); 
            return 1;
        default:
            return 0;
    }
}

// Fonction pour traiter les opération de type 2
int op2(TypeOperateur *Op){ 
    switch(lexeme_courant().nature){
        case ETOILE: 
            *Op = tranfo_nature_TypeOperateur(lexeme_courant().nature);
            avancer();
            return 1;
        default: 
            return 0;
    }
}

/************************************************************\
|                           SELECT                           |
\************************************************************/
// Récupération de données depuis la base de données

// Fonction pour traiter le début d'une requête SELECT
void req_select(table_aff_t* table_aff){

    Lexeme LC;
    Ast A_condi=NULL;
    if (lexeme_courant().nature!=SELECT){erreur();} // Mot SELECT
    avancer();

    int nb_attributs = 0;
    int a_etoile = 0; //Regarde si il y a une etoile (tous les attributs)

    cel_colonne_tete_t* attributs=init_colonne_tete(); //Initialisation des attributs 
    remplissage_colonne_tete(attributs,"col1_tempo", 0, RIEN); //On rempli avec un premier attribut fictif qu'on enlèvera par la suite
    rec_seq_attribut(attributs, &nb_attributs,&a_etoile);

    //On enlève l'attribut fictif
    cel_colonne_tete_t* temp=attributs; //Attribut temporaire 
    attributs=attributs->suiv;
    free(temp->nom_col); 
    free(temp);

    if (lexeme_courant().nature!=FROM){erreur();} //Mot FROM
    avancer();

    LC=lexeme_courant();
    if (LC.nature!=VAR){erreur();}

    //On vérifie que la table existe bien
    table_t* relation = search_table_aff(table_aff, LC.chaine);
    if (relation==NULL) {erreur_table_aff(LC.chaine);}

    //Si on a trouvé au moins une étoile dans les attributs
    if (a_etoile==1){
        developpe_etoile (relation,attributs,&nb_attributs);
    }
    avancer();
    
    //On regarde si il y a un WHERE
    switch (lexeme_courant().nature) {
        case SEPINST:
            avancer();
            break;
        case WHERE:
            avancer();
            rec_condition(&A_condi,relation); 
            if (lexeme_courant().nature!=SEPINST){erreur();}
            avancer();
            break;
        default:
            erreur();
            break;
   }
   
   //On crée la table qui va faire le select
   table_t* affichage = requete_select(relation, attributs, nb_attributs, A_condi); 
   afficher_table_final(affichage); //On l'affiche
   lib_table(affichage); //On libère la table d'affichage à la fin
   lib_ast(A_condi);
}

// Fonction pour traiter une séquence d'attributs dans une requête SELECT
void rec_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs, int* a_etoile){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case ETOILE:
        case VAR :   
            *nb_attributs += 1;
            //On rajoute un type ALL si il y a une étoile
            cel_colonne_tete_t* attributs_suite=init_colonne_tete();
            if (LC.nature==ETOILE){
                remplissage_colonne_tete(attributs_suite,LC.chaine,0,ALL);
                *a_etoile=1;
            }
            else {                
                remplissage_colonne_tete(attributs_suite,LC.chaine,0,RIEN);
            }            
            attributs->suiv = attributs_suite;                
            avancer();
            rec_suite_seq_attribut(attributs_suite, nb_attributs,a_etoile);
            break;

        default : //Si il n'y a ni d'étoile ni d'argument
            erreur();
            break;
    }
}

// Fonction pour traiter la suite d'une séquence d'attributs dans une requête SELECT
void rec_suite_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs, int* a_etoile){
    switch (lexeme_courant().nature){
        case VIRGULE:
            avancer();
            rec_seq_attribut(attributs, nb_attributs,a_etoile);
            break;
        default:
            break;  //epsilon
    }
}


/************************************************************\
|                           INSERT                           |
\************************************************************/
// Ajout de nouvelles données dans la base de données

// Fonction pour traiter le début de la requête INSERT 
void req_insert(table_aff_t* table_aff){
    Lexeme LC;
    if (lexeme_courant().nature!=INSERT){erreur();}
    avancer();
    if (lexeme_courant().nature!=INTO){erreur();}
    avancer();

    LC=lexeme_courant();
    if (LC.nature!=VAR){erreur();}
    avancer();

    // Vérifie que la table existe vraiment
    table_t* table = search_table_aff(table_aff, LC.chaine);    
    if (table==NULL) {erreur_table_aff(LC.chaine); }

    if (lexeme_courant().nature!=VALUES){erreur();}
    avancer();
    if (lexeme_courant().nature!=PARO){erreur();}
    avancer();
    // Traiter la séquence de valeurs à insérer
    Rec_seq_values(table);
    
    if (lexeme_courant().nature!=PARF){erreur();}
    avancer();
    if (lexeme_courant().nature!=SEPINST){erreur();}
    avancer();
    // afficher_table_final(table); //Affichage de la table avec la nouvelle valeur
}


// Fonction pour traiter la séquence de valeurs à insérer dans une ligne de la table
void Rec_seq_values(table_t* table){
    list_ligne_t* ligne=init_list_ligne();
    cel_colonne_tete_t* cel_col;
    int position=0;
    // Vérifier s'il y a des colonnes dans la table
    if (table->tete_col==NULL){
        erreur_colonne_inexistante();
    }
    rec_values(ligne,table->tete_col,&position,table);
    // Traiter les valeurs suivantes s'il y en a
    if (table->tete_col->suiv!=NULL){
        cel_col=rec_seq_suite_values(ligne,table->tete_col->suiv,&position,table);
        // S'il reste des colonnes à traiter, signaler une erreur
        if (cel_col!=NULL){
            erreur_manque_donnee_insert();
        }
    }
    // Ajout de la ligne à la table
    ajouter_liste_ligne(table,ligne);
}


// Fonction récursive pour traiter les valeurs suivantes à insérer dans une ligne de la table
cel_colonne_tete_t* rec_seq_suite_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col,int* position,table_t* table){
    Lexeme LC=lexeme_courant();
    // Si la virgule indique qu'il y a des valeurs suivantes à traiter
    if (LC.nature==VIRGULE){
        avancer();
        rec_values(ligne,cel_col,position, table);
        if (cel_col->suiv!=NULL){// S'il reste des colonnes dans la table, continuer la récursion
            return rec_seq_suite_values(ligne,cel_col->suiv,position,table);
        }
        return cel_col->suiv;
    }
    return cel_col;
}

// Fonction pour traiter les valeurs à insérer dans une ligne de la table
void rec_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col,int* position,table_t* table){
    Lexeme LC=lexeme_courant();

    // Si la valeur est un string
    if (LC.nature==STR){
        if (cel_col->type_don!=STRING){// Vérifie si la colonne accepte les chaînes de caractères
            erreur_donnee_illegale();
        }
        // Vérifie si il n'y a pas déjà une valeur pareil si primary key
        if (*position==table->primarykey){
            if (search_unique_key(table,LC.chaine,0)==1){
                erreur_donnee_deja_existant(LC.ligne);
            }
        }
        // Ajouter la chaîne de caractères à la ligne
        ajout_string_cellule(ligne,LC.chaine);
        avancer();
    }
    // Si la valeur est un nombre entier
    else if (LC.nature==NOMBRE){   
        if (cel_col->type_don!=ENTIER){// Vérifie si la colonne accepte les nombres entiers
            erreur_donnee_illegale();
        }
        // Vérifie si il n'y a pas déjà une valeur pareil si primary key
        if (*position==table->primarykey){
            if (search_unique_key(table,"",LC.val)==1){
                erreur_donnee_deja_existant(LC.ligne);
            }
        }
        // Ajouter le nombre entier à la ligne
        ajout_entier_cellule(ligne,LC.val);
        avancer();
    }
    else {
        erreur();
    }
    // Mettre à jour la position dans la ligne
    *position=*position+1;
}




/************************************************************\
|                           UPDATE                           |
\************************************************************/
// Mise à jour des données existantes dans la base de données

// Fonction pour traiter la requête UPDATE
void req_update(table_aff_t* table_aff){
    Lexeme LC;
    Ast A_condi=NULL;
    if (lexeme_courant().nature!=UPDATE){erreur();}
    avancer();

    LC=lexeme_courant();
    if (LC.nature!=VAR){erreur();}
    avancer();
    // Vérifie que la table existe vraiment
    table_t* table = search_table_aff(table_aff, LC.chaine);
    if (table==NULL) {erreur_table_aff(LC.chaine);}

    if (lexeme_courant().nature!=SET){erreur();}
    avancer();
    
    //Liste des affectations à faire
    affectation_cel_t* aff_cel=init_affectation_cel();
    rec_affecation(table,aff_cel); // Traitement les affectations
    
    //On regarde si il y a un WHERE
    switch (lexeme_courant().nature) {
        case SEPINST:
            avancer();
            break;
        case WHERE:
            avancer();
            rec_condition(&A_condi,table); 
            if (lexeme_courant().nature!=SEPINST){erreur();}
            avancer();
            break;
        default:
            erreur();
            break;
    }
    // Exécute la requête UPDATE
    requete_update(table, aff_cel, A_condi);
    lib_ast(A_condi); // Libérer la mémoire allouée pour l'arbre d'analyse syntaxique de la condition WHERE
    //afficher_table_final(table); //Affichage de la table pour debug
}

// Gère le traitement des affectations dans une requête UPDATE
void rec_affecation(table_t* table,affectation_cel_t* aff_cel){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=VAR){erreur();}//Attribut
    // Recherche/vérifie la position de l'attribut dans la table/ que l'attribut soit dans la table
    int res_recherche_att=search_attribut(table,LC.chaine); //recupere la position de l'attribut
    if (res_recherche_att==-1){erreur_attribut_non_reconnu(LC.chaine); }  //si l'attribut n'est pas dans la table

    aff_cel->num_att_aff= res_recherche_att;
    avancer();
    if (lexeme_courant().nature!=EGAL){erreur();} //symbole =
    avancer();
    // Traiter la suite de l'affectation
    rec_affecation_suite(table,aff_cel);
    rec_seq_suite_affecation(table,aff_cel);
}


// Fonction traitant la suite de l'affectation si la valeur est une chaîne ou un calcul
void rec_affecation_suite(table_t* table,affectation_cel_t* aff_cel){
    Lexeme LC=lexeme_courant();
    
    if (LC.nature==STR){
        aff_cel->chaine_aff=strdup(LC.chaine);
        avancer();
    }
    else {
        Ast A;
        rec_eag(&A,table);  //permet de faire un calcul dans un arbre
        aff_cel->arbre=A;
    }
}

// Fonction traitant la séquence des affectations supplémentaires s'il y en a
void rec_seq_suite_affecation(table_t* table,affectation_cel_t* aff_cel){
    if (lexeme_courant().nature==VIRGULE){ //Si il y a d'autre affectations 
        avancer();
        affectation_cel_t* aff_cel2=init_affectation_cel();
        aff_cel->suiv=aff_cel2;
        rec_affecation(table,aff_cel2);// Traite la nouvelle affectation
    }
}




/************************************************************\
|                    Requêtes Générales                      |
\************************************************************/
// Gestion des requêtes en générales sur la base de données

// Fonction reconnaissant et de traitant différents types de requêtes
void rec_requete(table_aff_t* table_aff){
    switch( lexeme_courant().nature){
        case CREATE :// Traite une requête CREATE
            req_create(table_aff);
            break;
        case SELECT : // Traite une requête SELECT
            req_select(table_aff);
            break;
        case INSERT : // Traite une requête INSERT
            req_insert(table_aff);
            break;
        case UPDATE : // Traite une requête UPDATE
            req_update(table_aff);
            break;
        default: //On ne reconnait pas la requete
            erreur_requete();
    }   
}

// Fonction effectuant l'analyse syntaxique du fichier spécifié et appliquant les requêtes
void analyser(char *fichier,table_aff_t* tab_aff) {
    
   demarrer(fichier);// Démarrer l'analyse lexicale du fichier
   // Effectuer l'analyse syntaxique du fichier en traitant chaque requête
   while (! fin_de_sequence()){
       rec_requete(tab_aff);
   } 
}