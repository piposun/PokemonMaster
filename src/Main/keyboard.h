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
#define nbDigitMax 5


/*! \fn int clearBuffer(void);
    \brief Cette fonction vide le buffer du clavier.

    \return Retourne le nombres de caractères qu'il restait dans le buffer stdin.
*/
int clearBuffer(void);

/*! \fn int keyboardChar(char *string, int nbCharMax);
    \brief Cette fonction securise et borne la saisie clavier des caracteres.

    \param string pointeur sur variable contenant la saisie clavier.
    \param nbCharMax taille de la chaine de caracteres.
    \return La fonction renvoie un entier de valeur 0 si la saisie s'est deroulee sans erreur et 1 si erreur de saisie
*/
int keyboardChar(char *string, int nbCharMax);

/*! \fn int keyboardInt(int *number, int min, int max);
    \brief Cette fonction securise et borne la saisie clavier des entiers.

    \param number pointeur sur la chaine de caractere saisie au clavier (nombres).
    \param min valeur minimum du chiffre a saisir.
    \param max valeur maximum du chiffre a saisir.
    \return La fonction renvoie un entier de valeur 0 si la saisie s'est deroulee sans erreur et 1 si erreur de saisie
*/
int keyboardInt(int *number, int min, int max);

#endif /* HEADER_KEYBOARD */
