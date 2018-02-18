#ifndef HEADER_KEYBOARD
#define HEADER_KEYBOARD

/*! \file keyboard.h
    \brief Gestionnaire des saisies clavier.
    \version 0.1

    Permet de securiser la saisie operateur des caracteres et des entiers.
*/

/*! \def nbDigitMax
    \brief Definition du nombre de digit d'un entier saisi par l'operateur
*/
#define nbDigitMax 5 // équivalent à 99999


/*! \fn int ClearBuffer(void)
    \brief Vide le buffer du clavier.

    \param level Niveau du message.
    \param format Format du message.
    \param ... Liste d'arguments variable.
*/
int ClearBuffer(void); /* fonction de vidage buffer avant saisie clavier */

/*! \fn int KeyboardChar(char *string, int nbCharMax)
    \brief Securise et borne la saisie clavier des caracteres.

    \param level Niveau du message.
    \param format Format du message.
    \param ... Liste d'arguments variable.
*/
int KeyboardChar(char *string, int nbCharMax); /* fonction saisie de caracteres sécurisée */

/*! \fn int KeyboardInt(int *number, int min, int max)
    \brief Securise et borne la saisie clavier des entiers.

    \param level Niveau du message.
    \param format Format du message.
    \param ... Liste d'arguments variable.
*/
int KeyboardInt(int *number, int min, int max);/* fonction saisie d'entiers sécurisée */

#endif /* HEADER_KEYBOARD */
