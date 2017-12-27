#include <stdlib.h>
#include <string.h>

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
