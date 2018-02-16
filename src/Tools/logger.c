#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "logger.h"

void log_print(LEVEL_LOG level, int line, char *file, const char *format, ...)
{
  int dbgLevel = LEVEL_LOG_DEBUG; // Niveau de log par defaut
  int max_va_list_size = 4146;  // Taille max des arguments
  char* outColor;               // Pointer sur la couleur
  char* va_msg;                 // Chaine des arguments
  int msgSize;                  // Taille du message
  char* msg;                    // Message
  char info[255];
  va_list args;                 // Liste des arguments


  // initialisation des arguments
  va_start(args, format);

  // Allocation de la chaine de caractere des arguments
  va_msg = (char*)malloc(strlen(format) + max_va_list_size);

  // Creation de la chaine de caractere des arguments
  vsnprintf(va_msg, strlen(format) + max_va_list_size, format, args);

  sprintf(info, "%s:%d ", file, line);

  // Taille du message
  msgSize = strlen(info) + strlen(va_msg) + SIZE_HEADER  + strlen( strerror(errno)) + SIZE_MARGE;

  // Allocation de la variable du message complet
  msg = (char*)malloc(msgSize);

  sprintf(msg, "\t%s", info);

  /* Verifie le niveau du log avec le niveau demande
     et ajoute l'entete du message ainsi que la couleur */
  if(level == LEVEL_LOG_INFO) {
    outColor = COL_INFO;
    sprintf(msg + strlen(info) + 1, "INFO  : ");
  } else if(level == LEVEL_LOG_WARN && dbgLevel >= LEVEL_LOG_WARN) {
    outColor = COL_WARN;
    sprintf(msg + strlen(info) + 1, "WARN  : ");
  } else if(level == LEVEL_LOG_DEBUG && dbgLevel >= LEVEL_LOG_DEBUG) {
    outColor = COL_DEBUG;
    sprintf(msg + strlen(info) + 1, "DEBUG : ");
  } else if(level == LEVEL_LOG_ERROR) {
    outColor = COL_ERROR;
    sprintf(msg + strlen(info) + 1, "ERROR : ");
  }

  // Creation de la chaine de caractere du message
  sprintf(msg + strlen(msg), "%s\n", va_msg);

  // Si errno contient autre chose que "Success", on ecrit dans le log
  if(errno) {
    // On ajout a la fin du precedent message
    sprintf(msg + strlen(msg), "\t\t-> errno : %s\n", strerror(errno));
    // Clear errno
    errno = 0;
  }

  // On ecrit le code couleur
  write(STDOUT_FILENO, outColor, strlen(outColor));
  // On ecrit le message
  write(STDOUT_FILENO, msg, strlen(msg));

  // Fin de la gestion des arguments
  va_end(args);

  // Liberation de la memoire
  free(va_msg);
  free(msg);
}
