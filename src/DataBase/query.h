#ifndef HEADER_QUERY
#define HEADER_QUERY

#include "dataBase.h"

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
  int nbRecord;                /**< TODO. */
  char *data;                 /**< TODO. */
} Query;

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

#endif /* HEADER_QUERY */
