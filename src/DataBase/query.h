#ifndef HEADER_QUERY
#define HEADER_QUERY

#include "dataBase.h"

<<<<<<< HEAD
=======
/*! \enum DATA_FIELD
    \brief Gere le type de la donnees.
*/
typedef enum {
  CONDITION_EQUAL = 0, /**< Condition egale a */
  CONDITION_GREAT,     /**< Condition superieur a */
  CONDITION_LOW        /**< Condition inferieur a */
}CONDITION;

>>>>>>> dev_adrien
/*! \struct DescriptorTable
    \brief TODO.
*/
typedef struct {
  int nbField;          /**< TODO. */
  int sizeRecord;       /**< TODO. */
  DataField *dataField; /**< TODO. */
} DescriptorTable;

/*! \struct Query
    \brief TODO.
*/
typedef struct {
  DescriptorTable descriptor; /**< TODO. */
<<<<<<< HEAD
  int nbRecord;                /**< TODO. */
  char *data;                 /**< TODO. */
} Query;

=======
  int nbRecord;               /**< TODO. */
  char *data;                 /**< TODO. */
} Query;

/*! \struct Condition
    \brief TODO.
*/
typedef struct {
  char field[DATA_FIELD_MAX_CHARACTER]; /**< TODO. */
  char value[DATA_FIELD_MAX_CHARACTER]; /**< TODO. */
  CONDITION type;                       /**< TODO. */
} Condition;

>>>>>>> dev_adrien
/*! \fn DATA_BASE createDescriptorResult(HeaderTable *header, Query query, char listField[][DATA_FIELD_MAX_CHARACTER])
    \brief TODO.

    \param TODO.
    \return TODO.
*/
DATA_BASE createDescriptorResult(HeaderTable *header, Query *query, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField);

/*! \fn Query * excuteQuery(DataBase *dataBase, char *sql)
    \brief TODO.

    \param dataBase Reference de la DataBase.
    \param TODO.
    \return TODO.
*/
Query * excuteQuery(DataBase *dataBase, char *sql);

/*! \fn DATA_FIELD closeQuery(Query *query)
    \brief TODO.

    \param TODO.
    \return TODO.
*/
DATA_BASE closeQuery(Query *query);

/*! \fn DATA_FIELD getTypeQueryById(Query * query, int index)
    \brief TODO.

    \param dataBase Reference de la DataBase.
    \param TODO.
    \return TODO.
*/
DATA_FIELD getTypeQueryById(Query * query, int index);

/*! \fn char * getNameQueryById(Query * query, int index)
    \brief TODO.

    \param dataBase Reference de la DataBase.
    \param TODO.
    \return TODO.
*/
char * getNameQueryById(Query * query, int index);

/*! \fn char * getDataQueryById(Query * query, int index)
    \brief TODO.

    \param dataBase Reference de la DataBase.
    \param TODO.
    \return TODO.
*/
char * getDataQueryById(Query * query, int row, int colum);

/*! \fn char * getDataQueryByName(Query * query, char * fieldName)
    \brief TODO.

    \param dataBase Reference de la DataBase.
    \param TODO.
    \return TODO.
*/
char * getDataQueryByName(Query * query, int row, char * fieldName);

/*! \fn Query * commandSelect(HeaderTable header, char *listField, int nbField)
    \brief TODO.

    \param TODO.
    \param TODO.
    \return TODO.
*/
Query * commandSelect(HeaderTable *header, FILE *file, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField);

/*! \fn DATA_BASE commandInsert(HeaderTable *header, FILE *file, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField, char listValue[][DATA_FIELD_MAX_CHARACTER])
    \brief TODO.

    \param TODO.
    \param TODO.
    \return TODO.
*/
DATA_BASE commandInsert(HeaderTable *header, FILE *file, char listField[][DATA_FIELD_MAX_CHARACTER], int nbField, char listValue[][DATA_FIELD_MAX_CHARACTER]);

<<<<<<< HEAD
=======
/*! \fn DATA_BASE checkCondition(HeaderTable *header, Condition *condition, char *record)
    \brief TODO.

    \param TODO.
    \param TODO.
    \return TODO.
*/
DATA_BASE checkCondition(HeaderTable *header, Condition *condition, char *record);

>>>>>>> dev_adrien
#endif /* HEADER_QUERY */
