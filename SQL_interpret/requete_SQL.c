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

// Recherche un attribut dans une table et renvoyer sa position
int search_attribut(table_t* table,char* attribut){
    //Renvoie -1 si pas trouver, la position de l'attribut si trouvé
    cel_colonne_tete_t* tempo = table->tete_col;
    while (tempo!=NULL && strcmp(tempo->nom_col,attribut)!=0){  //parcours la table
        tempo = tempo->suiv;
    }
    if (tempo==NULL){ //si il n'est pas dans la table
        return -1;
    }
    return tempo->pos; //renvoie la position de l'attribut
}

// Fonction pour rechercher une clé primaire dans une table
int search_unique_key(table_t* table, char* nom_string,int nb_ent){
    //Renvoie 0 si pas trouver, 1 si trouver
    if (table->primarykey==-1){return 0;}// Si la table n'a pas de clé primaire

    //Parcours de toutes les lignes du tableau
    list_ligne_t* ligne=table->tete_ligne;
    while (ligne!=NULL){
        //On parcours pour aller sur le bon attribut
        donnee_cel_t* don_cel=ligne->tete;    
        for (int i=0; i< table->primarykey;i++){ 
            don_cel=don_cel->suiv;
        }

        //Vérification si ça n'existe pas déjà
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

// Fonction pour développer le symbole '*' dans SELECT
void developpe_etoile(table_t* table, cel_colonne_tete_t* attributs, int* nb_attributs){
    // Initialise un pointeur à la tête de la liste des colonnes de la table
    cel_colonne_tete_t* tete_table = table->tete_col;

    // Vérifie si la table est vide
    if (tete_table == NULL) {
        printf("Erreur '%s' table sans colonne", table->nom_table);
    }

    // Boucle principale parcourant tous les attributs
    while (attributs != NULL) {
        // Vérifie si l'attribut est de type ALL
        if (attributs->type_don == ALL) {
            // Garde une référence à la cellule suivante après le ALL
            cel_colonne_tete_t* cel_apres_all = attributs->suiv;

            // Met à jour le type de donnée de l'attribut avec celui de la première colonne de la table
            attributs->type_don = tete_table->type_don;
            
            // Libère la mémoire allouée pour le nom de colonne de l'attribut et lui attribue le nom de la première colonne de la table
            free(attributs->nom_col);
            attributs->nom_col = strdup(tete_table->nom_col);

            // Avance à la colonne suivante dans la table
            tete_table = tete_table->suiv;

            // Boucle interne parcourant le reste des colonnes de la table
            while (tete_table != NULL) {
                // Initialise une nouvelle cellule de colonne pour chaque colonne de la table
                cel_colonne_tete_t* attributs_suite = init_colonne_tete();

                // Remplit les détails de la nouvelle cellule de colonne avec les informations de la colonne actuelle de la table
                remplissage_colonne_tete(attributs_suite, tete_table->nom_col, 0, tete_table->type_don);

                attributs->suiv = attributs_suite;
                attributs = attributs_suite;
                tete_table = tete_table->suiv;
            }

            // Met à jour le nombre total d'attributs en fonction du nombre d'arguments de la table
            *nb_attributs = *nb_attributs + table->nb_arg - 1;

            // Réinitialise tete_table à la tête de la liste des colonnes de la table
            tete_table = table->tete_col;

            // Lie la cellule suivant le ALL à la liste d'attributs
            attributs->suiv = cel_apres_all;
        }        
        // Avance à la cellule suivante dans la liste d'attributs
        attributs = attributs->suiv;
    }
}

// Fonction pour exécuter une requête SELECT
table_t* requete_select(table_t* relation, cel_colonne_tete_t* attributs, int nb_attributs, Ast A_condi){
    
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
    
    //Ajouter les valeurs dans la table affichage
    list_ligne_t* relation_liste_ligne = relation->tete_ligne;
    while(relation_liste_ligne != NULL){
        //Ajoute les valeurs de la ligne si la condition du WHERE est satisfaite
        if (evaluation_bool(A_condi,relation_liste_ligne)){

            //On crée la ligne séléctionner dans l'affichage
            list_ligne_t* ligne_affichage = init_list_ligne();

            //Parcours de la ligne de la table originel
            for (int i = 0; i < nb_attributs; i++){
                donnee_cel_t* relation_tete_ligne = relation_liste_ligne->tete;
                for (int j = 0; j < positions[i]; j++){ // On se déplace jusqu'à l'attribut demandé
                    relation_tete_ligne = relation_tete_ligne->suiv;
                }
                //On rajoute à la ligne d'affichage suivant son type
                if (relation_tete_ligne->type_don==ENTIER){
                    ajout_entier_cellule(ligne_affichage, relation_tete_ligne->val);
                } 
                else{
                    ajout_string_cellule(ligne_affichage, relation_tete_ligne->string);
                }
            }
            ajouter_liste_ligne(affichage, ligne_affichage); //On rajoute à la table d'affichage la ligne construite au dessus
            
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
    return affichage;
}


// Fonction pour exécuter une requête UPDATE
void requete_update(table_t* relation, affectation_cel_t* aff_cel, Ast A_condi ){
    //Parcours de toute la table
    list_ligne_t* relation_liste_ligne = relation->tete_ligne;
    while(relation_liste_ligne != NULL){
        
        //Ajoute les valeurs de la ligne si la condition du WHERE est satisfaite
        if (evaluation_bool(A_condi,relation_liste_ligne)){
            affectation_cel_t* aff_cel_tempo=aff_cel; 

            //On parcours toutes les affectations que l'on doit faire
            while (aff_cel_tempo!=NULL){

                // Récupère la cellule de données correspondant à l'attribut à mettre à jour
                donnee_cel_t* relation_tete_ligne = relation_liste_ligne->tete;
                for (int j = 0; j < aff_cel_tempo->num_att_aff; j++){
                    relation_tete_ligne = relation_tete_ligne->suiv;
                }

                // Met à jour la valeur de l'attribut en fonction du type de donnée
                if (relation_tete_ligne->type_don==ENTIER){      
                    int res = evaluation(aff_cel_tempo->arbre,relation_liste_ligne);   
                    // Vérifie si la valeur à mettre à jour existe déjà dans la clé primaire de la table
                    if (aff_cel_tempo->num_att_aff==relation->primarykey){
                        if (search_unique_key(relation,"",res)==1){
                            printf("Erreur: La valeur que vous voulez rajoutez existe déjà\n");
                            exit(1);
                        }
                    }              
                    relation_tete_ligne->val=res;
                } 
                else{ //Le cas où nous sommes
                    // Vérifie si la valeur à mettre à jour existe déjà dans la clé primaire de la table
                    if (aff_cel_tempo->num_att_aff==relation->primarykey){
                        if (search_unique_key(relation,aff_cel_tempo->chaine_aff,0)==1){
                            printf("Erreur: La valeur que vous voulez rajoutez existe déjà\n");
                            exit(1);
                        }
                    }
                    // Libère la mémoire allouée pour la chaîne existante et copie la nouvelle valeur
                    free(relation_tete_ligne->string); 
                    relation_tete_ligne->string=strdup(aff_cel_tempo->chaine_aff);
                }
                aff_cel_tempo=aff_cel_tempo->suiv;
            }
        }
        // Libère la mémoire allouée pour la liste d'affectations
        relation_liste_ligne = relation_liste_ligne->suiv;
    } 
    lib_affectation_cel(aff_cel);
}
