#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"menu.h"
#include"keyboard.h"
#include"usecases.h"
#include"logger.h"
#include"dataBase.h"

void menu(DataBase *dataBase) {

  int requestChoice = 0; /* variable de selection du usecase de l'utilisateur */

  do { /* Boucle tant que l'utilisateur ne quitte pas le programme */
    userRequestsMenu(); /* fonction d'affichage du menu usecases*/
    userRequestsSelection(&requestChoice, dataBase); /* fonction de selection des usecases*/
  } while(requestChoice != 0);
}

void userRequestsMenu() { /* Menu d'affichage des cas d'utilisation de la base de donnee*/
  MENU("\n\t\t*** MENU POKEMONS ***\n");
  MENU("0 --> Quitter le programme\n");
  MENU("1 --> Afficher la liste de tous les pokemons [numero + noms]\n");
  MENU("2 --> Afficher le profil detaille d'un pokemon\n");
  MENU("3 --> Ajouter un pokemon a ma collection personnelle\n");
  MENU("4 --> Afficher la liste de mes pokemons [numero + noms]\n");
  //MENU("5 --> Afficher la liste des compatibilites d'accouplement d'un pokemon\n");
  //MENU("6 --> Afficher la liste des pokemons possedes et compatibles en accouplement d'un pokemon\n");
  //MENU("7 --> Modifier une caracteristique d'un pokemon dans la base de donnees\n");
  //MENU("8 --> Administrateur: Ajouter un pokemon dans la base de donnees\n");
  MENU("9 --> Administrateur: Supprimer un pokemon dans la base de donnees\n");
  MENU("10--> Administrateur: Charger la base de donnees\n");
}

void userRequestsSelection(int *requestChoice, DataBase *dataBase) {
  int keyboardTest = 0; /* Variable de test de la selection des cas d'utilisation par l'operateur */
  int password = 0, validation=0; /* Variable contenant le mot de passe saisi par l'administrateur*/

      keyboardTest = keyboardInt(requestChoice, 0, 10);

      if (keyboardTest == 1) {
        ERROR("\n\n\tProbleme dans la saisie d'un cas d'utilisation");
      }
      else {
        switch (*requestChoice) { /* switch de selection des cas d'utilisations des requêtes SQL */
          case 0: /* Sortie du programme suite choix operateur */
            MENU("\nSortie du programme\n");
            break;
          case 1: /* Afficher la liste des numeros + noms de tous les pokemons */
            pokemonList(0,dataBase); /* 0 indique liste complete sans selection */
            break;
          case 2: /* Afficher le profil detaille d'un pokemon */
            pokemonProfil(0,dataBase); // 0 indique qu'il faudra demander quel Pokemon afficher
            break;
          case 3: /* Ajouter un pokemon a ma collection personnelle */
            updatePokemonList(dataBase);
            break;
          case 4: /* Afficher la liste des pokemons possedes */
            myPokemonList(dataBase);
            break;
          case 5: /* Acceder a la liste des compatibilites d'accouplement d'un pokemon */
            //allCouplingPossibilitiesPokemonList(dataBase);
            break;
          case 6: /* Acceder a la liste des pokemons possedes et compatibles en accouplement d'un pokemon */
            //myCouplingPossibilitiesPokemonList(dataBase);
            break;
          case 7: /* Modifier une caracteristique d'un pokemon */
            //updatePokemon(dataBase);
            break;
          case 8: /* Ajouter un pokemon dans la base */
            MENU("\n\tSaisir le mot de passe administrateur (666)\n");

            if (keyboardInt(&password,0,999)==0) {
              if(password==666){
                //addPokemon(dataBase);
                break;
              }
              else
                ERROR("\nMot de passe incorrect\n");
              break;
            }
            else {
              ERROR("\n\n\tProbleme dans la saisie du mot de passe");
              break;
            }
          case 9: /* Supprimer un pokemon dans la base */
          MENU("\n\tSaisir le mot de passe administrateur (666)\n");

          if (keyboardInt(&password,0,999)==0) {
            if(password==666){
              deletePokemon(dataBase);
              break;
            }
            else
              ERROR("\nMot de passe incorrect\n");
            break;
          }
          else {
            ERROR("Probleme dans la saisie du mot de passe");
            break;
          }
          case 10: /* Passer en mode administrateur pour charger la base de donnees */
          MENU("\n\tSaisir le mot de passe administrateur (999)\n");

          if (keyboardInt(&password,0,999)==0) {
            if(password==999){
              MENU("Voulez vous restaurer la base de donnee? (0=non / 1=oui)");
              if (keyboardInt(&validation,0,1)==0) {
                if (validation == 1) {
                  administrator(dataBase);
                  break;
                } else {
                  MENU("\nRestauration annulee");
                  break;
                }
              } else {
                ERROR("\nMot de passe incorrect\n");
                break;
              }
            }else {
              ERROR("\n\n\tProbleme dans la saisie du mot de passe");
              break;
            }
            }
          default:
            break;
        }
        }
}
