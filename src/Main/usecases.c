#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"

#include "keyboard.h"
#include "usecases.h"


<<<<<<< HEAD
void PokemonList(int pokeId){
=======
void pokemon_list(int pokeId){
>>>>>>> dev_philippe

Query *query = NULL;
char  *field = NULL;
char num[]={Numero};
char name[]={Nom};
char textQuery[255]={"SELECT num,name FROM Pokemon"};

<<<<<<< HEAD
if (pokeId==0) { /* Le pokemon 0 n'existe pas, c'est donc une clé pour tous les afficher */
  query = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
  INFO("\nInventaire des Pokemons dans la base\n");
  INFO("\n---------------------------------\n");
  INFO("\n| %*s", SizeNum, num);
  INFO("| %*s|\n", SizeName, name);
}
else{
  textQuery=sprintf("SELECT num,name FROM Pokemon WHERE id=%s", pokeId); /* Complete la requete SQL avec les pokeId*/
  query = excuteQuery(dataBase, textQuery); /* Requete SQL */
}
=======
  if (pokeId==0) { /* Le pokemon 0 n'existe pas, c'est donc une clé pour tous les afficher */
    query = excuteQuery(dataBase, textQuery);  // Requete sur l'ensemble de la base
    INFO("\nInventaire des Pokemons dans la base\n");
    INFO("\n---------------------------------\n");
    INFO("\n| %*s", SizeNum, num);
    INFO("| %*s|\n", SizeName, name);
  }
  else{
    textQuery=sprintf("SELECT num,name FROM Pokemon WHERE id=%s", pokeId); /* Complete la requete SQL avec les pokeId*/
    query = excuteQuery(dataBase, textQuery); /* Requete SQL */
  }
>>>>>>> dev_philippe

  if (query == NULL) {
    ERROR("Erreur dans la requete");
  }
<<<<<<< HEAD

  else {
    for(int i = 1; i < query->nbRecord; i++) { //Commence à 1 pour ignorer le descripteur
      for(int j = 0; j < query->descriptor.nbField; j++) {
        field = getDataQueryById(query, i, j);
        switch (getTypeQueryById(query, j)) {
          case 0:
          {
            INFO("\nIl n'y a pas de donnees correspondant aux criteres demandes.\n");
          }
          case DATA_FIELD_INT:
          {
            INFO("| %*d", SizeNum, (int)*field);
          }
          break;
          case DATA_FIELD_CHAR:
          {
            INFO("| %*s|\n",SizeName, field);
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



void MyPokemonList(void){

  Query *query = NULL;
  char  *field = NULL;
  char textQuery[255]={"SELECT num,name FROM Pokemon WHERE id="};

    query = excuteQuery(dataBase, "SELECT poke_id FROM Pos");

    if (query == NULL) {
      ERROR("Erreur dans la requete");
    }
    else {
      INFO("\nInventaire des Pokemons attrapes\n");
      INFO("\n--------------------------------\n");
      INFO("\n| %*s", SizeNum, num);
      INFO("| %*s|\n", SizeName, name);
      for(int i = 1; i < query->nbRecord; i++) { //Commence à 1 pour ignorer le descripteur
        for(int j = 0; j < query->descriptor.nbField; j++) {
          field = getDataQueryById(query, i, j);
          switch (getTypeQueryById(query, j)) {
            case 0:
            {
              INFO("\nIl n'y a pas de donnees correspondant aux criteres demandes.\n");
            }
            case DATA_FIELD_INT:
            {
              DEBUG("%c, ", field); /* Affiche les pokeId reçus */
              void PokemonList(int (int)*field);
            }
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
    }
=======
  else{
    if (query->nbRecord>0) {
      for(int i = 1; i < query->nbRecord; i++) { //Commence à 1 pour ignorer le descripteur
        for(int j = 0; j < query->descriptor.nbField; j++) {
          field = getDataQueryById(query, i, j);
          switch (getTypeQueryById(query, j)) {
            case DATA_FIELD_INT:
            {
              INFO("| %*d", SizeNum, (int)*field);
            }
            break;
            case DATA_FIELD_CHAR:
            {
              INFO("| %*s|\n",SizeName, field);
            }
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
>>>>>>> dev_philippe

void my_pokemon_list(void){

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
        INFO("\n| %*s", SizeNum, num);
        INFO("| %*s|\n", SizeName, name);

        for(int i = 0; i < query->nbRecord; i++) {
          for(int j = 0; j < query->descriptor.nbField; j++) {
            field = getDataQueryById(query, i, j);
            switch (getTypeQueryById(query, j)) {
              case 0:
              {
                INFO("\nIl n'y a pas de donnees correspondant aux criteres demandes.\n");
              }
              case DATA_FIELD_INT:
              {
                DEBUG("%c, ", field); /* Affiche les pokeId reçus */
                void pokemon_list(int (int)*field);
              }
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
      }
      else{
        INFO("\nVous n'avez pas encore attrape de Pokemon.\n");
        closeQuery(query);
      }
    }
}

void pokemon_profil(){

  char pokemonName[20]="\0";
  char
  int keyboardTest = 0; /* Variable de test de la selection des cas d'utilisation par l'operateur */
  int requestChoice = 0; /* variable de selection du usecase de l'utilisateur */

  MENU("\nVoulez-vous choisir le Pokemon par :");
  MENU("\n1 - Son Nom")
  MENU("\n2 - Son numéro")
  MENU("\n0 - Retour au menu principal")
  keyboardTest=KeyboardInt(requestChoice, 0, 2);

  

}
void PokemonGenealogy(){
  char pokemonName[20]="\0";



}
void all_coupling_possibilities_pokemon_list(){
  char pokemonName[20]="\0";



}
void my_coupling_possibilities_pokemon_list(){
  char pokemonName[20]="\0";



}
void SearchPokemonSameCaract(){
  char pokemonName[20]="\0";



}
void SearchPokemonSameNCaracts(){
  char pokemonName[20]="\0";



}
void add_pokemon(){

  printf("Veuillez saisir le nom du Pokemon que vous souhaitez ajouter\n");
  scanf("%s",pokemonName);



}
void update_pokemon(){
  char fieldName[20]="\0";
  char oldFieldValue[20]="\0";
  char newFieldValue[20]="\0";

}
void administrator(){
  char pokemonName[20]="\0";



}
