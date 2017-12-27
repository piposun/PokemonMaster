#ifndef HEADER_DATA_BASE
#define HEADER_DATA_BASE

/*! \file dataBase.h
    \brief Gestionnare de la base de donnees
    \version 0.1

    Contient les fonctions de gestion de la DataBase.
*/

/*! \struct DataBase
    \brief Structure de gestion de la base de donnees.
*/
typedef struct {
  int nbTable; /**< Contient le nombre de table. */
} DataBase;

/*! \enum DATA_BASE
    \brief Gere la valeur de retour.
*/
typedef enum {
  DATA_BASE_FAILURE = 0, /**< Etat echec */
  DATA_BASE_SUCCESS /**< Etat succes */
}DATA_BASE;

/*! \fn DataBase *dataBase connect()
    \brief Cette fonction initialise la DataBase.

    \return Reference de la DataBase.
*/
DataBase* connect();

/*! \fn  DATA_BASE close(DataBase *dataBase)
    \brief Cette fonction detruit la reference a la DataBase.

    \param dataBase Reference de la DataBase a detruire.
    \return Renvoie la valeur DATA_BASE.
*/
DATA_BASE close(DataBase *dataBase);

#endif /* HEADER_DATA_BASE */
