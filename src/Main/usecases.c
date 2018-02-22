#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"

#include "keyboard.h"
#include "usecases.h"

int choicePokemon(int *pokeId, DataBase *dataBase){

  int keyboardTest = 0; //Variable de test de la selection des cas d'utilisation par l'operateur
  int requestChoice = 0; // variable de selection du usecase de l'utilisateur
  int pokeNum=0;
  char pokeName[sizeNum]={0};
  Query *query = NULL;
  char  *field = NULL;
  char  *ptPokeId = NULL;
  char num[]="Numero";
  char name[]="Nom";
  char textQuery[255]={0};

  MENU("\nVoulez-vous choisir le Pokemon par :");
  MENU("\n1 - Son Nom");
  MENU("\n2 - Son numéro");
  MENU("\n0 - Retour au menu principal");
  keyboardTest=keyboardInt(&requestChoice, 0, 2);
  if (keyboardTest == 1) {
    ERROR("\n\n\tProbleme dans la saisie du choix");
  }
  else {
    switch (requestChoice) {

      case 1:
        keyboardTest=keyboardChar(pokeName, sizeName);
        if (keyboardTest == 1) {
          ERROR("\n\n\tProbleme dans la saisie du Pokemon");
          return 1;
        } else {
          sprintf(textQuery,"SELECT id FROM Pokemon WHERE name=\"%s\"", pokeName); // Complete la requete SQL avec les pokeId
          query = excuteQuery(dataBase, textQuery); // Requete SQL
          ptPokeId = getDataQueryById(query, 0, 0);
          memcpy(pokeId,ptPokeId, sizeof(int));
          closeQuery(query);
          break;
        }

      case 2:
        keyboardTest=keyboardInt(&pokeNum, 0, 9999);
        if (keyboardTest == 1) {
          ERROR("\n\n\tProbleme dans la saisie du Pokemon");
          return 1;
        } else {
          sprintf(textQuery,"SELECT id FROM Pokemon WHERE num=\"%d\"", pokeNum); // Complete la requete SQL avec les pokeId
          query = excuteQuery(dataBase, textQuery); // Requete SQL
          ptPokeId = getDataQueryById(query, 0, 0);
          memcpy(pokeId,ptPokeId, sizeof(int));
          closeQuery(query);
         break;
        }
      default:
        break;
    }
  }
  return 0;
}

void pokemonList(int pokeId, DataBase *dataBase){

  Query *query = NULL;
  char  *field = NULL;
  char num[]="Numero";
  char name[]="Nom";
  char textQuery[255]={"SELECT num,name FROM Pokemon"};
  int receptPokeId=0;


  if (pokeId==0) { // Le pokemon 0 n'existe pas, c'est donc une clé pour tous les afficher
    query = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
    INFO("\nInventaire des Pokemons dans la base\n");
    INFO("-----------------------------");
    INFO("|%*s|%*s|", sizeNum, num, sizeName, name);
  }
  else{
    sprintf(textQuery,"SELECT num,name FROM Pokemon WHERE id=\"%d\"", pokeId); // Complete la requete SQL avec les pokeId
    query = excuteQuery(dataBase, textQuery); // Requete SQL
  }

  if (query == NULL) {
    ERROR("Erreur dans la requete");
  }
  else{
    if (query->nbRecord>0) {
      for(int i = 0; i < query->nbRecord; i++) {
        for(int j = 0; j < query->descriptor.nbField; j++) {
          field = getDataQueryById(query, i, j);
          switch (getTypeQueryById(query, j)) {
            case DATA_FIELD_INT:
              memcpy(&receptPokeId, field, sizeof(int));
              break;
            case DATA_FIELD_CHAR:
              INFO("| %*d|%*s|", sizeNum, receptPokeId, sizeName, field);
              break;
            default:
              break;
          }
        }
      }
      closeQuery(query);
    }
    else{
      INFO("\nIl n'y a pas de donnees correspondant aux criteres demandes.\n");
      closeQuery(query);
    }
  INFO("-----------------------------");
  }
}

void myPokemonList(DataBase *dataBase){

  Query *query = NULL;
  char  *field = NULL;
  char num[]="Numero";
  char name[]="Nom";
  char textQuery[255]={"SELECT id FROM Pos"};
  int fieldInt=0;

    query = excuteQuery(dataBase, textQuery);

    if (query == NULL) {
      ERROR("Erreur dans la requete");
    }
    else{
      if (query->nbRecord>0) {
        INFO("\nInventaire des Pokemons attrapes\n");
        INFO("-----------------------------");
        INFO("|%*s|%*s|", sizeNum, num, sizeName, name);
        for(int i = 0; i < query->nbRecord; i++) {
          DEBUG("Passage dans la boucle i : %d",i);
          for(int j = 0; j < query->descriptor.nbField; j++) {
            DEBUG("Passage dans la boucle j : %d",j);
            field = getDataQueryById(query, i, j);
            DEBUG("Valeur de id : %d", (int) * field);
            switch (getTypeQueryById(query, j)) {
              case DATA_FIELD_INT:
                DEBUG("Dans la condition INT"); // Affiche les pokeId reçus
                memcpy(&fieldInt, field, sizeof(int));
                DEBUG("Valeur envoyer a pokemonList : %d",fieldInt);
                pokemonList(fieldInt,dataBase);  // Appel la fonction pokemonList pour remplir le tableau ligne par ligne
              break;

              case DATA_FIELD_CHAR:
              {
                ERROR("\nLes donnes retournees ne sont pas des nombres Id\n");
              }
              break;

              default:
              break;
            }
          }
        }
        closeQuery(query);
      } else {
        INFO("\nVous n'avez pas encore attrape de Pokemon.\n");
        closeQuery(query);
      }
    }
}

int pokemonProfil(int pokeId, DataBase *dataBase){

  Query *queryPokemon = NULL;
  Query *queryNature = NULL;
  Query *query = NULL;
  Query *queryGroup = NULL;
  char  *field = NULL;
  char  *label = NULL;
  char pokeName[sizeName]="\0";
  int  choiceTest=0;
  int  natureId  =0;
  int  groupId  =0;
  char textQuery[255]={"SELECT * FROM Pokemon"};

  if (pokeId==0){ // Le pokemon 0 n'existe pas, c'est donc qu'il faut demander le choix du Pokemon
    choiceTest=choicePokemon(&pokeId, dataBase);
    if (choiceTest == 1) {
      ERROR("\n\n\tProbleme dans la saisie du Pokemon");
      return 1; // Erreur
    }
  }
  if (pokeId!=0){
    sprintf(textQuery,"SELECT * FROM Pokemon WHERE id=\"%d\"", pokeId); // Complete la requete SQL avec les pokeId
    queryPokemon = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
    if (queryPokemon == NULL) {
      ERROR("Erreur dans la requete");
      return 1; //Erreur
    }
    else{
      if (queryPokemon->nbRecord>0) {
        for(int i = 0; i < queryPokemon->nbRecord; i++) {
          for(int j = 0; j < queryPokemon->descriptor.nbField; j++) {
            field = getDataQueryById(queryPokemon, i, j);
            label = getNameQueryById(queryPokemon, j);
            switch (getTypeQueryById(queryPokemon, j)) {
              case DATA_FIELD_INT:
                if(strcmp(label,"type")==0){
                  memcpy(&natureId, field, sizeof(int));
                  sprintf(textQuery,"SELECT name FROM Type WHERE id=\"%d\"", natureId); // Complete la requete SQL avec les pokeId
                  queryNature = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
                  field = getDataQueryById(queryNature, 0,0);
                } else {
                  INFO("%*s :  %*d", sizeLabel, label, sizeName, (int)*field);
                  break;
                }
              case DATA_FIELD_CHAR:
                INFO("%*s : %*s", sizeLabel, label, sizeName, field);
                break;
              default:
                break;
            }
          }
        }
        sprintf(textQuery,"SELECT id_group FROM JointGroup WHERE id_species=\"%d\"",pokeId);
          query = excuteQuery(dataBase, textQuery);  // Requete sur la base
          if (query == NULL) {
            ERROR("\nErreur dans la requete sur la table JointGroup\n");
            return 1; //Erreur
          }
          else{
            DEBUG("%d lignes retournees",query->nbRecord);
            if (query->nbRecord>0) {
              for(int i = 0; i < query->nbRecord; i++) {
                DEBUG("Passage dans boucle i : %d",i);
                for(int j = 0; j < query->descriptor.nbField; j++) {
                  field = getDataQueryById(query, i, j);
                  DEBUG("Passage dans boucle j : %d",j);
                  switch (getTypeQueryById(query, j)) {
                    case DATA_FIELD_INT:
                        DEBUG(" Valeur du groupe : %d", (int) * field);
                        memcpy(&groupId, field, sizeof(int));
                        sprintf(textQuery,"SELECT name FROM Group WHERE id=\"%d\"", groupId); // Complete la requete SQL avec les pokeId
                        queryGroup = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
                        field = getDataQueryById(queryGroup, 0,0);
                    case DATA_FIELD_CHAR:
                      INFO("%*s : %*s", sizeLabel, "Groupe", sizeName, field);
                      break;

                    default:
                      break;
                  }
                }
              }
              closeQuery(query);
            }
          }
        closeQuery(queryGroup);
        closeQuery(queryNature);
        closeQuery(queryPokemon);
        }
      }
    }
}

/*
void allCouplingPossibilitiesPokemonList(){
}

void myCouplingPossibilitiesPokemonList(){
}
*/
void deletePokemon(DataBase *dataBase){ // Suppression d'un pokemon par l'admin
  Query *query = NULL; // Pointeur de structure qui recupere les donnees suite a une requete
  char textQuery[255]={0}; // Chaine de caracteres contenant la requete SQL
  int pokeId=0, choiceTest=0, validation=0;
  char pokeName[sizeName]={0};


  choiceTest=choicePokemon(&pokeId,dataBase); // On appel la fonction pour selectionner l'id du pokemon demander par l'admin
  if (choiceTest == 1) {
    ERROR("\n\n\tProbleme dans la saisie du Pokemon");
  } else {
    if (choiceTest == 0) {
      if(pokeId != 0) {
        pokemonProfil(pokeId,dataBase); // On affiche le profil complet du pokemon
        MENU("\nEtes-vous sur de vouloir supprimer le pokemon? (0=non / 1=oui)");
        if (keyboardInt(&validation,0,1)==0) {
          if (validation == 1) {
            sprintf(textQuery,"DELETE FROM Pokemon WHERE id=\"%d\"", pokeId);
            query = excuteQuery(dataBase, textQuery);
            if (query == NULL) {
              MENU("Pokemon supprime");
            } else {
              ERROR("Erreur dans la requete");
            }
          } else {
            MENU("Suppression du pokemon annulee");
          }
        } else {
          MENU("\nProbleme dans la saisie de la validation");
        }
      }
    }
  }

}
/*
void addPokemon(){

}

void updatePokemon(){
}
*/

void updatePokemonList(DataBase *dataBase){
  Query *query = NULL; // Pointeur de structure qui recupere les donnees suite a une requete
  char textQuery[255]={0}; // CHaine de caracteres contenant la requete SQL
  int pokeId=0, choiceTest=0, validation=0;
  char pokeName[sizeName]={0};


  choiceTest=choicePokemon(&pokeId,dataBase); // On appel la fonction pour selectionner l'id du pokemon
  if (choiceTest == 1) {
    ERROR("\n\n\tProbleme dans la saisie du Pokemon");
  } else {
    if (choiceTest == 0) {
      if(pokeId != 0) {
        pokemonProfil(pokeId,dataBase); // On affiche le profil complet du pokemon
        MENU("\nEtes-vous sur de vouloir ajouter le pokemon? (0=non / 1=oui)");
        if (keyboardInt(&validation,0,1)==0) {
          if (validation == 1) {
            sprintf(textQuery,"INSERT INTO Pos VALUES (\"%d\")", pokeId);
            query = excuteQuery(dataBase, textQuery);
            if (query == NULL) {
              MENU("Pokemon ajoute");
            } else {
              ERROR("Erreur dans la requete");
            }
          } else {
            MENU("Ajout du pokemon annulee");
          }
        } else {
          MENU("\nProbleme dans la saisie de la validation");
        }
      }
    }
  }
}

void administrator(DataBase *dataBase){
Query *query = NULL;

restoreTables(dataBase);

query = excuteQuery(dataBase, "SELECT * FROM Pokemon WHERE id=1");

  if (query != NULL) {
    for(int i = 0; i < query->nbRecord; i++) {
      char * field;
      for(int j = 0; j < query->descriptor.nbField; j++) {
        field = getDataQueryById(query, i, j);
        switch (getTypeQueryById(query, j)) {
          case DATA_FIELD_PK:
          case DATA_FIELD_INT:
          {
            DEBUG("Champ %d %s : %d", j+1, getNameQueryById(query, j), (int)*field);
          }
          break;
          case DATA_FIELD_CHAR:
          {
            DEBUG("Champ %d %s : %s",j+1, getNameQueryById(query, j), field);
          }
          break;
          default:
          break;
        }
      }
    }

    closeQuery(query);

  }
}
