#ifndef HEADER_MENU
#define HEADER_MENU

#include "dataBase.h"
#include "logger.h"

/*! \file menu.h
    \brief Menu d'affichage et de selection des cas d'utilisation de la base de donnees.
    \version 0.1

    Permet d'afficher le menu à l'utilisateur et de selectionner les donnees qu'il souhaite afficher.
*/


/*! \fn void menu(DataBase *dataBase);
    \brief Fonction qui appel l'IHM.
    \param dataBase pointeur sur la base de donnees.
*/
void menu(DataBase *dataBase);

/*! \fn void userRequestsMenu(void);
    \brief Fonction qui affiche le menu des cas d'utilisation de la base de donnees.
*/
void userRequestsMenu(void);

/*! \fn void userRequestsSelection(int *requestChoice);
    \brief Fonction qui selectionne le bon cas d'utilisation en fonction de la demande utilisateur.

    \param requestChoice variable de selection saisie par l'utilisateur.
    \param dataBase pointeur sur la base de donnees.
*/
void userRequestsSelection(int *requestChoice, DataBase *dataBase);

#endif /* HEADER_MENU */
