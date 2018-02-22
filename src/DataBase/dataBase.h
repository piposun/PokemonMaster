#ifndef HEADER_DATA_BASE
#define HEADER_DATA_BASE

/*! \file dataBase.h
    \brief Gestionnare de la base de donnees
    \version 0.1

    Contient les fonctions de gestion de la DataBase.
*/

#include <stdio.h>

/*! \def CHARACTER_RECORD_VALID
    \brief Définition du caractère valide d'un enregistrement.
*/
#define CHARACTER_RECORD_VALID '\0'

/*! \def CHARACTER_RECORD_INVALID
    \brief Définition du caractère invalide d'un enregistrement.
*/
#define CHARACTER_RECORD_INVALID '#'

/*! \def DATA_FIELD_MAX_CHARACTER
    \brief Définition de la taille max du nom des champs dans la base.
*/
#define DATA_FIELD_MAX_CHARACTER 20

/*! \def TABLE_MAX_COLUM_CHARACTER
    \brief Définition de la taille max de la valeur des champs dans la base.
*/
#define TABLE_MAX_COLUM_CHARACTER 1000

/*! \def PATH_MAX_CHARACTER
    \brief Définition de la taille max du chemin d'accès des fichiers.
*/
#define PATH_MAX_CHARACTER 100

/*! \def PATH_BASE
    \brief Définition du chemin d'accès des fichiers.
*/
#define PATH_BASE "../ressources/bases/"

/*! \struct Date
    \brief Structure contenant la date en jour, mois, année.
*/
typedef struct {
  short day;   /**< Champ jour. */
  short month; /**< Champ mois. */
  short year;  /**< Champ année. */
} Date;

/*! \struct Table
    \brief Structure d'une table de la base.
*/
typedef struct {
  char name[DATA_FIELD_MAX_CHARACTER]; /**< Contient le nom de la table. */
  FILE *file; /**< Référence du fichier de la table. */
} Table;

/*! \struct DataBase
    \brief Structure de gestion de la base de donnees.
*/
typedef struct {
  int nbTable; /**< Contient le nombre de table. */
  Table *tables; /**< Pointer sur l'ensemble des tables. */
} DataBase;

/*! \enum DATA_FIELD
    \brief Gere le type de la donnees.
*/
typedef enum {
  DATA_FIELD_PK = 0,  /**< Type primary_key. */
  DATA_FIELD_INT,     /**< Type int. */
  DATA_FIELD_CHAR,    /**< Type char. */
  DATA_FIELD_FLOAT,   /**< Type float. */
  DATA_FIELD_DOUBLE,  /**< Type double. */
  DATA_FIELD_UNKNOWN, /**< Type inconnu. */
}DATA_FIELD;

/*! \struct DataField
    \brief Structure d'un élément de description d'un champ dans une table.
*/
typedef struct {
  int offset; /**< La position de la donnees dans . */
  DATA_FIELD type; /**< Le type de la donnees. */
  char name[DATA_FIELD_MAX_CHARACTER]; /**< Le nom du champ. */
  int size; /**< La taille du champ. */
} DataField;


/*! \enum DATA_BASE
    \brief Gere la valeur de retour.
*/
typedef enum {
  DATA_BASE_FAILURE = 0, /**< Etat echec */
  DATA_BASE_SUCCESS      /**< Etat succes */
}DATA_BASE;

/*! \struct HeaderTable
    \brief Structure de l'entête d'une table de la base.
*/
typedef struct {
  short valid;           /**< Champ de validité de la table. */
  Date lastUpdate;       /**< Champ de la dernière mise à jour. */
  int nbRecord;          /**< Nombre d'enregistrement dans la table. */
  int nbField;           /**< Nombre de champs par enregistrement. */
  int lengthHeader;      /**< Taille de l'entête. */
  int lengthRecord;      /**< Taille des enregistremnts. */
  int lengthField;       /**< Taille de l'entête de description des champs. */
  DataField *descriptor; /**< Pointer sur les descriptions des champs. */
} HeaderTable;

/*! \fn DataBase * connect(char *pathBase)
    \brief Cette fonction initialise la DataBase.

    \param pathBase Chemin d'accès aux bases. Si NULL => chemin par défaut.
    \return Renvoie la référence de la DataBase à initialise.
*/
DataBase * connect(char *pathBase);

/*! \fn  DATA_BASE close(DataBase *dataBase)
    \brief Cette fonction detruit la Référence à la DataBase.

    \param dataBase Référence de la DataBase à detruire.
    \return Renvoie la valeur DATA_BASE.
*/
DATA_BASE close(DataBase *dataBase);

/*! \fn HeaderTable * getHeaderTable(FILE* tableFile)
    \brief Cette fonction initialise l'entete d'un table passé en argument.

    \param tableFile Référence du fichier de la table.
    \return Renvoie l'entete de la table.
*/
HeaderTable * getHeaderTable(FILE* tableFile);

/*! \fn void destroyHeader(HeaderTable *header)
    \brief Détruit la référence de l'entête d'une table passé en argument.

    \param Référence de l'entête de la table à détruire.
*/
void destroyHeader(HeaderTable *header);

/*! \fn FILE* searchTable(DataBase *dataBase, char* nameTable)
    \brief Cette fonction recherche la table dans la base de donnée et retourne le fichier asocié.

    \param dataBase Référence de la DataBase.
    \param nameTable Nom de la table.
    \return Référence du fichier de la table.
*/
FILE* searchTable(DataBase *dataBase, char* nameTable);

/*! \fn char * getRecord(HeaderTable *header, FILE* file, int indexRecord)
    \brief Cette fonction permet de récupérer un enregistrement dans une table.

    \param header Référence de l'entête de la table.
    \param file Référence du fichier de la table.
    \param indexRecord Le numéro de l'enregistrement à récupérer.
    \return Référence de l'enregistrement.
*/
char * getRecord(HeaderTable *header, FILE* file, int indexRecord);

/*! \fn DATA_BASE recordIsValid(char *record)
    \brief Cette fonction vérifie si l'enregistrement est valide.

    \param Référence de l'enregistrement.
    \return Renvoie DATA_BASE. => DATA_BASE_SUCCESS si l'enregistrement est valide.
*/
DATA_BASE recordIsValid(char *record);

/*! \fn openTables(DataBase *dataBase)
    \brief Cette fonction initialise toutes les tables disponibles dans le répertoire ressources/bases.

    \param dataBase Référence de la DataBase.
*/
void openTables(DataBase *dataBase);

/*! \fn void openTable(DataBase *dataBase, char *table);
    \brief Cette fonction initialise une table via son nom.

    \param dataBase Référence de la DataBase.
    \param dataBase Nom de la table à initialiser.
*/
void openTable(DataBase *dataBase, char *table);

/*! \fn void closeTables(DataBase *dataBase)
    \brief Cette fonction détruit toutes les références des tables de la base.

    \param dataBase Référence de la DataBase.
*/
void closeTables(DataBase *dataBase);

/*! \fn restoreTables(DataBase *dataBase)
    \brief Cette fonction restores les tables dans un état d'usine.

    \param dataBase Référence de la DataBase.
*/
void restoreTables(DataBase *dataBase);
#endif /* HEADER_DATA_BASE */
