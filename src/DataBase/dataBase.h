#ifndef HEADER_DATA_BASE
#define HEADER_DATA_BASE

/*! \file dataBase.h
    \brief Gestionnare de la base de donnees
    \version 0.1

    Contient les fonctions de gestion de la DataBase.
*/

#include <stdio.h>

/*! \def CHARACTER_RECORD_VALID
    \brief TODO.
*/
#define CHARACTER_RECORD_VALID '\0'

/*! \def CHARACTER_RECORD_INVALID
    \brief TODO.
*/
#define CHARACTER_RECORD_INVALID '#'

/*! \def DATA_FIELD_MAX_CHARACTER
    \brief TODO.
*/
#define DATA_FIELD_MAX_CHARACTER 20

/*! \def TABLE_MAX_COLUM_CHARACTER
    \brief TODO.
*/
#define TABLE_MAX_COLUM_CHARACTER 1000

/*! \def PATH_MAX_CHARACTER
    \brief TODO.
*/
#define PATH_MAX_CHARACTER 100

/*! \def PATH_BASE
    \brief TODO.
*/
#define PATH_BASE "../ressources/bases/"

/*! \struct Date
    \brief TODO.
*/
typedef struct {
  short day;   /**< TODO. */
  short month; /**< TODO. */
  short year;  /**< TODO. */
} Date;

/*! \struct Table
    \brief TODO.
*/
typedef struct {
  char name[DATA_FIELD_MAX_CHARACTER]; /**< Contient le nom de la table. */
  FILE *file; /**< Reference du fichier de la table. */
} Table;

/*! \struct DataBase
    \brief Structure de gestion de la base de donnees.
*/
typedef struct {
  int nbTable; /**< Contient le nombre de table. */
  Table *tables; /**< TODO. */
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
    \brief Structure TODO.
*/
typedef struct {
  int offset; /**< La position de la donnees dans . */
  DATA_FIELD type; /**< Le type de la donnees. */
  char name[DATA_FIELD_MAX_CHARACTER]; /**< TODO. */
  int size; /**< TODO. */
} DataField;


/*! \enum DATA_BASE
    \brief Gere la valeur de retour.
*/
typedef enum {
  DATA_BASE_FAILURE = 0, /**< Etat echec */
  DATA_BASE_SUCCESS      /**< Etat succes */
}DATA_BASE;

/*! \struct HeaderTable
    \brief TODO.
*/
typedef struct {
  short valid;           /**< TODO. */
  Date lastUpdate;       /**< TODO. */
  int nbRecord;          /**< TODO. */
  int nbField;           /**< TODO. */
  int lengthHeader;      /**< TODO. */
  int lengthRecord;      /**< TODO. */
  int lengthField;       /**< TODO. */
  DataField *descriptor; /**< TODO. */
} HeaderTable;

/*! \fn DataBase * connect(char *pathBase)
    \brief Cette fonction initialise la DataBase.

    \param dataBase Reference de la DataBase a initialise.
    \param pathBase Chemin d'acces aux bases. Si NULL => chemin par defaut.
    \return Renvoie la valeur DATA_BASE.
*/
DataBase * connect(char *pathBase);

/*! \fn  DATA_BASE close(DataBase *dataBase)
    \brief Cette fonction detruit la reference a la DataBase.

    \param dataBase Reference de la DataBase a detruire.
    \return Renvoie la valeur DATA_BASE.
*/
DATA_BASE close(DataBase *dataBase);

/*! \fn HeaderTable * getHeaderTable(FILE* tableFile)
    \brief TODO.

    \param tableFile Reference du fichier de la table.
    \return Renvoie l'entete de la table.
*/
HeaderTable * getHeaderTable(FILE* tableFile);

/*! \fn void destroyHeader(HeaderTable *header)
    \brief TODO.

    \param TODO
*/
void destroyHeader(HeaderTable *header);

/*! \fn FILE* searchTable(DataBase *dataBase, char* nameTable)
    \brief TODO.

    \param dataBase Reference de la DataBase.
    \param nameTable Nom de la table.
    \return Reference du fichier de la table.
*/
FILE* searchTable(DataBase *dataBase, char* nameTable);

/*! \fn char * getRecord(HeaderTable *header, FILE* file, int indexRecord)
    \brief TODO.

    \param dataBase Reference de la DataBase.
    \param TODO.
    \return TODO.
*/
char * getRecord(HeaderTable *header, FILE* file, int indexRecord);

/*! \fn DATA_BASE recordIsValid(char *record)
    \brief TODO.

    \param TODO.
    \return TODO.
*/
DATA_BASE recordIsValid(char *record);

/*! \fn openTables(DataBase *dataBase)
    \brief TODO.

    \param dataBase Reference de la DataBase.
*/
void openTables(DataBase *dataBase);

/*! \fn void openTable(DataBase *dataBase, char *table);
    \brief TODO.

    \param dataBase Reference de la DataBase.
*/
void openTable(DataBase *dataBase, char *table);

/*! \fn void closeTables(DataBase *dataBase)
    \brief TODO.

    \param dataBase Reference de la DataBase.
*/
void closeTables(DataBase *dataBase);

/*! \fn restoreTables(DataBase *dataBase)
    \brief TODO.

    \param dataBase Reference de la DataBase.
*/
void restoreTables(DataBase *dataBase);
#endif /* HEADER_DATA_BASE */
