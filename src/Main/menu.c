#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<menu.h>
#include<keyboard.h>
#include<usecases.h>
#include "logger.h"

int main(void) {
  long requestChoice=0, caractChoice=0; /* variables de selections de l'utilisateur dans les menus */
  char pokemonName[20]="\0";
  do {
    UserRequestsMenu(); /* fonction d'affichage du menu */
    UserRequestsSelection(&requestChoice);
  } while(requestChoice!=0);
  return 0;
}
void UserRequestsMenu(){
  MENU("\n\t\t*** MENU POKEMONS ***\n");
  MENU("\t0 --> Quitter le programme\n");
  MENU("\t1 --> Afficher tous les pokemons [numero + noms]\n");
  MENU("\t2 --> Afficher le profil detaille d'un pokemon\n");
  MENU("\t3 --> Ajouter un pokemon a ma collection personnelle\n");
  MENU("\t4 --> Afficher la liste de mes pokemons [numero + noms]\n");
  MENU("\t5 --> Afficher la liste des compatibilites d'accouplement d'un pokemon\n");
  MENU("\t6 --> Afficher la liste des pokemons possedes et compatibles en accouplement d'un pokemon\n");
  MENU("\t7 --> Modifier une caracteristique d'un pokemon dans la base de donnees\n");
  MENU("\t8 --> Ajouter un pokemon dans la base de donnees\n");
  MENU("\t9 --> Supprimer un pokemon dans la base de donnees\n");
}
void UserRequestsSelection(int *requestChoice){
  do {
    do{
      *requestChoice=KeyboardLong();
      if(*requestChoice<0 && *requestChoice>11)
        MENU("\nVeuillez saisir un nombre compris dans le menu de selection\n");
    }while(*requestChoice<0 && *requestChoice>11)

    switch (*requestChoice) { /* switch de selection des cas d'utilisations des requêtes SQL */
      case 0: /* Sortie du programme suite choix operateur */
        MENU("\nSortie du programme\n");
        break;
      case 1: /* Afficher la liste des numeros + noms de tous les pokemons */
          PokemonList();
          break;
      case 3: /* Afficher le profil detaille d'un pokemon */
          PokemonProfil();
          break;
      case 9: /* Ajouter un pokemon dans la base */
          AddPokemon();
          break;
      case 2: /* Afficher la liste des pokemons possedes */
          MyPokemonList();
          break;
      case 5: /* Acceder a la liste des compatibilites d'accouplement d'un pokemon */
          AllCouplingPossibilitiesPokemonList();
          break;
      case 6: /* Acceder a la liste des pokemons possedes et compatibles en accouplement d'un pokemon */
          MyCouplingPossibilitiesPokemonList();
          break;
      case 666: /* Ajouter un pokemon dans la base */
        AddPokemon();
        break;
      case : /* Modifier une caracteristique d'un pokemon */
        UpdatePokemon();
        break;
      case 1000: /* Passer en mode administrateur */
        Administrator();
        break;
      default:
        break;
  }
}
void PokemonCaractsMenu(void){
  MENU("\n### CARACTERISTIQUES POKEMONS ###\n");
  MENU("1  --> Nature\n");
  MENU("2  --> Groupe\n");
  MENU("3  --> Niveau\n");
  MENU("4  --> Points de vie\n");
  MENU("5  --> Attaque\n");
  MENU("6  --> Defense\n");
  MENU("7  --> Attaque speciale\n");
  MENU("8  --> Defense speciale\n");
  MENU("9  --> Taille\n");
  MENU("10 --> Poids\n");
  MENU("12 --> Description\n");
  MENU("13 --> Genealogie\n");
  MENU("14 --> Evolution\n");
  MENU("15 --> Accouplement\n");
}
void *PokemonCaractSelection(int *caractChoice){
  char caract[20]={0};
  do {
    do{
      *caractChoice=KeyboardLong();
      if(*caractChoice<1 && *caractChoice>15)
        MENU("\nVeuillez saisir un nombre compris dans le menu de selection\n");
    }while(*caractChoice<1 && *caractChoice>15)

    switch (*caractChoice) { /* switch de selection d'une caracteristique */
      case 1: /* Nature */
          strcpy(caract,"Nature");
          break;
      case 2:
          strcpy(caract,"Groupe");
          break;
      case 3:
          strcpy(caract,"Niveau");
          break;
      case 4:
          strcpy(caract,"Nature");
          break;
      case 5:
          strcpy(caract,"Nature");
          break;
      case 6:
          strcpy(caract,"Nature");
          break;
      case 7:
          strcpy(caract,"Nature");
          break;
      case 8:
          strcpy(caract,"Nature");
          break;
      case 9:
          strcpy(caract,"Nature");
          break;
      case 10:
          strcpy(caract,"Nature");
          break;
      case 11:
          strcpy(caract,"Nature");
          break;
      case 12:
            strcpy(caract,"Nature");
          break;
      case 13:
          strcpy(caract,"Nature");
          break;
      case 14:
          strcpy(caract,"Nature");
          break;
      case 15:
          strcpy(caract,"Nature");
          break;
      default:
        break;
  }
  return caract;
}
