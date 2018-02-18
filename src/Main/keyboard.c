#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<keyboard.h>

void ClearBuffer(void){
  int c=0;
  while(c!='\n' && c!=EOF){ /*  On parcours le buffer jusqu'à la fin pour le décharger dans la variable temporaire  */
    c=getchar();
  }
}
int KeyboardReading(char *string, int size){/*  On passe à la fonction le tab de char et la taille du tab  */

      char *charReturn = NULL;

      if (fgets(string, size, stdin) != NULL){/*  Lecture de la saisie clavier avec test d'erreur  */
          charReturn = strchr(string, '\n');/*  Recherche du caractere de retour chariot (généré lors de la saisie par le "entrer" opérateur)  */
          if (charReturn != NULL) /*  Si on trouve le \n  */
          {
              *charReturn = '\0';} /*  On le remplace par \0  */
          else{
              ClearBuffer();/*  Vidage buffer dans tous les cas par sécurité  */
          }
          return 1; /*  Saisie sans erreur on retourne 1  */
      }
      else{
          ClearBuffer();
          return 0; /*  Erreur de saisie, on vide le buffer malgré tout  */
      }
}
long KeyboardLong(){
    char numberText[12] = {0}; /*  Chiffre à 12digit max  */

    if (KeyboardReading(numberText,100)==1){ /*  On appel la fonction saisie clavier que l'on test également  */
        return strtol(numberText, NULL, 10); /*  Si la saisie s'est bien passé on retourne le texte convertit en long  */
    }
    else{
        return -1; /*  Si erreur on retourne 0  */
    }
}
