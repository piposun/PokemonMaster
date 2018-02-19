#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"menu.h"
#include"keyboard.h"
#include"usecases.h"
#include"logger.h"

int main(void) {

  int requestChoice = 0; /* variable de selection du usecase de l'utilisateur */

  do { /* Boucle tant que l'utilisateur ne quitte pas le programme */
    user_requests_menu(); /* fonction d'affichage du menu usecases*/
    user_requests_selection(&requestChoice); /* fonction de selection des usecases*/
  } while(requestChoice != 0);

  return 0;
}

void user_requests_menu() { /* Menu d'affichage des cas d'utilisation de la base de donnee*/
  MENU("\n\t\t*** MENU POKEMONS ***\n");
  MENU("\t0 --> Quitter le programme\n");
  MENU("\t1 --> Afficher la liste de tous les pokemons [numero + noms]\n");
  MENU("\t2 --> Afficher le profil detaille d'un pokemon\n");
  MENU("\t3 --> Ajouter un pokemon a ma collection personnelle\n");
  MENU("\t4 --> Afficher la liste de mes pokemons [numero + noms]\n");
  MENU("\t5 --> Afficher la liste des compatibilites d'accouplement d'un pokemon\n");
  MENU("\t6 --> Afficher la liste des pokemons possedes et compatibles en accouplement d'un pokemon\n");
  MENU("\t7 --> Modifier une caracteristique d'un pokemon dans la base de donnees\n");
}

void user_requests_selection(int *requestChoice) {
  int keyboardTest = 0; /* Variable de test de la selection des cas d'utilisation par l'operateur */

      keyboardTest = KeyboardInt(requestChoice, 0, 10);

      if (keyboardTest == 1) {
        ERROR("\n\n\tProbleme dans la saisie d'un cas d'utilisation");
      }
      else {
        switch (*requestChoice) { /* switch de selection des cas d'utilisations des requêtes SQL */
          case 0: /* Sortie du programme suite choix operateur */
            MENU("\nSortie du programme\n");
            break;
          case 1: /* Afficher la liste des numeros + noms de tous les pokemons */
            pokemon_list(0); /* 0 indique liste complete sans selection */
            break;
          case 2: /* Afficher le profil detaille d'un pokemon */
            pokemon_profil();
            break;
          case 3: /* Ajouter un pokemon a ma collection personnelle */
            update_pokemon_list();
            break;
          case 4: /* Afficher la liste des pokemons possedes */
            my_pokemon_list();
            break;
          case 5: /* Acceder a la liste des compatibilites d'accouplement d'un pokemon */
            all_coupling_possibilities_pokemon_list();
            break;
          case 6: /* Acceder a la liste des pokemons possedes et compatibles en accouplement d'un pokemon */
            my_coupling_possibilities_pokemon_list();
            break;
          case 7: /* Modifier une caracteristique d'un pokemon */
            update_pokemon();
            break;
          case 8: /* Ajouter un pokemon dans la base */
            add_pokemon();
            break;
          case 9: /* Supprimer un pokemon dans la base */
            delete_pokemon();
            break;
          case 10: /* Passer en mode administrateur pour charger la base de donnees */
            administrator();
            break;
          default:
            break;
        }
      }
}
