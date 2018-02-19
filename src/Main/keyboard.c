#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"keyboard.h"
#include"logger.h"

int clear_buffer(void){
  int buffer=0, i=0;
  while( buffer=getchar() != '\n' && buffer != EOF) { /*  On parcours le buffer jusqu'à la fin pour le decharger dans la variable temporaire */
    i++; /* Si la variable de comptage "i" s'est incrementee, c'est qu'il restait des caracteres dans le buffer stdin*/
  }
  return i;
}

int keyboard_char(char *string, int nbCharMax) { /*  On passe à la fonction la variable qui va contenir la chaine de caractere et le nombre de caracteres maximum*/

      char *charReturn = NULL; /* Pointeur qui va pointer l'adresse du caractere "\n" */
      int max=0;

      if (nbCharMax==0) { /* Si nbCharMax=0, on identifie une demande de saisie de nombres */
        nbCharMax=nbDigitMax;  // 5 équivalent à 99999
      }
      else{ /* Sinon c'est une demande de chaine de caracteres */
        printf("\nVeuillez saisir vos donnees au clavier, attention %d caracteres maximum : ",nbCharMax-1) ;
      }

      do {
        if (fgets(string, nbCharMax, stdin) != NULL) { /*  Lecture de la saisie clavier avec test d'erreur */
          charReturn = strchr(string, '\n'); /*  Recherche du caractere "\n" genere lors de la saisie par le "entrer" operateur) */
            if (charReturn != NULL) { /*  Si on trouve le \n  */
              *charReturn = '\0'; /*  On le remplace par caractere "\0" */
              max=0; /* Necessaire en cas de boucle */
            }
            else{ /* Si on n'a pas trouver le /n, alors le buffer n'est pas vide */
              max=clear_buffer();/*  Donc on vide le buffer  */
            }
        }
        else{  /* Si erreur de la commande de saisie */
            max=clear_buffer(); /* Par sécurite, on vide le buffer */
            return 1; /* Erreur de saisie, retourne 1 */
        }

        if(strlen(string)==0) {
          printf("\nVous devez saisir au moins un caractère. Veuillez saisir une donnee correcte : ");}
        else{
          if(max>0) {
            printf("\nVous avez saisi trop de caractère. Veuillez saisir une donnee correcte : ");
          }
        }

      }while (max>0 || strlen(string)==0); /*On boucle si la saisie operateur est vide ou superieur au nombre de caractere maximum*/

      return 0; /* Saisie sans erreur on retourne 0 */
}

int keyboard_int(int *number, int min, int max) { /*  On passe à la fonction la variable qui va contenir le chiffre et la taille min et max du chiffre*/

    int nbCharMax=0; /* =0 est l'identifiant pour une demande de nombre a keyboard_char */
    int numberTemp=0;
    char numberText[nbDigitMax]; // équivalent à 99999

    do {
      printf("\nVeuillez saisir un nombre entier compris entre %d et %d : ",min,max );

      if (keyboard_char(numberText,nbCharMax)==0){ /*  On appel la fonction saisie clavier que l'on test egalement  */
          sscanf(numberText,"%d", &numberTemp); /*converti la chaine de caractere en int */
      }
      else{
          return 1; /*  Si erreur du programme de saisie on retourne 1  */
      }
    } while(numberTemp<min || numberTemp>max);

    *number=numberTemp;

    return 0;  /* Saisie sans erreur on retourne 0  */
}
