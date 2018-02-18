#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "dataBase.h"
#include "query.h"
#include "logger.h"

#include "keyboard.h"
#include "usecases.h"


void PokemonList(int pokeId){

Query *query = NULL;
char  *field = NULL;
char num[]={Numero};
char name[]={Nom};
char textQuery[255]={"SELECT num,name FROM Pokemon"};

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

  if (query == NULL) {
    ERROR("Erreur dans la requete");
  }

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



}
void PokemonProfil(){
  char pokemonName[20]="\0";



}
void PokemonGenealogy(){
  char pokemonName[20]="\0";



}
void AllCouplingPossibilitiesPokemonList(){
  char pokemonName[20]="\0";



}
void MyCouplingPossibilitiesPokemonList(){
  char pokemonName[20]="\0";



}
void SearchPokemonSameCaract(){
  char pokemonName[20]="\0";



}
void SearchPokemonSameNCaracts(){
  char pokemonName[20]="\0";



}
void AddPokemon(){

  printf("Veuillez saisir le nom du Pokemon que vous souhaitez ajouter\n");
  scanf("%s",pokemonName);



}
void UpdatePokemon(){
  char fieldName[20]="\0";
  char oldFieldValue[20]="\0";
  char newFieldValue[20]="\0";

}
void Administrator(){
  char pokemonName[20]="\0";



}
