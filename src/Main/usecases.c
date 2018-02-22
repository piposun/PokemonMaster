#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"

#include "keyboard.h"
#include "usecases.h"

int choicePokemon(int *pokeId){

  int keyboardTest = 0; /* Variable de test de la selection des cas d'utilisation par l'operateur */
  int requestChoice = 0; /* variable de selection du usecase de l'utilisateur */
  int pokeNum=0;
  char pokeName[sizeNum]={0};
  Query *query = NULL;
  char  *field = NULL;
  char num[]={Numero};
  char name[]={Nom};
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
          textQuery=sprintf("SELECT id FROM Pokemon WHERE name="%s"", pokeName); /* Complete la requete SQL avec les pokeId*/
          query = excuteQuery(dataBase, textQuery); /* Requete SQL */
          *pokeId = getDataQueryById(query, 0, 0);
          break;
        }

      case 2:
        keyboardTest=keyboardInt(&pokeNum, 0, 9999);
        if (keyboardTest == 1) {
          ERROR("\n\n\tProbleme dans la saisie du Pokemon");
          return 1;
        } else {
          textQuery=sprintf("SELECT id FROM Pokemon WHERE num="%d"", pokeNum); /* Complete la requete SQL avec les pokeId*/
          query = excuteQuery(dataBase, textQuery); /* Requete SQL */
          *pokeId = getDataQueryById(query, 0, 0);
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
      for(int i = 0; i < query->nbRecord; i++) {
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
  int fieldInt=0;

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
                memcpy(&fieldInt, field, sizeof(int));
                pokemonList(fieldInt); /* Appel la fonction pokemonList pour remplir le tableau ligne par ligne*/
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



void pokemonProfil(){

  Query *query = NULL;
  char  *field = NULL;

  char pokeName[sizeName]="\0";
  int pokeId=0, choiceTest=0;
  char textQuery[255]={"SELECT * FROM Pokemon"};

  choiceTest=choixPokemon(&pokeId, pokeName);
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
    ERROR("\n\n\tProbleme dans la saisie du numero du Pokemon");
  } else {
    MENU("\nVeuillez saisir le nom du pokemon: ");
    keyboardTest=keyboardChar(name, 20);
    if (keyboardTest == 1) {
      ERROR("\n\n\tProbleme dans la saisie du nom du Pokemon");
    } else {
      MENU("\nVeuillez saisir les points de vie du pokemon: ");
      keyboardTest=keyboardInt(&hp, 0, 9999);
      if (keyboardTest == 1) {
        ERROR("\n\n\tProbleme dans la saisie des points de vie du Pokemon");
      } else {
        MENU("\nVeuillez saisir les points d'attaque du pokemon: ");
        keyboardTest=keyboardInt(&atk, 0, 9999);
        if (keyboardTest == 1) {
          ERROR("\n\n\tProbleme dans la saisie des points d'attaque du Pokemon");
        } else {
          MENU("\nVeuillez saisir les points de defense du pokemon: ");
          keyboardTest=keyboardInt(&def, 0, 9999);
          if (keyboardTest == 1) {
            ERROR("\n\n\tProbleme dans la saisie des points de defense du Pokemon");
          } else {
            MENU("\nVeuillez saisir les points d'attaque speciale du pokemon: ");
            keyboardTest=keyboardInt(&speAtk, 0, 9999);
            if (keyboardTest == 1) {
              ERROR("\n\n\tProbleme dans la saisie des points d'attaque speciale du Pokemon");
            } else {
              MENU("\nVeuillez saisir les points de defense speciale du pokemon: ");
              keyboardTest=keyboardInt(&speDef, 0, 9999);
              if (keyboardTest == 1) {
                ERROR("\n\n\tProbleme dans la saisie des points de defense speciale du Pokemon");
              } else {
                MENU("\nVeuillez saisir la taille du pokemon: ");
                keyboardTest=keyboardInt(&height, 0, 9999);
                if (keyboardTest == 1) {
                  ERROR("\n\n\tProbleme dans la saisie de la taille du Pokemon");
                } else {
                  MENU("\nVeuillez saisir le poids du pokemon: ");
                  keyboardTest=keyboardInt(&weight, 0, 9999);
                  if (keyboardTest == 1) {
                    ERROR("\n\n\tProbleme dans la saisie du poids du Pokemon");
                  } else {
                    MENU("\nVeuillez saisir le type de nature du pokemon: ");
                    keyboardTest=keyboardInt(&nature, 0, 9999);
                    if (keyboardTest == 1) {
                      ERROR("\n\n\tProbleme dans la saisie du type de nature du Pokemon");
                    } else {
                      textQuery=sprintf("SELECT num,name FROM Pokemon WHERE id=%s", pokeId); /* Complete la requete SQL avec les pokeId*/
                    }
                  }
                }
              }
            }
          }
        }
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
  Query *query = NULL;
  char textQuery[255]={"DELETE * FROM Pokemon WHERE id="};
  int poke_id=0, choiceTest=0, validation=0;
  char pokeName[sizeName]={0};


  choiceTest=choixPokemon(&pokeId, pokeName);
  if (choiceTest == 1) {
    ERROR("\n\n\tProbleme dans la saisie du Pokemon");
  } else {
    if (choiceTest == 0) {
      if(pokeId != 0) {
        pokemonProfil()
        MENU("Etes-vous sur de vouloir supprimer le pokemon numero: %d? (0=NON / 1=oui)",pokeId);
        if (keyboardInt(&validation,0,1)==0) {
          if (validation == 1)
            textQuery=sprintf("DELETE * FROM Pokemon WHERE id=%d", pokeId); // Complete la requete SQL avec les pokeId
            query = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
            if (query == NULL) {
              ERROR("Erreur dans la requete");
            } else {
              ???// Que nous retourne la requete en cas de bonne suppression?
            }
          } else {
            MENU("Suppression du pokemon numero: %d annulee",pokeId);
          }
      } else {
      // TODO ajouter la fonction pokemonProfil
      MENU("Etes-vous sur de vouloir supprimer le pokemon %s? (0=NON / 1=oui)",pokeName);
      if (keyboardInt(&validation,0,1)==0) {
        if (validation == 1)
          textQuery=sprintf("DELETE * FROM Pokemon WHERE id=%s", pokeId);
          query = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
          if (query == NULL) {
            ERROR("Erreur dans la requete");
          } else {
            ???// Que nous retourne la requete en cas de bonne suppression?
          }
        } else {
          MENU("Suppression du pokemon numero %s annulee",pokeName);
        }
      }
    }
  }
  closeQuery(query);
}

void administrator(){

}
