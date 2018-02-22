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
         break;
        }
      default:
        break;
    }
  }
  closeQuery(query);
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
          for(int j = 0; j < query->descriptor.nbField; j++) {
            field = getDataQueryById(query, i, j);
            switch (getTypeQueryById(query, j)) {

              case DATA_FIELD_INT:
                DEBUG("%c, ", field); // Affiche les pokeId reçus
                memcpy(&fieldInt, field, sizeof(int));
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
/*
void pokemonProfil(){

  Query *query = NULL;
  char  *field = NULL;

  char pokeName[sizeName]="\0";
  int pokeId=0, choiceTest=0;
  char textQuery[255]={"SELECT * FROM Pokemon"};

  choiceTest=choicePokemon(&pokeId);
  if (pokeId!=0){ // Le pokemon 0 n'existe pas, c'est donc une clé pour traiter par nom
    textQuery=sprintf("SELECT * FROM Pokemon WHERE id=%d", pokeId); // Complete la requete SQL avec les pokeId
    query = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
  }
  else{
    textQuery=sprintf("SELECT * FROM Pokemon WHERE name=%s", pokeName); // Complete la requete SQL avec les pokeId
    query = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
  }

}

void allCouplingPossibilitiesPokemonList(){
}

void myCouplingPossibilitiesPokemonList(){
}
*/
void deletePokemon(DataBase *dataBase){ // Suppression d'un pokemon par l'admin
  Query *query = NULL; // Pointeur de structure qui recupere les donnees suite a une requete
  char textQuery[255]={0}; // CHaine de caracteres contenant la requete SQL
  int pokeId=0, choiceTest=0, validation=0;
  char pokeName[sizeName]={0};


  choiceTest=choicePokemon(&pokeId,dataBase); // On appel la fonction pour selectionner l'id du pokemon demander par l'admin
  DEBUG ("%d pokeId",pokeId);
  if (choiceTest == 1) {
    ERROR("\n\n\tProbleme dans la saisie du Pokemon");
  } else {
    if (choiceTest == 0) {
      if(pokeId != 0) {
        //pokemonProfil(pokeId,dataBase); // On affiche le profil complet du pokemon
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
  closeQuery(query);
}
/*
void addPokemon(){


}

void updatePokemon(){
}
*/

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
