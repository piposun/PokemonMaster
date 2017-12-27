#ifndef HEADER_OPEN_FILE
#define HEADER_OPEN_FILE

/*! \file openFile.h
    \brief Gestionnare de fichier
    \version 0.1

    Contient les fonctions de gestion de fichiers.
*/

#include <stdio.h>

/*! \enum
    \brief Gere la valeur de retour.
*/
typedef enum {
  OPEN_FILE_FAILURE = 0, /**< Etat echec */
  OPEN_FILE_SUCCESS /**< Etat succes */
} OPEN_FILE;

/*! \fn int openFile(FILE **file, char *pathName, char *mode)
    \brief Cette fonction ouvre un fichier.

    \param file File descriptor.
    \param pathName Chemin d'acces du fichier a ouvrir.
    \param mode Mode d'ouverture du fichier.
    \return Indique si l'ouverture du fichier a reussi.
*/
int openFile(FILE **file, char *pathName, char *mode);

#endif /* HEADER_OPEN_FILE */
