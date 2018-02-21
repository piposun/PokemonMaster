#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"

#include "keyboard.h"
#include "usecases.h"

int choixPokemon(int *pokeId, char *pokeName){

  int keyboardTest = 0; /* Variable de test de la selection des cas d'utilisation par l'operateur */
  int requestChoice = 0; /* variable de selection du usecase de l'utilisateur */

  MENU("\nVoulez-vous choisir le Pokemon par :");
  MENU("\n1 - Son Nom")
  MENU("\n2 - Son numéro")
  MENU("\n0 - Retour au menu principal")
  keyboardTest=KeyboardInt(&requestChoice, 0, 2);
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
          break;
        }

      case 2:
        keyboardTest=keyboardInt(pokeId, 0, 9999);
        if (keyboardTest == 1) {
          ERROR("\n\n\tProbleme dans la saisie du Pokemon");
          return 1;
        } else {
         break;
        }

      default:
        break;
    }
  }
  return 0;
}

void pokemonList(int pokeId){

  Query *query = NULL;
  char  *field = NULL;
  char num[]={Numero};
  char name[]={Nom};
  char textQuery[255]={"SELECT num,name FROM Pokemon"};

  if (pokeId==0) { /* Le pokemon 0 n'existe pas, c'est donc une clé pour tous les afficher */
    query = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
    INFO("\nInventaire des Pokemons dans la base\n");
    INFO("\n---------------------------------\n");
    INFO("\n| %*s", sizeNum, num);
    INFO("| %*s|\n", sizeName, name);
  }
  else{
    textQuery=sprintf("SELECT num,name FROM Pokemon WHERE id=%s", pokeId); /* Complete la requete SQL avec les pokeId*/
    query = excuteQuery(dataBase, textQuery); /* Requete SQL */
  }

  if (query == NULL) {
    ERROR("Erreur dans la requete");
  }
  else{
    if (query->nbRecord>0) {
      for(int i = 1; i < query->nbRecord; i++) { //Commence à 1 pour ignorer le descripteur
        for(int j = 0; j < query->descriptor.nbField; j++) {
          field = getDataQueryById(query, i, j);
          switch (getTypeQueryById(query, j)) {
            case DATA_FIELD_INT:
              INFO("| %*d", sizeNum, (int)*field);
              break;
            case DATA_FIELD_CHAR:
              INFO("| %*s|\n",sizeName, field);
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
  }
}

void myPokemonList(void){

  Query *query = NULL;
  char  *field = NULL;
  char num[]={Numero};
  char name[]={Nom};
  char textQuery[255]={"SELECT poke_id FROM Pos"};

    query = excuteQuery(dataBase, textQuery);

    if (query == NULL) {
      ERROR("Erreur dans la requete");
    }
    else{
      if (query->nbRecord>0) {
        INFO("\nInventaire des Pokemons attrapes\n");
        INFO("\n--------------------------------\n");
        INFO("\n| %*s", sizeNum, num);
        INFO("| %*s|\n", sizeName, name);

        for(int i = 0; i < query->nbRecord; i++) {
          for(int j = 0; j < query->descriptor.nbField; j++) {
            field = getDataQueryById(query, i, j);
            switch (getTypeQueryById(query, j)) {

              case DATA_FIELD_INT:
                DEBUG("%c, ", field); /* Affiche les pokeId reçus */
                void pokemonList(int (int)*field); /* Appel la fonction pokemonList pour remplir le tableau ligne par ligne*/
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

  choiceTest=choixPokemon(&pokeId, pokeName)
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

void addPokemon(){

  int id=0, num=0, hp=0, atk=0, def=0, speAtk=0, speDef=0, height=0, weight=0, nature=0;
  char name[20]={0}, desc[255]={0};
  int keyboardTest=0;

  MENU("\nVeuillez saisir le numero du pokemon: ");
  keyboardTest=keyboardInt(&num, 0, 9999);

  if (keyboardTest == 1) {
    ERROR("\n\n\tProbleme dans la saisie du Pokemon");
  }
  else {
    MENU("\nVeuillez saisir les points de vie du pokemon: ");
    keyboardTest=keyboardInt(&hp, 0, 9999);

    if (keyboardTest == 1) {
      ERROR("\n\n\tProbleme dans la saisie du Pokemon");
    }
    else {
      MENU("\nVeuillez saisir les points d'attaque du pokemon: ");
      keyboardTest=keyboardInt(&atk, 0, 9999);

      if (keyboardTest == 1) {
        ERROR("\n\n\tProbleme dans la saisie du Pokemon");
      }
      else {
        MENU("\nVeuillez saisir les points d'attaque du pokemon: ");
        keyboardTest=keyboardInt(&atk, 0, 9999);
      }
    }
  }
}

void updatePokemon(){
  char fieldName[20]="\0";
  char oldFieldValue[20]="\0";
  char newFieldValue[20]="\0";

}

void deletePokemon(){
}

void administrator(){

}

*/
