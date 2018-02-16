#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<keyboard.h>
#include<requests.h>
#include<usecases.h>


void UserRequestsMenu(void); /*prototype fonction: Affichage menu*/
void UserRequestsSelection(int *requestChoice);/*switch de sélection des usecases*/
void PokemonCaractsMenu(void); /*prototype fonction: Affichage composition détaillée d'un pokémon*/
char *PokemonCaractSelection(int *caractChoice);/*switch de sélection des usecases*/
void Display(???); /*fonction d'affichage des résultats*/

int main(void) {
  long requestChoice=0, caractChoice=0; /*variables de selections de l'utilisateur dans les menus*/
  char pokemonName[20]="\0";
  do {
    UserRequestsMenu(); /*fonction d'affichage du menu*/
    UserRequestsSelection(&requestChoice);
  } while(requestChoice!=0);
  return 0;
}
void UserRequestsMenu(){
  printf("\n### MENU POKEMONS ###\n");
  printf("0  --> Quitter\n");
  printf("1  --> Afficher tous les pokemons [noms + id]\n");
  printf("2  --> Afficher mes pokemons [noms + id]\n");
  printf("3  --> Afficher le profil detaille d'un pokemon\n");
  printf("4  --> Afficher la genealogie detaillee d'un pokemon\n");
  printf("5  --> Afficher la liste des compatibilites d'accouplement d'un pokemon\n");
  printf("6  --> Afficher la liste des pokemons possedes et compatibles en accouplement d'un pokemon\n");
  printf("7  --> Rechercher tous les pokemons avec une caracteristique commune\n");
  printf("8  --> Rechercher tous les pokemons avec n caracteristiques communes\n");
  printf("9  --> Ajouter un pokemon attrape dans ma liste de pokemons possedes\n");
  printf("10 --> Modifier une caracteristique d'un pokemon\n");
  printf("11 --> Passer en mode administrateur de la base de donnees\n");
}
void UserRequestsSelection(int *requestChoice){
  do {
    do{
      *requestChoice=KeyboardLong();
      if(*requestChoice<0 && *requestChoice>11)
        printf("Veuillez saisir un nombre compris dans le menu de selection\n");
    }while(*requestChoice<0 && *requestChoice>11)

    switch (*requestChoice) { /*switch de sélection des cas d'utilisations des requêtes SQL*/
      case 0: /*Sortie du programme suite choix opérateur*/
        printf("\nSortie du programme\n");
        break;
      case 1: /*Afficher la liste de noms de tous les pokemons*/
          PokemonList();
          break;
      case 2: /*Afficher la liste des pokemons possedes*/
          MyPokemonList();
          break;
      case 3: /*Afficher le profil detaille d'un pokemon*/
          PokemonProfil();
          break;
      case 4: /*Afficher la genealogie detaillee d'un pokemon*/
          PokemonGenealogy();
          break;
      case 5: /*Acceder a la liste des compatibilites d'accouplement d'un pokemon*/
          AllCouplingPossibilitiesPokemonList();
          break;
      case 6: /*Acceder a la liste des pokemons possedes et compatibles en accouplement d'un pokemon*/
          MyCouplingPossibilitiesPokemonList();
          break;
      case 7: /*Rechercher tous les pokemons avec une caracteristique commune*/
          SearchPokemonSameCaract();
          break;
      case 8: /*Rechercher tous les pokemons avec n caracteristiques communes*/
          SearchPokemonSameNCaracts();
          break;
      case 9: /*Ajouter un pokemon dans la base*/
        AddPokemon();
        break;
      case 10: /*Modifier une caracteristique d'un pokemon*/
        UpdatePokemon();
        break;
      case 11: /*Passer en mode administrateur*/
        Administrator();
        break;
      default:
        break;
  }
}
void PokemonCaractsMenu(void){
  printf("\n### CARACTERISTIQUES POKEMONS ###\n");
  printf("1  --> Nature\n");
  printf("2  --> Groupe\n");
  printf("3  --> Niveau\n");
  printf("4  --> Points de vie\n");
  printf("5  --> Attaque\n");
  printf("6  --> Defense\n");
  printf("7  --> Attaque speciale\n");
  printf("8  --> Defense speciale\n");
  printf("9  --> Taille\n");
  printf("10 --> Poids\n");
  printf("12 --> Description\n");
  printf("13 --> Genealogie\n");
  printf("14 --> Evolution\n");
  printf("15 --> Accouplement\n");
}
void *PokemonCaractSelection(int *caractChoice){
  char caract[20]={0};
  do {
    do{
      *caractChoice=KeyboardLong();
      if(*caractChoice<1 && *caractChoice>15)
        printf("\nVeuillez saisir un nombre compris dans le menu de selection\n");
    }while(*caractChoice<1 && *caractChoice>15)

    switch (*caractChoice) { /*switch de sélection d'une caractéristique*/
      case 1: /*Nature*/
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
