#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "logger.h"
#include "openFile.h"

int openFile(FILE **file, char *pathFile, char *mode) {
  int success = OPEN_FILE_SUCCESS;  // Valeur de retour

  // Ouverture du fichier
  *file = fopen(pathFile, mode);

  /* Si l'ouverture du fichier a echoue et qu'on n'est pas en mode w+
  la fonction se rappelle elle meme avec le mode w+ */
  if (*file == NULL && strcmp(mode, "w+") != 0) {
    WARN("Impossible de lire le fichier %s ouverure en w+ !", pathFile);
    success = openFile(file, pathFile, "w+");
  }
  /* Si l'ouverture du fichier a echoue et qu'on est en mode w+
  alors on s'arrete */
  else if (*file == NULL) {
    ERROR("Impossible de lire le fichier %s meme en w+ !", pathFile);
    success = OPEN_FILE_FAILURE;
  }

  return success;
}

ListFiles * getListFile(char *pathRep, char *extension) {
  ListFiles *list = NULL;
  DIR* rep = NULL;
  struct dirent* fichierLu = NULL; /* Déclaration d'un pointeur vers la structure dirent. */

  rep = opendir(pathRep);
  if (rep == NULL) {
    return NULL;
  }

  list = (ListFiles*)malloc(sizeof(ListFiles));

  if (list == NULL) {
    closedir(rep);
    return NULL;
  }

  strcpy(list->extension, extension);
  list->nbFiles = 0;
  list->nameFiles = NULL;

  while ((fichierLu = readdir(rep)) != NULL) {
    if (strstr (fichierLu->d_name, list->extension) != NULL) {
      list->nbFiles++;
      if (list->nameFiles == NULL) {
        list->nameFiles = malloc(sizeof(char*));
      }
      else {
        list->nameFiles = realloc(list->nameFiles, sizeof(char*) * list->nbFiles);
      }

      list->nameFiles[list->nbFiles-1] = NULL;
      list->nameFiles[list->nbFiles-1] = malloc(sizeof(char) * 100);
      strcpy(list->nameFiles[list->nbFiles-1], strtok(fichierLu->d_name, "."));
    }
  }

  if (closedir(rep) == -1) {
    return NULL;
  }

  return list;
}

void closeListFile(ListFiles *list) {
  if (list->nbFiles > 0) {
    for (int i = 0; i < list->nbFiles; i++) {
      free(list->nameFiles[i]); // On libère chaque ligne
    }

    free(list->nameFiles); // On libère la première dimension
  }

  free(list); // On libère la structure
}
