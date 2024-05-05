#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"
#include "table_affectation.h"
#include "table_donnes.h"
#include "type_ast.h"
#include "ast_construction.h"
#include "ast_parcours.h"


void erreur(){
    printf("Erreur syntaxique, on n'est pas à la fin\n");
    exit(1);
}

void erreur_table_aff(char * nom_table){
    printf("Erreur : la table %s n'a pas été trouvé dans la base de donnée\n",nom_table);
    exit(1);
}

void erreur_attribut_non_reconnu(char * nom_attribut){
    printf("Attribut '%s' non recconu\n",nom_attribut);
    exit(1);
}


void rec_suite_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs, int* a_etoile){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case VIRGULE:
            avancer();
            rec_seq_attribut(attributs, nb_attributs,a_etoile);
            break;
        default:
            break;  //epsilon
    }
}

void rec_seq_attribut(cel_colonne_tete_t* attributs, int* nb_attributs, int* a_etoile){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case ETOILE:
            *nb_attributs += 1;
            cel_colonne_tete_t* attributs_suite=init_colonne_tete();
            remplissage_colonne_tete(attributs_suite,LC.chaine,0,ALL);
            
            attributs->suiv = attributs_suite;    
            
            avancer();
            *a_etoile=1;
            break;
        case VAR :   
            *nb_attributs += 1;
            cel_colonne_tete_t* attributs_suite2=init_colonne_tete();
            remplissage_colonne_tete(attributs_suite2,LC.chaine,0,RIEN);
            
            attributs->suiv = attributs_suite2; 
            avancer();
            rec_suite_seq_attribut(attributs_suite2, nb_attributs,a_etoile);
            break;
        default :
            erreur();
            break;
    }
}

void developpe_etoile (table_t* table, cel_colonne_tete_t* attributs, int* nb_attributs){
    cel_colonne_tete_t* tete_table=table->tete_col;
    if (tete_table==NULL){
        printf("Erreur '%s' table sans colonne",table->nom_table);
    }
    while (attributs!=NULL){
        if (attributs->type_don==ALL){
            cel_colonne_tete_t* cel_apres_all=attributs->suiv;
            attributs->type_don=tete_table->type_don;
            free(attributs->nom_col);
            attributs->nom_col=strdup(tete_table->nom_col);
            
            tete_table=tete_table->suiv;
            while (tete_table!=NULL){
                cel_colonne_tete_t* attributs_suite=init_colonne_tete();
                remplissage_colonne_tete(attributs_suite,tete_table->nom_col,0,tete_table->type_don);
                attributs->suiv = attributs_suite;       
                attributs=attributs_suite;
                tete_table=tete_table->suiv;            
            }
            *nb_attributs=*nb_attributs+table->nb_arg-1;
            tete_table=table->tete_col;
            attributs->suiv=cel_apres_all;
        }
        attributs=attributs->suiv;
    }
}


table_t* requete_select(table_t* relation, cel_colonne_tete_t* attributs, int nb_attributs, Lexeme tab_where[3]){
    // table_t* affichage = init_table("affichage", nb_attributs, attributs);
    
    table_t* affichage = init_table();
    remplissage_table(affichage,"affichage", nb_attributs, attributs);

    //Convertir les attributs du select en tableau d'int
    int positions[nb_attributs];
    cel_colonne_tete_t* attributs_temp = attributs;
    for (int i = 0; i < nb_attributs; i++){        
        int res_recherche_att=search_attribut(relation,attributs_temp->nom_col);
        if (res_recherche_att==-1){erreur_attribut_non_reconnu(attributs_temp->nom_col); }
        positions[i] = res_recherche_att;
        attributs_temp = attributs_temp->suiv;
    }    

    //Convertir la position du WHERE en numero
    if (tab_where[0].val == 1){
        int res_recherche_att=search_attribut(relation,tab_where[1].chaine);
        if (res_recherche_att==-1){erreur_attribut_non_reconnu(tab_where[1].chaine); }
        tab_where[1].val = search_attribut(relation,tab_where[1].chaine);
    }
    
    //Ajouter les valeurs dans la table affichage
    list_ligne_t* relation_liste_ligne = relation->tete_ligne;
    while(relation_liste_ligne != NULL){
        
        int check_where = 0;
        
        if (tab_where[0].val == 1){
            //Avance jusqu'a la colonne a vérifier pour le WHERE
            donnee_cel_t* where_tete_ligne = relation_liste_ligne->tete;
            if (tab_where[1].val > 0){
                for (int i = 0; i < tab_where[1].val; i++){
                    where_tete_ligne = where_tete_ligne->suiv;
                }
            }

            //Vérifie la condition du WHERE
            if (tab_where[2].nature == STR){
                switch (tab_where[0].nature){
                case EGAL:
                    if (where_tete_ligne->string == NULL || strcmp(where_tete_ligne->string, tab_where[2].chaine)!=0){
                        check_where = 1;
                    }
                    break;
                case DIFF:
                    if (where_tete_ligne->string == NULL || strcmp(where_tete_ligne->string, tab_where[2].chaine)==0){
                        check_where = 1;
                    }
                    break;
                default:
                    break;
                }
            }else {
                switch (tab_where[0].nature){
                case EGAL:
                    if (where_tete_ligne->val != tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case DIFF:
                    if (where_tete_ligne->val == tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case SUP:
                    if (where_tete_ligne->val <= tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case SUPEGAL:
                    if (where_tete_ligne->val < tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case INF:
                    if (where_tete_ligne->val >= tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case INFEGAL:
                    if (where_tete_ligne->val > tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        //Ajoute les valeurs de la ligne si la condition du WHERE est satisfaite
        if (check_where != 1){
            list_ligne_t* ligne_affichage = init_list_ligne();
            
            for (int i = 0; i < nb_attributs; i++){
                donnee_cel_t* relation_tete_ligne = relation_liste_ligne->tete;
                for (int j = 0; j < positions[i]; j++){
                    relation_tete_ligne = relation_tete_ligne->suiv;
                }
                if (relation_tete_ligne->type_don==ENTIER){
                    ajout_entier_cellule(ligne_affichage, relation_tete_ligne->val);
                    // printf("\nVal: %d", relation_tete_ligne->val);
                } 
                else{
                    ajout_string_cellule(ligne_affichage, relation_tete_ligne->string);
                    // printf("\nVal: %s", relation_tete_ligne->string);
                }
            }
            ajouter_liste_ligne(affichage, ligne_affichage);
        }

        relation_liste_ligne = relation_liste_ligne->suiv;
    }  

    //Ajout des type_don a colonne_tete attributs
    if (affichage->tete_ligne != NULL){
        cel_colonne_tete_t* attributs_temp2 = attributs;
        donnee_cel_t* cel_temp2 = affichage->tete_ligne->tete;
        while(attributs_temp2 != NULL){
            attributs_temp2->type_don = cel_temp2->type_don;
            attributs_temp2 = attributs_temp2->suiv;
            cel_temp2 = cel_temp2->suiv;
        }
    }
    

    //afficher_table_final(affichage);
    return affichage;
}

void rec_deb_select(table_aff_t* table_aff){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=SELECT){
      erreur();
   }
   avancer();
   int nb_attributs = 0;
   int a_etoile = 0;
    cel_colonne_tete_t* attributs=init_colonne_tete();
    remplissage_colonne_tete(attributs,"col1_tempo", 0, RIEN);
   rec_seq_attribut(attributs, &nb_attributs,&a_etoile);
   cel_colonne_tete_t* temp=attributs;
   attributs=attributs->suiv;
   free(temp->nom_col);
   free(temp);
//    printf("Les attributs de la nouvelle table dans requete:\n");
//    afficher_colonne_tete_final(attributs);
//    printf("\nNombre d'attributs_dans requete: %d \n", nb_attributs);

   LC=lexeme_courant();
   if (LC.nature!=FROM){
      erreur();
   }
   avancer();
   LC=lexeme_courant();
   if (LC.nature!=VAR){  
      erreur();
   }
   table_t* relation = search_table_aff(table_aff, LC.chaine);
   if (relation==NULL) {erreur_table_aff(LC.chaine); }
   if (a_etoile==1){
        developpe_etoile (relation, attributs,&nb_attributs);
   }
//    printf("Table trouvé dans requete: \n");
   //afficher_table_final(relation);
   avancer();
   LC=lexeme_courant();
   Lexeme tab_where[3]; // [OPERATEUR / val = 0 si pas de WHERE val = 1 si WHERE, colonne, valeur]
   tab_where[0].val = 0;
   
   switch (LC.nature) {
        case SEPINST:
            avancer();
            break;
        case WHERE:
            avancer();
            rec_condition(tab_where); 
            LC=lexeme_courant();
            if (LC.nature!=SEPINST){
                erreur();
            }
            avancer();
            break;
        default:
            erreur();
            break;
   }
   
   table_t* affichage = requete_select(relation, attributs, nb_attributs, tab_where);
   afficher_table_final(affichage);
   
}


void rec_condition(Lexeme tab_where[3]){
    Lexeme LC=lexeme_courant();
    if (LC.nature != VAR){   //attribut
        erreur();
    }
    tab_where[1] = LC;
    avancer();

    LC=lexeme_courant();
    tab_where[0] = LC;
    tab_where[0].val = 1;
    switch (LC.nature){
        case SUP:
        case INF:
        case SUPEGAL:
        case INFEGAL:
            avancer();
            LC=lexeme_courant();
            if (LC.nature != NOMBRE){
                erreur();
            }   
            tab_where[2] = LC;
            avancer();
            break;

        case EGAL:
        case DIFF:
            avancer();
            LC=lexeme_courant();
            switch (LC.nature){
                case STR:
                    tab_where[2] = LC;
                    avancer();
                    break;
                case NOMBRE:
                    tab_where[2] = LC;
                    avancer();
                    break;
                default:
                    erreur();
                    break;
            }
            break;
        default:
            erreur();
            break;
    }
}



void rec_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col,int* position,table_t* table){
    Lexeme LC=lexeme_courant();
    
    if (LC.nature==STR){
        if (cel_col->type_don!=STRING){
            printf("Ajout de donnée illégale\n");
            erreur();
        }
        if (*position==table->primarykey){
            if (search_unique_key(table,LC.chaine,0)==1){
                printf("Erreur: La valeur que vous voulez rajoutez existe déjà, ligne :%d\n",LC.ligne);
                exit(1);
            }
        }
        ajout_string_cellule(ligne,LC.chaine);
        avancer();
    }
    else if (LC.nature==NOMBRE){   
        if (cel_col->type_don!=ENTIER){
            printf("Ajout de donnée illégale\n");
            erreur();
        }
        if (*position==table->primarykey){
            if (search_unique_key(table,"",LC.val)==1){
                printf("Erreur: La valeur que vous voulez rajoutez existe déjà, ligne :%d\n",LC.ligne);
                exit(1);
            }
        }
        ajout_entier_cellule(ligne,LC.val);
        avancer();
    }
    else {
        erreur();
    }
    *position=*position+1;
}

cel_colonne_tete_t* rec_seq_suite_values(list_ligne_t* ligne,cel_colonne_tete_t* cel_col,int* position,table_t* table){
    Lexeme LC=lexeme_courant();
    
    if (LC.nature==VIRGULE){
        avancer();
        rec_values(ligne,cel_col,position, table);
        if (cel_col->suiv!=NULL){
            return rec_seq_suite_values(ligne,cel_col->suiv,position,table);
        }
        return cel_col->suiv;
    }
    return cel_col;
}

void Rec_seq_values(table_t* table){
    list_ligne_t* ligne=init_list_ligne();
    cel_colonne_tete_t* cel_col;
    int position=0;
    if (table->tete_col==NULL){
        printf("Erreur : Pas de colonne à quoi rajouter des données\n");
        exit(1);
    }
    rec_values(ligne,table->tete_col,&position,table);
    // afficher_lex(lexeme_courant());
    if (table->tete_col->suiv!=NULL){
        // printf("\nrentre dans boucle 1\n");
        cel_col=rec_seq_suite_values(ligne,table->tete_col->suiv,&position,table);
        if (cel_col!=NULL){
            printf("Il manque des données à rajouter dans l'insert\n");
            erreur();
        }
    }
    ajouter_liste_ligne(table,ligne);
}


void debut_insert(table_aff_t* table_aff){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=INSERT){
      erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=INTO){
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=VAR){
        erreur();
    }
    avancer();

    table_t* table = search_table_aff(table_aff, LC.chaine);    
    if (table==NULL) {erreur_table_aff(LC.chaine); }

    LC=lexeme_courant();
    if (LC.nature!=VALUES){
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=PARO){
        erreur();
    }
    avancer();
    Rec_seq_values(table);
    
    LC=lexeme_courant();
    if (LC.nature!=PARF){
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=SEPINST){
        erreur();
    }
    avancer();
    // afficher_table_final(table);
}

int search_unique_key(table_t* table, char* nom_string,int nb_ent){
    //Renvoie 0 si pas trouver, 1 si trouver
    if (table->primarykey==-1){return 0;}

    list_ligne_t* ligne=table->tete_ligne;
    while (ligne!=NULL){
        donnee_cel_t* don_cel=ligne->tete;    
        for (int i=0; i< table->primarykey;i++){
            don_cel=don_cel->suiv;
        }
        switch (don_cel->type_don){
            case ENTIER:
                if (don_cel->val==nb_ent){
                    return 1;
                }
                break;
            case STRING: 
                if (strcmp(don_cel->string,nom_string)==0){
                    return 1;
                }
                break;
            default:
                break;
        }       
        ligne=ligne->suiv;        
    }
    return 0;
}


void rec_create(table_aff_t* table_aff){
    /*debut de l'arbre create (debut de la fonction pour faire les create)*/
    //initialisation
    char nomtable[100];
    int n=0;
    cel_colonne_tete_t* colonnes=(cel_colonne_tete_t*)malloc(sizeof(cel_colonne_tete_t));
    Lexeme LC=lexeme_courant(); //mot create
    if (LC.nature!=CREATE){
      erreur();
    }
    avancer();
    LC=lexeme_courant(); //mot table
    if (LC.nature!=TABLE){
        erreur();
    }
    avancer();
    LC=lexeme_courant(); //nom de la table
    if (LC.nature!=VAR){
        erreur();
    }
    strcpy(nomtable,LC.chaine); //on le stocke quelque part pour l'instant
    avancer();
    LC=lexeme_courant(); //parenthese ouvrante
    if (LC.nature!=PARO){
        erreur();
    }
    avancer();
    table_t* table = init_table(); //initialisation de la table dans la memoire (en C)
    rec_seq_creation_attribut(table,colonnes,&n); //interieur de la table
    
    LC=lexeme_courant(); //parenthese fermante
    if (LC.nature!=PARF){
        erreur();
    }
    avancer();

    LC=lexeme_courant(); //point virgule
    if (LC.nature!=SEPINST){
        erreur();
    }
    avancer();
    remplissage_table(table,nomtable,n,colonnes); //on remplit la table
    // afficher_table(table);
    ajout_table_aff(table_aff,table); //on ajoute a la liste de tables la table table
}

void rec_seq_creation_attribut(table_t* table, cel_colonne_tete_t* colonnes, int* n){
    /*interieur de la table*/
    rec_creation_attribut(table,colonnes,n);
    table->tete_col=colonnes;
    rec_seq_suite_cree_attribut(table,colonnes,n);
}

void rec_creation_attribut(table_t* table,cel_colonne_tete_t* colonnes,int* n){
    /*nom des attributs et position dans la table*/
    Lexeme LC=lexeme_courant(); /*nom de la variable*/
    if (LC.nature!=VAR){
        erreur();
    }
    if (search_attribut(table,LC.chaine)!=-1){
        printf("Erreur: attribut déjà existant dans la table\n");
        exit(1);
    }
    colonnes->pos=*n; //position
    colonnes->nom_col=strdup(LC.chaine); //on stocke le nom de l'attribut en memoire (en C)
    *n=*n+1; //on change de position pour plus tard
    avancer();
    rec_type(colonnes);//definit le type de l'attribut
}

void rec_type(cel_colonne_tete_t* colonnes){
    /*type d'un attribut*/
    Lexeme LC=lexeme_courant();//type TEXT ou ENTIER
    if ((LC.nature!=TEXT) && (LC.nature!=INTEGER)){ //Si aucun des deux, erreur
        erreur();
    }
    if (LC.nature==TEXT){ //si text
        colonnes->type_don=STRING;
    }
    else if (LC.nature==INTEGER) { //si entier
        colonnes->type_don=ENTIER;
    }
    avancer();
}

void rec_seq_suite_cree_attribut(table_t* table,cel_colonne_tete_t* colonnes,int* n){
    /*boucle pour l'interieur du create*/
    Lexeme LC=lexeme_courant();
    if (LC.nature==VIRGULE){ //si il y a une virgule
        avancer();
        LC=lexeme_courant(); //si il y a un autre attribut a enregistrer
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
}

void rec_contrainte(table_t* table, cel_colonne_tete_t* colonnes){
    /*primary key*/
    Lexeme LC=lexeme_courant();
    if (LC.nature!=CONSTRAINT){//mot contrainte
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=VAR){ //variable nom de la primary key
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=PRIMARY){//mot primary
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=KEY){ //mot key
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=PARO){//parenthese ouvrante
        erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=VAR){// variable
        erreur();
    }
    int res_recherche_att=search_attribut(table,LC.chaine);
    if (res_recherche_att==-1){erreur_attribut_non_reconnu(LC.chaine); }
    table->primarykey = res_recherche_att;

    //printf("\nprimary key: %s\n",table->pk);
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=PARF){//parenthese fermante
        erreur();
    }
    avancer();
    if (LC.nature==VIRGULE){//virgule (au-cas ou on rajoute des fk et check)
        avancer();
        rec_contrainte(table, colonnes);
    }
}




void rec_valeur(){
    Lexeme LC=lexeme_courant();
    switch (LC.nature){
        case STR:
        case NOMBRE:
            avancer();
            break;
        default:
            erreur();
            break;
    }
}

void rec_eag(Ast *A1,table_t* table)
 	 {
 		seq_terme(A1,table);
 	 }
 	 
 	 void seq_terme(Ast *A2,table_t* table)
 	 {
		Ast A1;
 		terme(&A1,table);
 		suite_seq_terme(A1,A2,table);
 	 }

 	void suite_seq_terme(Ast A1 , Ast *A2,table_t* table)
	{
		Ast A3,A4;
		TypeOperateur op;
			
		if(op1(&op))
		{
		terme(&A3,table);
		A4 = creer_operation(op,A1,A3);
		suite_seq_terme(A4,A2,table);		
		}	
		else
		{   
			*A2=A1;		
		}
	}
	
	void terme(Ast *A1,table_t* table)
	{
		seq_facteur(A1,table);
	}
	
	void seq_facteur(Ast *A2,table_t* table)
	{
		Ast A1;
		facteur(&A1,table);
		suite_seq_facteur(A1,A2,table);
	}
	
	void suite_seq_facteur(Ast A1 , Ast *A2,table_t* table)
	{
		Ast A3,A4;
		TypeOperateur op;
		int r = op2(&op);
		if(r!=0)
		{
		facteur(&A3,table);
		A4 = creer_operation(op,A1,A3);
		suite_seq_facteur(A4,A2,table);
		}
		else
		{
			*A2=A1;		
		}
		
		
	}
	
	void facteur(Ast *A1,table_t* table)
	{
		switch(lexeme_courant().nature){
			case NOMBRE:
				*A1 = creer_valeur(lexeme_courant().val);
				avancer();
				break;
            
			case VAR:     
                printf("") ; //à cause du case qui ne marche pas bien      
                int res_recherche_att=search_attribut(table,lexeme_courant().chaine);
                if (res_recherche_att==-1){erreur_attribut_non_reconnu(lexeme_courant().chaine); }
				*A1 = creer_attribut(res_recherche_att);
				avancer();
				break;
			case PARO:
				avancer();
				rec_eag(A1,table);
				if(lexeme_courant().nature == PARF)
		  		{
		  			avancer();
		  		}
		  		else
		  		{
		  			printf("ERREUR : parenthese fermante attendue (ligne %u, colonne %u)\n",
                                                    lexeme_courant().ligne, lexeme_courant().colonne);
		  			exit(1);
		  		}
		  		break;
		  	default:
		  			printf("ERREUR : entier ou parenthese ouvrante attendu (ligne %u, colonne %u)\n",
                                                    lexeme_courant().ligne, lexeme_courant().colonne);
		  			exit(0);

		  }
        
	}
	
	int op1(TypeOperateur *Op)
	{
        
		switch(lexeme_courant().nature){
			case PLUS:
			case MOINS:
				*Op = Operateur(lexeme_courant().nature);
				avancer(); 
				return 1;
			default:
				return 0;
		}
        
	}
	
	int op2(TypeOperateur *Op)
	{ 
		switch(lexeme_courant().nature){
			case ETOILE: 
				*Op = Operateur(lexeme_courant().nature);
				avancer();
				return 1;
			default: 
				return 0;
		}
	}
		 
 	 	TypeOperateur Operateur(Nature_Lexeme nature)
{
		switch(nature)
		{
			case PLUS : return N_PLUS;
			case MOINS : return N_MOINS;
			case ETOILE: return N_MUL;
			default: printf("Erreur operateur"); exit(1) ; 
		}
}

int search_attribut(table_t* table,char* attribut){
    cel_colonne_tete_t* tempo = table->tete_col;
    while (tempo!=NULL && strcmp(tempo->nom_col,attribut)!=0){
        tempo = tempo->suiv;
    }
    if (tempo==NULL){
        return -1;
    }
    return tempo->pos;    
}

void rec_affecation(table_t* table,affectation_cel_t* aff_cel){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=VAR){
      erreur();
    }
    int res_recherche_att=search_attribut(table,LC.chaine);
    if (res_recherche_att==-1){erreur_attribut_non_reconnu(LC.chaine); }
    aff_cel->num_att_aff= res_recherche_att;
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=EGAL){
        erreur();
    }
    avancer();
    rec_affecation_suite(table,aff_cel);
    rec_seq_suite_affecation(table,aff_cel);
}


void rec_affecation_suite(table_t* table,affectation_cel_t* aff_cel){
    Lexeme LC=lexeme_courant();
    
    if (LC.nature==STR){
        aff_cel->chaine_aff=strdup(LC.chaine);
        avancer();
    }
    else {
        Ast A;
        rec_eag(&A,table);

        //afficherA (A);
        aff_cel->arbre=A;
    }
}

void rec_seq_suite_affecation(table_t* table,affectation_cel_t* aff_cel){
    Lexeme LC=lexeme_courant();
    if (LC.nature==VIRGULE){
        avancer();
        affectation_cel_t* aff_cel2=init_affectation_cel();
        aff_cel->suiv=aff_cel2;
        rec_affecation(table,aff_cel2);
        rec_seq_suite_affecation(table,aff_cel2);
    }
}


void rec_update(table_aff_t* table_aff){
    Lexeme LC=lexeme_courant();
    if (LC.nature!=UPDATE){
      erreur();
    }
    avancer();
    LC=lexeme_courant();
    if (LC.nature!=VAR){
        erreur();
    }
    avancer();
    table_t* table = search_table_aff(table_aff, LC.chaine);
    if (table==NULL) {erreur_table_aff(LC.chaine); }
    //afficher_table_final(table);
    LC=lexeme_courant();
    if (LC.nature!=SET){
        erreur();
    }
    avancer();
    
    //Liste des affectations à faire
    affectation_cel_t* aff_cel=init_affectation_cel();
    rec_affecation(table,aff_cel);
    
    Lexeme tab_where[3]; // [OPERATEUR / val = 0 si pas de WHERE val = 1 si WHERE, colonne, valeur]
   tab_where[0].val = 0;
   
    LC=lexeme_courant();   
   switch (LC.nature) {
        case SEPINST:
            avancer();
            break;
        case WHERE:
            avancer();
        
            rec_condition(tab_where); 
            LC=lexeme_courant();
            if (LC.nature!=SEPINST){
                erreur();
            }
            avancer();
            break;
        default:
            erreur();
            break;
    }
    requete_update(table, aff_cel, tab_where);
    //afficher_table_final(table);
}

void requete_update(table_t* relation, affectation_cel_t* aff_cel, Lexeme tab_where[3]){
    
    //Convertir la position du WHERE en numero
    if (tab_where[0].val == 1){
        int res_recherche_att=search_attribut(relation,tab_where[1].chaine);
        if (res_recherche_att==-1){erreur_attribut_non_reconnu(tab_where[1].chaine); }
        tab_where[1].val =res_recherche_att;
    }
    
    //MODIFICATION  les valeurs dans la table affichage
    list_ligne_t* relation_liste_ligne = relation->tete_ligne;
    while(relation_liste_ligne != NULL){
        
        int check_where = 0;
        
        if (tab_where[0].val == 1){
            //Avance jusqu'a la colonne a vérifier pour le WHERE
            donnee_cel_t* where_tete_ligne = relation_liste_ligne->tete;
            if (tab_where[1].val > 0){
                for (int i = 0; i < tab_where[1].val; i++){
                    where_tete_ligne = where_tete_ligne->suiv;
                }
            }

            //Vérifie la condition du WHERE
            if (tab_where[2].nature == STR){
                switch (tab_where[0].nature){
                case EGAL:
                    if (where_tete_ligne->string == NULL || strcmp(where_tete_ligne->string, tab_where[2].chaine)!=0){
                        check_where = 1;
                    }
                    break;
                case DIFF:
                    if (where_tete_ligne->string == NULL || strcmp(where_tete_ligne->string, tab_where[2].chaine)==0){
                        check_where = 1;
                    }
                    break;
                default:
                    break;
                }
            }else {
                switch (tab_where[0].nature){
                case EGAL:
                    if (where_tete_ligne->val != tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case DIFF:
                    if (where_tete_ligne->val == tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case SUP:
                    if (where_tete_ligne->val <= tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case SUPEGAL:
                    if (where_tete_ligne->val < tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case INF:
                    if (where_tete_ligne->val >= tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                case INFEGAL:
                    if (where_tete_ligne->val > tab_where[2].val){
                    check_where = 1;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        //MODIFIE les valeurs de la ligne si la condition du WHERE est satisfaite
        if (check_where != 1){
            affectation_cel_t* aff_cel_tempo=aff_cel;
            while (aff_cel_tempo!=NULL){

                donnee_cel_t* relation_tete_ligne = relation_liste_ligne->tete;
                for (int j = 0; j < aff_cel_tempo->num_att_aff; j++){
                    relation_tete_ligne = relation_tete_ligne->suiv;
                }

                if (relation_tete_ligne->type_don==ENTIER){      
                    int res = evaluation_update(aff_cel_tempo->arbre,relation_liste_ligne);   

                    if (aff_cel_tempo->num_att_aff==relation->primarykey){
                        if (search_unique_key(relation,"",res)==1){
                            printf("Erreur: La valeur que vous voulez rajoutez existe déjà\n");
                            exit(1);
                        }
                    }              
                    relation_tete_ligne->val=res;
                } 
                else{
                    if (aff_cel_tempo->num_att_aff==relation->primarykey){
                        if (search_unique_key(relation,aff_cel_tempo->chaine_aff,0)==1){
                            printf("Erreur: La valeur que vous voulez rajoutez existe déjà\n");
                            exit(1);
                        }
                    }
                    free(relation_tete_ligne->string);
                    relation_tete_ligne->string=strdup(aff_cel_tempo->chaine_aff);
                }
                aff_cel_tempo=aff_cel_tempo->suiv;
            }
        }

        relation_liste_ligne = relation_liste_ligne->suiv;
    } 
}

void rec_requete(table_aff_t* table_aff){
    Lexeme LC=lexeme_courant();
    // int res;
    // rec_exp(&res);
    // return;
    switch(LC.nature){
        case SELECT :
            rec_deb_select(table_aff);
            break;
        case CREATE :
            rec_create(table_aff);
            break;
        case INSERT :
            debut_insert(table_aff);
            break;
        case UPDATE :
            rec_update(table_aff);
            break;
        default:
        break;
    }   
}


int evaluation_update(Ast expr,list_ligne_t* lst_lig) {
    int denominateur ;
	switch(expr->nature)
	{
	  case OPERATION:
		switch (expr->operateur)
		{

			case N_MUL:
			  return evaluation_update(expr->gauche,lst_lig)*evaluation_update(expr->droite,lst_lig);
	
			case N_MOINS:
			  return evaluation_update(expr->gauche,lst_lig)-evaluation_update(expr->droite,lst_lig);

			case N_PLUS:
			  return evaluation_update(expr->gauche,lst_lig)+evaluation_update(expr->droite,lst_lig);

			
			default:
			  exit(1);  // erreur operateur
		}

	   case VALEUR:
		return expr->valeur; 

       case ATTRIBUT:
            printf("");
                int i=0;
                donnee_cel_t* cel_donn = lst_lig->tete;
                while (cel_donn!=NULL && i<expr->num_attribut){
                    cel_donn = cel_donn->suiv;
                    i++;
                }
                if (cel_donn==NULL || cel_donn->type_don!=ENTIER){
                    printf("Attribut avec le mauvais numero: %d\n",expr->num_attribut);
                    exit(1);
                }
                return cel_donn->val; 
            break;
		
	   default:
		exit(2); // erreur Ast 
	}

}


void analyser(char *fichier) {
    cel_colonne_tete_t* col_tete=init_colonne_tete();
    remplissage_colonne_tete(col_tete,"col1",0,ENTIER);
    
    col_tete->suiv=init_colonne_tete();
    remplissage_colonne_tete(col_tete->suiv,"col2",1,STRING);

    
    col_tete->suiv->suiv=init_colonne_tete();
    remplissage_colonne_tete(col_tete->suiv->suiv,"col3",2,ENTIER);
    
    
    table_t* table=init_table();
    remplissage_table(table,"table",3,col_tete);
    list_ligne_t* list_lig=init_list_ligne();
    list_ligne_t* list_suiv=init_list_ligne();
    ajout_entier_cellule(list_lig,69691111);
    ajout_string_cellule(list_lig,"dld,zpd");
    ajout_entier_cellule(list_lig,1);
    ajout_entier_cellule(list_suiv,111);
    ajout_string_cellule(list_suiv,"ARABE");
    ajout_entier_cellule(list_suiv,112);
    ajouter_liste_ligne(table, list_lig);
    ajouter_liste_ligne(table, list_suiv);

   table_aff_t* tab_aff= init_table_aff();
   ajout_table_aff(tab_aff,table);

   demarrer(fichier);
   while (! fin_de_sequence()){

   rec_requete(tab_aff);

   } 
}