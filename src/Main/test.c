#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define nbDigitMax 5 // équivalent à 99999

int ClearBuffer(void){
  int buffer=0, i=0;
  while( buffer=getchar() != '\n' && buffer != EOF) { /*  On parcours le buffer jusqu'à la fin pour le decharger dans la variable temporaire */
    i++; /* Si la variable de comptage "i" s'est incrementee, c'est qu'il restait des caracteres dans le buffer stdin*/
  }
  return i;
}

int KeyboardChar(char *string, int nbCharMax){ /*  On passe à la fonction la variable qui va contenir la chaine de caractere et le nombre de caracteres maximum*/

      char *charReturn = NULL; /* Pointeur qui va pointer l'adresse du caractere "\n" */
      int max=0;

      if (nbCharMax==0){
        nbCharMax=nbDigitMax;  // 5 équivalent à 99999
      }
      else{
        printf("\nVeuillez saisir vos donnees au clavier, attention %d caracteres maximum : ",nbCharMax-1);
      }

      do {
        if (fgets(string, nbCharMax, stdin) != NULL) { /*  Lecture de la saisie clavier avec test d'erreur */
          charReturn = strchr(string, '\n'); /*  Recherche du caractere "\n" genere lors de la saisie par le "entrer" operateur) */
            if (charReturn != NULL) { /*  Si on trouve le \n  */
              *charReturn = '\0'; /*  On le remplace par caractere "\0" */
              max=0; /* Necessaire en cas de boucle */
            }
            else{ /* Si on n'a pas trouver le /n, alors le buffer n'est pas vide */
              max=ClearBuffer();/*  Donc on vide le buffer  */
            }
        }
        else{  /* Si erreur de la commande de saisie */
            max=ClearBuffer(); /* Par sécurite, on vide le buffer */
            return 1; /* Erreur de saisie, retourne 1 */
        }

        if(strlen(string)==0){printf("\nVous devez saisir au moins un caractère. Veuillez saisir une donnee correcte : ");}
        else{
          if(max>0){
            printf("\nVous avez saisi trop de caractère. Veuillez saisir une donnee correcte : ");
          }
        }

      }while (max>0 || strlen(string)==0); /*On boucle si la saisie operateur est vide ou superieur au nombre de caractere maximum*/
      return 0; /* Saisie sans erreur on retourne 0 */
}
int KeyboardInt(int *number, int min, int max) { /*  On passe à la fonction la variable qui va contenir le chiffre et la taille min et max du chiffre*/

    int nbCharMax=0; /* =0 est l'identifiant pour une demande de nombre a KeyboardChar */
    int numberTemp=0;
    char numberText[nbDigitMax]; // équivalent à 99999

    do {
      printf("\nVeuillez saisir un nombre entier compris entre %d et %d : ",min,max );
      if (KeyboardChar(numberText,nbCharMax)==0){ /*  On appel la fonction saisie clavier que l'on test egalement  */
          sscanf(numberText,"%d", &numberTemp); /*converti la chaine de caractere en int */
      }
      else{
          return 1; /*  Si erreur du programme de saisie on retourne 1  */
      }
    } while(numberTemp<min || numberTemp>max);

    *number=numberTemp;
    return 0;  /* Saisie sans erreur on retourne 0  */
}


int main(void){

  char boby[20]={0};
  int lapointe=0;
  int *pt_lapointe=&lapointe;
  int testreading=0, testlong=0;
  int min=5, max=20;
  do{
    printf("\nSaisir du texte\n");
    testreading=KeyboardChar(boby,max);
    printf("texte saisi: %s\nresultat=%d\n",boby,testreading);

    printf("\nSaisir un entier\n");
    testlong=KeyboardInt(pt_lapointe,min,max);
    printf("chiffre saisi: %d\nresultat=%d\n",lapointe,testlong);
  }while(1);

  return 0;
}
